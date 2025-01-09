#include "PixelPipeline.h"

PixelPipeline::PixelPipeline(Uint8* video_ram, Uint32* system_colors, Uint8* bg_palette, Uint8* obj_palette_0, Uint8* obj_palette_1, Uint8* lcd_control)
: video_ram(video_ram),
  bg_palette(bg_palette),
  obj_palette_0(obj_palette_0),
  obj_palette_1(obj_palette_1),
  lcd_control(lcd_control),
  pixel_fetcher(video_ram) {

    // nothing to do
}

// ----------------------------------------------------------------------------

PixelPipeline::~PixelPipeline() {
    // nothing to do
}

// ----------------------------------------------------------------------------

void PixelPipeline::reset(Uint8 screen_x, Uint8 screen_y, Uint8 viewport_x, Uint8 viewport_y) {
    clock_cycles = 0;

    while (!bg_fifo.empty()) {
        bg_fifo.pop();
    }

    while (!obj_fifo.empty()) {
        obj_fifo.pop();
    }

    Uint16 top_left_x = viewport_x + screen_x;
    Uint16 top_left_y = viewport_y + screen_y;

    pixels_to_discard = top_left_x % 8;
    Uint8 y_offset = top_left_y % 8;

    Uint8 leftmost_viewport_tile_column = (top_left_x / 8) % 32;
    Uint8 leftmost_viewport_tile_row = (top_left_y / 8) % 32;

    pixel_fetcher.reset(leftmost_viewport_tile_column, leftmost_viewport_tile_row, y_offset);

    state = INITIAL_FULL_BG_PIXELS_FETCH;
}

// ----------------------------------------------------------------------------

void PixelPipeline::clock() {
    if (state == INITIAL_FULL_BG_PIXELS_FETCH && bg_fifo.size() == 16) {
        for (int i = 0; i < pixels_to_discard; i++) {
            bg_fifo.pop();
        }

        state = FETCHING_BG_PIXELS;
    }

    if (pixel_fetcher.is_ready_with_pixel()) {
        PixelInfo next_pixels[8];
        pixel_fetcher.get_pixels_and_continue(next_pixels);
        for (int i = 0; i < 8; i++) {
            bg_fifo.push(next_pixels[i]);
        }
    }

    clock_cycles++;
    if (clock_cycles % 2 == 0) {
        pixel_fetcher.clock(*lcd_control);
    }
}

// ----------------------------------------------------------------------------

bool PixelPipeline::is_ready_with_next_pixel() {
    return state == FETCHING_BG_PIXELS && bg_fifo.size() > 8;
}

// ----------------------------------------------------------------------------

Uint8 PixelPipeline::get_next_pixel_color_index() {
    Uint8 next_pixel_color_index = bg_fifo.front().color_index;
    bg_fifo.pop();

    int mapped_bg_color_index = (*bg_palette >> (next_pixel_color_index * 2)) & 0x03;

    if (!obj_fifo.empty()) {
        PixelInfo obj_pixel = obj_fifo.front();
        obj_fifo.pop();

        if (!obj_pixel.bg_has_priority || next_pixel_color_index == 0x00) {
            next_pixel_color_index = obj_pixel.color_index;
            Uint8 palette = obj_pixel.use_obj_1_palette ? *obj_palette_1 : *obj_palette_0;
            mapped_bg_color_index = (palette >> (next_pixel_color_index * 2)) & 0x03;
        }
    }

    return mapped_bg_color_index;
}

// ----------------------------------------------------------------------------

void PixelPipeline::load_obj_pixels(Uint8 obj_tile_id, Uint8 obj_attributes, Uint8 obj_y_offset) {
    // To keep the pixel fetcher's code a little cleaner, we'll cheat here
    // for now and just load object pixels on demand.

    while (!obj_fifo.empty()) {
        obj_fifo.pop();
    }

    Uint8 obj_height = ((*lcd_control >> 2) & 0x01) == 0x00 ? 8 : 16;

    bool bg_has_priority = ((obj_attributes >> 7) & 0x01) != 0x00;
    bool is_y_flipped = ((obj_attributes >> 6) & 0x01) != 0x00;
    bool is_x_flipped = ((obj_attributes >> 5) & 0x01) != 0x00;
    bool use_obj_palette_1 = ((obj_attributes >> 4) & 0x01) != 0x00;

    if (is_y_flipped) {
        obj_y_offset = obj_height - 1 - obj_y_offset;
    }

    Uint16 target_video_ram_address = obj_tile_id * 16 + obj_y_offset * 2;
    Uint8 tile_data_lsb = video_ram[target_video_ram_address++];
    Uint8 tile_data_msb = video_ram[target_video_ram_address];

    PixelInfo obj_pixels[8];
    for (int i = 0; i < 8; i++) {
        int target_bit = is_x_flipped ? i : 7 - i;
        int color_index = (((tile_data_msb >> target_bit) & 0x01) ? 0x02 : 0x00) | ((tile_data_lsb >> target_bit) & 0x01);

        obj_pixels[i].color_index = color_index;
        obj_pixels[i].use_obj_1_palette = use_obj_palette_1;
        obj_pixels[i].bg_has_priority = bg_has_priority;
    }

    for (int i = 0; i < 8; i++) {
        obj_fifo.push(obj_pixels[i]);
    }
}

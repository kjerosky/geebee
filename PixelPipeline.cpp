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

    while (!pixel_fifo.empty()) {
        pixel_fifo.pop_front();
    }

    while (!obj_fifo.empty()) {
        obj_fifo.pop_front();
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
    if (state == INITIAL_FULL_BG_PIXELS_FETCH && pixel_fifo.size() == 16) {
        for (int i = 0; i < pixels_to_discard; i++) {
            pixel_fifo.pop_front();
        }

        state = FETCHING_BG_PIXELS;
    }

    if (pixel_fetcher.is_ready_with_pixel()) {
        PixelInfo next_pixels[8];
        pixel_fetcher.get_pixels_and_continue(next_pixels);
        for (int i = 0; i < 8; i++) {
            pixel_fifo.push_back(next_pixels[i]);
        }
    }

    clock_cycles++;
    if (clock_cycles % 2 == 0) {
        pixel_fetcher.clock(*lcd_control);
    }
}

// ----------------------------------------------------------------------------

bool PixelPipeline::is_ready_with_next_pixel() {
    return state == FETCHING_BG_PIXELS && pixel_fifo.size() > 8;
}

// ----------------------------------------------------------------------------

Uint8 PixelPipeline::get_next_pixel_color_index() {
    PixelInfo next_pixel = pixel_fifo.front();
    pixel_fifo.pop_front();

    int next_pixel_mapped_color_index;
    if (next_pixel.is_obj_pixel) {
        Uint8 obj_palette = next_pixel.use_obj_1_palette ? *obj_palette_1 : *obj_palette_0;
        next_pixel_mapped_color_index = (obj_palette >> (next_pixel.color_index * 2)) & 0x03;
    } else {
        next_pixel_mapped_color_index = (*bg_palette >> (next_pixel.color_index * 2)) & 0x03;
    }

    return next_pixel_mapped_color_index;
}

// ----------------------------------------------------------------------------

void PixelPipeline::load_obj_pixels(Uint8 obj_tile_id, Uint8 obj_attributes, Uint8 obj_y_offset) {
    // To keep the pixel fetcher's code a little cleaner, we'll cheat here
    // for now and just load object pixels on demand.

    while (!obj_fifo.empty()) {
        obj_fifo.pop_front();
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
        obj_fifo.push_back(obj_pixels[i]);
    }

    mix_obj_pixels_with_bg_pixels();
}

// ----------------------------------------------------------------------------

void PixelPipeline::mix_obj_pixels_with_bg_pixels() {
    std::deque<PixelInfo> pixels_to_mix;
    for (int i = 0; i < obj_fifo.size(); i++) {
        PixelInfo pixel = pixel_fifo.front();
        pixel_fifo.pop_front();

        pixels_to_mix.push_back(pixel);
    }

    while (!pixels_to_mix.empty()) {
        PixelInfo current_pixel = pixels_to_mix.back();
        pixels_to_mix.pop_back();

        PixelInfo obj_pixel = obj_fifo.back();
        obj_fifo.pop_back();

        if ((obj_pixel.bg_has_priority && current_pixel.color_index == 0 && obj_pixel.color_index != 0) ||
            (!obj_pixel.bg_has_priority && obj_pixel.color_index != 0)) {
            pixel_fifo.push_front(obj_pixel);
        } else {
            pixel_fifo.push_front(current_pixel);
        }
    }
}

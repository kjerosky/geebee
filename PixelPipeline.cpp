#include "PixelPipeline.h"

PixelPipeline::PixelPipeline(Uint8* video_ram, Uint8* oam, Uint32* system_colors, Uint8* bg_palette, Uint8* obj_palette_0, Uint8* obj_palette_1, Uint8* lcd_control)
: bg_palette(bg_palette),
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

    state = INITIAL_FULL_FETCH;
}

// ----------------------------------------------------------------------------

void PixelPipeline::clock() {
    if (state == INITIAL_FULL_FETCH && bg_fifo.size() == 16) {
        for (int i = 0; i < pixels_to_discard; i++) {
            bg_fifo.pop();
        }

        state = STANDARD_OPERATION;
    }

    //todo - only bg for now
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
    return state == STANDARD_OPERATION && bg_fifo.size() > 8;
}

// ----------------------------------------------------------------------------

Uint8 PixelPipeline::get_next_pixel_color_index() {
    //todo - only bg for now
    Uint8 next_pixel_color_index = bg_fifo.front().color_index;
    bg_fifo.pop();

    int mapped_bg_color_index = (*bg_palette >> (next_pixel_color_index * 2)) & 0x03;
    return mapped_bg_color_index;
}

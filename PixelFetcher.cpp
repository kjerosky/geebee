#include "PixelFetcher.h"

PixelFetcher::PixelFetcher(Uint8* video_ram)
: video_ram(video_ram),
  state(FETCHING_TILE_ID) {

    // nothing to do
}

// ----------------------------------------------------------------------------

PixelFetcher::~PixelFetcher() {
    // nothing to do
}

// ----------------------------------------------------------------------------

void PixelFetcher::reset(Uint8 leftmost_viewport_tile_column, Uint8 leftmost_viewport_tile_row, Uint8 y_offset) {
    tile_column = leftmost_viewport_tile_column;
    tile_row = leftmost_viewport_tile_row;
    this->y_offset = y_offset;

    state = FETCHING_TILE_ID;
}

// ----------------------------------------------------------------------------

void PixelFetcher::clock(Uint8 lcd_control) {
    bool bg_and_obj_share_same_memory = ((lcd_control >> 4) & 0x01) == 0x01;

    Uint16 tile_map_offset = tile_row * 32 + tile_column;
    Uint16 base_bg_tile_map_address = (((lcd_control >> 3) & 0x01) == 0x00) ? 0x1800 : 0x1C00;

    switch (state) {
        case FETCHING_TILE_ID:
            tile_map_address = base_bg_tile_map_address + tile_map_offset;

            tile_id = video_ram[tile_map_address++];
            state = FETCHING_TILE_DATA_LSB;
            break;

        case FETCHING_TILE_DATA_LSB:
            tile_address = (bg_and_obj_share_same_memory ? tile_id : 0x100 + static_cast<Sint8>(tile_id)) * 16 + y_offset * 2;
            tile_data_lsb = video_ram[tile_address++];
            state = FETCHING_TILE_DATA_MSB;
            break;

        case FETCHING_TILE_DATA_MSB:
            tile_data_msb = video_ram[tile_address];

            tile_column = (tile_column + 1) % 32;

            state = PIXEL_IS_READY;
            break;

        default:
            // intentionally do nothing
            break;
    }
}

// ----------------------------------------------------------------------------

bool PixelFetcher::is_ready_with_pixel() {
    return state == PIXEL_IS_READY;
}

// ----------------------------------------------------------------------------

void PixelFetcher::get_pixels_and_continue(PixelInfo output_pixels[8]) {
    for (int i = 0; i < 8; i++) {
        int color_index = (((tile_data_msb >> (7 - i)) & 0x01) ? 0x02 : 0x00) | ((tile_data_lsb >> (7 - i)) & 0x01);
        output_pixels[i].color_index = color_index;

        // Background pixels don't care about this information.
        output_pixels[i].use_obj_1_palette = false;
        output_pixels[i].bg_has_priority = false;
    }

    state = FETCHING_TILE_ID;
}

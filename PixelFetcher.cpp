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

void PixelFetcher::reset(Uint16 start_tile_map_address, Uint8 y_offset) {
    tile_map_address = start_tile_map_address;
    this->y_offset = y_offset;
    state = FETCHING_TILE_ID;
}

// ----------------------------------------------------------------------------

void PixelFetcher::clock(Uint8 lcd_control) {
    bool bg_and_obj_share_same_memory = ((lcd_control >> 4) & 0x01) == 0x01;

    switch (state) {
        case FETCHING_TILE_ID:
            tile_id = video_ram[tile_map_address++];
            state = FETCHING_TILE_DATA_LSB;
            break;

        case FETCHING_TILE_DATA_LSB:
            tile_address = (bg_and_obj_share_same_memory ? tile_id : 0x1000 + static_cast<Sint8>(tile_id)) * 16 + y_offset * 2;
            tile_data_lsb = video_ram[tile_address++];
            state = FETCHING_TILE_DATA_MSB;
            break;

        case FETCHING_TILE_DATA_MSB:
            tile_data_msb = video_ram[tile_address];
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
    //todo
    for (int i = 0; i < 8; i++) {
        int color_index = (((tile_data_msb >> (7 - i)) & 0x01) ? 0x02 : 0x00) | ((tile_data_lsb >> (7 - i)) & 0x01);

        output_pixels[i].color_index = color_index;
        output_pixels[i].use_obj_1_palette = false;
        output_pixels[i].background_priority = false;
    }

    state = FETCHING_TILE_ID;
}

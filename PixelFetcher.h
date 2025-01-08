#ifndef PIXEL_FETCHER_H
#define PIXEL_FETCHER_H

#include <SDL2/SDL.h>

enum PixelFetcherState {
    FETCHING_TILE_ID,
    FETCHING_TILE_DATA_LSB,
    FETCHING_TILE_DATA_MSB,
    PIXEL_IS_READY,
};

struct PixelInfo {
    Uint8 color_index;
    bool use_obj_1_palette;
    bool background_priority;
};

class PixelFetcher {

public:

    PixelFetcher(Uint8* video_ram);
    ~PixelFetcher();

    void reset(Uint16 start_address, Uint8 y_offset);
    void clock(Uint8 lcd_control);
    bool is_ready_with_pixel();
    void get_pixels_and_continue(PixelInfo output_pixels[8]);

private:

    Uint8* video_ram;

    PixelFetcherState state;
    Uint16 tile_map_address;
    Uint8 tile_id;
    Uint8 y_offset;
    Uint16 tile_address;
    Uint8 tile_data_lsb;
    Uint8 tile_data_msb;
};

#endif

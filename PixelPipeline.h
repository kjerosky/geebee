#ifndef PIXEL_PIPELINE_H
#define PIXEL_PIPELINE_H

#include <SDL2/SDL.h>
#include <queue>

#include "PixelFetcher.h"

class PixelPipeline {

public:

    PixelPipeline(Uint8* video_ram, Uint8* oam, Uint32* system_colors, Uint8* bg_palette, Uint8* obj_palette_0, Uint8* obj_palette_1, Uint8* lcd_control);
    ~PixelPipeline();

    void reset(Uint8 screen_x, Uint8 screen_y, Uint8 viewport_x, Uint8 viewport_y);
    void clock();
    bool is_ready_with_next_pixel();
    Uint32 get_next_pixel_color_index();

private:

    Uint8* video_ram;
    Uint8* oam;
    Uint32* system_colors;
    Uint8* bg_palette;
    Uint8* obj_palette_0;
    Uint8* obj_palette_1;
    Uint8* lcd_control;

    std::queue<PixelInfo> bg_fifo;
    std::queue<PixelInfo> obj_fifo;
    PixelFetcher pixel_fetcher;

    PixelInfo get_next_bg_pixel();
    PixelInfo get_next_obj_pixel();
};

#endif

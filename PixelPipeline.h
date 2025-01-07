#ifndef PIXEL_PIPELINE_H
#define PIXEL_PIPELINE_H

#include <SDL2/SDL.h>

class PixelPipeline {

public:

    PixelPipeline(Uint8* video_ram, Uint8* oam, Uint32* system_colors);
    ~PixelPipeline();

    void clear();
    Uint32 get_next_pixel_color(Uint8 bg_palette, Uint8 obj_palette_0, Uint8 obj_palette_1);

private:

    Uint8* video_ram;
    Uint8* oam;
    Uint32* system_colors;

    Uint16 bg_fifo;
    Uint16 obj_fifo;
};

#endif

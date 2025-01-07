#include "PixelPipeline.h"

#include <cstdlib>

PixelPipeline::PixelPipeline(Uint8* video_ram, Uint8* oam, Uint32* system_colors)
: video_ram(video_ram),
  oam(oam),
  system_colors(system_colors) {

    clear();
}

// ----------------------------------------------------------------------------

PixelPipeline::~PixelPipeline() {
    // nothing to do
}

// ----------------------------------------------------------------------------

void PixelPipeline::clear() {
    bg_fifo = 0x0000;
    obj_fifo = 0x0000;
}

// ----------------------------------------------------------------------------

Uint32 PixelPipeline::get_next_pixel_color(Uint8 bg_palette, Uint8 obj_palette_0, Uint8 obj_palette_1) {
    //todo - output garbage for now
    int random_color_index = std::rand() % 4;
    return system_colors[random_color_index];
}

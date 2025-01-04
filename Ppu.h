#ifndef PPU_H
#define PPU_H

#include <SDL2/SDL.h>

class Ppu {

public:

    Ppu(SDL_Texture* screen_texture, SDL_PixelFormat* screen_texture_pixel_format);
    ~Ppu();

    void clock();
    void lock_screen_texture();
    void unlock_screen_texture();
    bool is_frame_complete();

private:

    SDL_Texture* screen_texture;
    SDL_PixelFormat* screen_texture_pixel_format;

    int scanline;
    int scanline_dot;
    bool frame_complete;

    Uint32* screen_pixels;
    int screen_pixels_row_length;

    static const int COLORS_SIZE = 4;
    Uint32 gameboy_pocket_colors[COLORS_SIZE];
};

#endif

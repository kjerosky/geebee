#include "Ppu.h"

#include <SDL2/SDL.h>
#include <cstdlib>

Ppu::Ppu(SDL_Texture* screen_texture, SDL_PixelFormat* screen_texture_pixel_format)
: screen_texture(screen_texture),
  screen_texture_pixel_format(screen_texture_pixel_format),
  scanline(0),
  scanline_dot(0),
  frame_complete(false) {
    gameboy_pocket_colors[0] = SDL_MapRGB(this->screen_texture_pixel_format, 0x00, 0x00, 0x00);
    gameboy_pocket_colors[1] = SDL_MapRGB(this->screen_texture_pixel_format, 0x55, 0x55, 0x55);
    gameboy_pocket_colors[2] = SDL_MapRGB(this->screen_texture_pixel_format, 0xAA, 0xAA, 0xAA);
    gameboy_pocket_colors[3] = SDL_MapRGB(this->screen_texture_pixel_format, 0xFF, 0xFF, 0xFF);
}

// ----------------------------------------------------------------------------

Ppu::~Ppu() {
    // nothing to do
}

// ----------------------------------------------------------------------------

void Ppu::clock() {
    if (scanline >= 0 && scanline < 144) {
        if (scanline_dot >= 0 && scanline_dot < 80) {
            // mode 2 - OAM scan
        } else if (scanline_dot >= 80 && scanline_dot < 240) {
            // mode 3 - drawing pixels

            int screen_pixel_x = scanline_dot - 80;
            int screen_pixel_y = scanline;
            int screen_pixel_index = screen_pixel_y * (screen_pixels_row_length / sizeof(Uint32)) + screen_pixel_x;

            // output garbage for now
            int random_color_index = std::rand() % COLORS_SIZE;
            screen_pixels[screen_pixel_index] = gameboy_pocket_colors[random_color_index];
        } else {
            // mode 0 - horizontal blank
        }
    } else {
        // mode 1 - vertical blank
    }

    frame_complete = false;
    scanline_dot++;
    if (scanline_dot >= 456) {
        scanline_dot = 0;

        scanline++;
        if (scanline > 153) {
            scanline = 0;
            frame_complete = true;
        }
    }
}

// ----------------------------------------------------------------------------

void Ppu::lock_screen_texture() {
    SDL_LockTexture(screen_texture, NULL, (void**)&screen_pixels, &screen_pixels_row_length);
}

// ----------------------------------------------------------------------------

void Ppu::unlock_screen_texture() {
    SDL_UnlockTexture(screen_texture);
}

// ----------------------------------------------------------------------------

bool Ppu::is_frame_complete() {
    return frame_complete;
}

#include "Ppu.h"

#include <SDL2/SDL.h>
#include <cstdlib>
#include <string>

Ppu::Ppu(SDL_Texture* screen_texture, SDL_PixelFormat* screen_texture_pixel_format)
: screen_texture(screen_texture),
  screen_texture_pixel_format(screen_texture_pixel_format),
  scanline(0),
  scanline_dot(0),
  frame_complete(false) {
    std::memset(video_ram, 0x00, VIDEO_RAM_SIZE * sizeof(Uint8));

    gameboy_pocket_colors[0] = SDL_MapRGB(this->screen_texture_pixel_format, 0xFF, 0xFF, 0xFF);
    gameboy_pocket_colors[1] = SDL_MapRGB(this->screen_texture_pixel_format, 0xAA, 0xAA, 0xAA);
    gameboy_pocket_colors[2] = SDL_MapRGB(this->screen_texture_pixel_format, 0x55, 0x55, 0x55);
    gameboy_pocket_colors[3] = SDL_MapRGB(this->screen_texture_pixel_format, 0x00, 0x00, 0x00);
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

// ----------------------------------------------------------------------------

Uint8 Ppu::cpu_read(Uint16 address) {
    if (address >= 0x8000 && address <= 0x9FFF) {
        return video_ram[address - 0x8000];
    }

    switch (address) {
        case 0xFF44:
            return scanline;
            break;
        default:
            return 0xFF;
            break;
    }
}

// ----------------------------------------------------------------------------

void Ppu::cpu_write(Uint16 address, Uint8 value) {
    if (address >= 0x8000 && address <= 0x9FFF) {
        video_ram[address - 0x8000] = value;
        return;
    }

    switch (address) {
        default:
            // ignore unspecified writes
            break;
    }
}

// ----------------------------------------------------------------------------

void Ppu::render_vram_to_texture(SDL_Texture* texture, int texture_width, int texture_height) {
    Uint32* texture_pixels;
    int texture_pixels_row_length;
    SDL_LockTexture(texture, NULL, (void**)&texture_pixels, &texture_pixels_row_length);

    int horizontal_tiles = texture_width / 8;
    int vertical_tiles = texture_height / 8;
    int vram_index = 0;
    for (int tile_y = 0; tile_y < vertical_tiles; tile_y++) {
        for (int tile_x = 0; tile_x < horizontal_tiles; tile_x++) {
            for (int pixel_y = 0; pixel_y < 8; pixel_y++) {
                Uint8 lo_byte = video_ram[vram_index];
                Uint8 hi_byte = video_ram[vram_index + 1];
                vram_index += 2;

                for (int bit = 7; bit >= 0; bit--) {
                    int pixel_x = 7 - bit;
                    int color_index = (((hi_byte >> bit) & 0x01) ? 0x02 : 0x00) | ((lo_byte >> bit) & 0x01);

                    int texture_pixel_x = tile_x * 8 + pixel_x;
                    int texture_pixel_y = tile_y * 8 + pixel_y;
                    int texture_pixel_index = texture_pixel_y * texture_width + texture_pixel_x;
                    texture_pixels[texture_pixel_index] = gameboy_pocket_colors[color_index];
                }
            }
        }
    }

    SDL_UnlockTexture(texture);
}

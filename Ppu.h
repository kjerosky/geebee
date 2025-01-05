#ifndef PPU_H
#define PPU_H

#include <SDL2/SDL.h>

#define COLORS_SIZE 4
#define VIDEO_RAM_SIZE 8192
#define TILE_DATA_SIZE 6144

class Ppu {

public:

    Ppu(SDL_Texture* screen_texture, SDL_PixelFormat* screen_texture_pixel_format);
    ~Ppu();

    void clock();
    void lock_screen_texture();
    void unlock_screen_texture();
    bool is_frame_complete();

    Uint8 cpu_read(Uint16 address);
    void cpu_write(Uint16 address, Uint8 value);
    void render_vram_to_texture(SDL_Texture* texture, int texture_width, int texture_height);
    void get_palette_colors(Uint32* output);

private:

    SDL_Texture* screen_texture;
    SDL_PixelFormat* screen_texture_pixel_format;

    Uint8 video_ram[VIDEO_RAM_SIZE];

    int scanline;
    int scanline_dot;
    bool frame_complete;

    Uint32* screen_pixels;
    int screen_pixels_row_length;

    Uint32 gameboy_pocket_colors[COLORS_SIZE];
    Uint8 bg_palette;
    Uint8 obj_palette_0;
    Uint8 obj_palette_1;

    Uint8 lcd_control;
    Uint8 lcd_status;
    Uint8 viewport_y;
    Uint8 viewport_x;
    Uint8 scanline_compare;
    Uint8 window_y;
    Uint8 window_x_plus_7;
};

#endif

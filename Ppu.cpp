#include "Ppu.h"

#include <SDL2/SDL.h>
#include <string>

Ppu::Ppu(SDL_Texture* screen_texture, SDL_PixelFormat* screen_texture_pixel_format)
: screen_texture(screen_texture),
  screen_texture_pixel_format(screen_texture_pixel_format),
  pixel_pipeline(video_ram, gameboy_pocket_colors, &bg_palette, &obj_palette_0, &obj_palette_1, &lcd_control),
  visible_obj_indices(10),
  scanline(0),
  scanline_dot(0),
  frame_complete(false),
  just_entering_mode_0(false),
  screen_pixel_x(-1),
  bg_palette(0x00),
  obj_palette_0(0x00),
  obj_palette_1(0x00) {

    std::memset(video_ram, 0x00, VIDEO_RAM_SIZE * sizeof(Uint8));
    std::memset(oam, 0x00, OAM_SIZE * sizeof(Uint8));

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

Uint8 Ppu::clock() {
    Uint8 interrupts_raised = 0x00;

    if (scanline == scanline_compare) {
        lcd_status |= 0x04;

        if (scanline_dot == 0 && (lcd_status & 0x40) != 0x00) {
            interrupts_raised |= 0x02;
        }
    } else {
        lcd_status &= 0xFB;
    }

    if (scanline >= 0 && scanline < 144) {
        if (scanline_dot >= 0 && scanline_dot < 80) {
            // mode 2 - OAM scan
            lcd_status = (lcd_status & 0xFC) | 0x02;

            if (scanline_dot == 0 && (lcd_status & 0x20) != 0x00) {
                interrupts_raised |= 0x02;
            }

            // For now, perform visible object search all at once at the start
            // of the scanline. Maybe move to cycle accuracy later?
            if (scanline_dot == 0) {
                Uint8 obj_height = ((lcd_control >> 2) & 0x01) == 0x00 ? 8 : 16;

                visible_obj_indices.clear();
                for (int i = 0; i < 40 && visible_obj_indices.size() < 10; i++) {
                    Uint8 obj_y = oam[i * 4];
                    if (obj_y == 0 || obj_y >= 160) {
                        continue;
                    }

                    Uint8 obj_lower_screen_y = obj_y - 16;
                    Uint8 obj_upper_screen_y = obj_y - 16 + obj_height;
                    if (scanline >= obj_lower_screen_y && scanline < obj_upper_screen_y) {
                        visible_obj_indices.push_back(i);
                    }
                }
            }
        } else if (screen_pixel_x >= 0 && screen_pixel_x < 160) {
            // mode 3 - drawing pixels
            lcd_status = (lcd_status & 0xFC) | 0x03;

            int screen_pixel_y = scanline;
            int screen_pixel_index = screen_pixel_y * (screen_pixels_row_length / sizeof(Uint32)) + screen_pixel_x;

            for (int i = 0; i < visible_obj_indices.size(); i++) {
                if (oam[visible_obj_indices[i] * 4 + 1] - 8 == screen_pixel_x) {
                    Uint8 obj_tile_id = oam[visible_obj_indices[i] * 4 + 2];
                    Uint8 obj_attributes = oam[visible_obj_indices[i] * 4 + 3];
                    Uint8 obj_y_offset = scanline - (oam[visible_obj_indices[i] * 4] - 16);

                    pixel_pipeline.load_obj_pixels(obj_tile_id, obj_attributes, obj_y_offset);
                }
            }

            pixel_pipeline.clock();
            if (pixel_pipeline.is_ready_with_next_pixel()) {
                Uint8 next_pixel_color_index = pixel_pipeline.get_next_pixel_color_index();
                screen_pixels[screen_pixel_index] = gameboy_pocket_colors[next_pixel_color_index];

                screen_pixel_x++;
                if (screen_pixel_x == 160) {
                    just_entering_mode_0 = true;
                }
            }
        } else {
            // mode 0 - horizontal blank
            lcd_status = lcd_status & 0xFC;

            if (just_entering_mode_0) {
                if ((lcd_status & 0x08) != 0x00) {
                    interrupts_raised |= 0x02;
                }

                just_entering_mode_0 = false;
            }
        }
    } else {
        // mode 1 - vertical blank
        lcd_status = (lcd_status & 0xFC) | 0x01;

        if (scanline == 144 && scanline_dot == 0) {
            interrupts_raised |= 0x01;

            if ((lcd_status & 0x08) != 0x00) {
                interrupts_raised |= 0x02;
            }
        }
    }

    frame_complete = false;
    scanline_dot++;
    if (scanline_dot == 80) {
        screen_pixel_x = 0;
        pixel_pipeline.reset(screen_pixel_x, scanline, viewport_x, viewport_y);
    } else if (scanline_dot >= 456) {
        scanline_dot = 0;

        scanline++;
        if (scanline > 153) {
            scanline = 0;
            frame_complete = true;
        }
    }

    return interrupts_raised;
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
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        return oam[address - 0xFE00];
    }

    switch (address) {
        case 0xFF40:
            return lcd_control;
            break;
        case 0xFF41:
            return lcd_status;
            break;
        case 0xFF42:
            return viewport_y;
            break;
        case 0xFF43:
            return viewport_x;
            break;
        case 0xFF44:
            return scanline;
            break;
        case 0xFF45:
            return scanline_compare;
            break;
        case 0xFF47:
            return bg_palette;
            break;
        case 0xFF48:
            return obj_palette_0;
            break;
        case 0xFF49:
            return obj_palette_1;
            break;
        case 0xFF4A:
            return window_y;
            break;
        case 0xFF4B:
            return window_x_plus_7;
            break;
    }

    return 0xFF;
}

// ----------------------------------------------------------------------------

void Ppu::cpu_write(Uint16 address, Uint8 value) {
    if (address >= 0x8000 && address <= 0x9FFF) {
        video_ram[address - 0x8000] = value;
        return;
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        oam[address - 0xFE00] = value;
        return;
    }

    switch (address) {
        case 0xFF40:
            lcd_control = value;
            break;
        case 0xFF41:
            lcd_status = value;
            break;
        case 0xFF42:
            viewport_y = value;
            break;
        case 0xFF43:
            viewport_x = value;
            break;
        case 0xFF45:
            scanline_compare = value;
            break;
        case 0xFF47:
            bg_palette = value;
            break;
        case 0xFF48:
            obj_palette_0 = value;
            break;
        case 0xFF49:
            obj_palette_1 = value;
            break;
        case 0xFF4A:
            window_y = value;
            break;
        case 0xFF4B:
            window_x_plus_7 = value;
            break;
        default:
            // ignore unspecified writes
            break;
    }
}

// ----------------------------------------------------------------------------

void Ppu::render_tiles_to_texture(SDL_Texture* texture, int texture_width, int texture_height) {
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
                    int color_index = (((hi_byte >> bit) & 0x01) ? 0x02 : 0x00) | ((lo_byte >> bit) & 0x01);
                    int mapped_bg_color_index = (bg_palette >> (color_index * 2)) & 0x03;

                    int pixel_x = 7 - bit;
                    int texture_pixel_x = tile_x * 8 + pixel_x;
                    int texture_pixel_y = tile_y * 8 + pixel_y;
                    int texture_pixel_index = texture_pixel_y * texture_width + texture_pixel_x;
                    texture_pixels[texture_pixel_index] = gameboy_pocket_colors[mapped_bg_color_index];
                }
            }
        }
    }

    SDL_UnlockTexture(texture);
}

// ----------------------------------------------------------------------------

void Ppu::get_palette_colors(Uint32* output) {
    output[0] = gameboy_pocket_colors[bg_palette & 0x03];
    output[1] = gameboy_pocket_colors[(bg_palette >> 2) & 0x03];
    output[2] = gameboy_pocket_colors[(bg_palette >> 4) & 0x03];
    output[3] = gameboy_pocket_colors[(bg_palette >> 6) & 0x03];
    output[4] = gameboy_pocket_colors[obj_palette_0 & 0x03];
    output[5] = gameboy_pocket_colors[(obj_palette_0 >> 2) & 0x03];
    output[6] = gameboy_pocket_colors[(obj_palette_0 >> 4) & 0x03];
    output[7] = gameboy_pocket_colors[(obj_palette_0 >> 6) & 0x03];
    output[8] = gameboy_pocket_colors[obj_palette_1 & 0x03];
    output[9] = gameboy_pocket_colors[(obj_palette_1 >> 2) & 0x03];
    output[10] = gameboy_pocket_colors[(obj_palette_1 >> 4) & 0x03];
    output[11] = gameboy_pocket_colors[(obj_palette_1 >> 6) & 0x03];
}

// ----------------------------------------------------------------------------

void Ppu::render_tile_map(SDL_Renderer* renderer, int tile_map_index, SDL_Texture* tiles_texture, int tiles_texture_width, bool show_objects) {
    int horizontal_tiles_count = tiles_texture_width / 8;

    SDL_Rect tile_rect;
    tile_rect.x = 0;
    tile_rect.y = 0;
    tile_rect.w = 8;
    tile_rect.h = 8;

    SDL_Rect map_tile_rect;
    map_tile_rect.x = 0;
    map_tile_rect.y = 0;
    map_tile_rect.w = 8;
    map_tile_rect.h = 8;

    bool bg_and_obj_share_same_memory = ((lcd_control >> 4) & 0x01) == 0x01;
    Uint16 base_tile_map_address = tile_map_index == 0 ? 0x1800 : 0x1C00;

    for (int map_tile_y = 0; map_tile_y < 32; map_tile_y++) {
        for (int map_tile_x = 0; map_tile_x < 32; map_tile_x++) {
            map_tile_rect.x = map_tile_x * 8;
            map_tile_rect.y = map_tile_y * 8;

            int tile_map_offset = map_tile_y * 32 + map_tile_x;
            Uint8 map_tile_id = video_ram[base_tile_map_address + tile_map_offset];

            Uint16 tile_index = bg_and_obj_share_same_memory ? map_tile_id : 0x100 + static_cast<Sint8>(map_tile_id);

            tile_rect.x = (tile_index % horizontal_tiles_count) * 8;
            tile_rect.y = (tile_index / horizontal_tiles_count) * 8;
            SDL_RenderCopy(renderer, tiles_texture, &tile_rect, &map_tile_rect);
        }
    }

    if (!show_objects) {
        return;
    }

    // temporary bad sprite display for debugging
    for (int i = 0; i < 160; i += 4) {
        int obj_y = (int)oam[i] - 16;
        int obj_x = (int)oam[i + 1] - 8;
        if (obj_x == 0 || obj_y == 0) {
            continue;
        }

        int obj_tile_index = oam[i + 2];
        tile_rect.x = (obj_tile_index % horizontal_tiles_count) * 8;
        tile_rect.y = (obj_tile_index / horizontal_tiles_count) * 8;

        map_tile_rect.x = obj_x;
        map_tile_rect.y = obj_y;

        SDL_RenderCopy(renderer, tiles_texture, &tile_rect, &map_tile_rect);
    }
}

// ----------------------------------------------------------------------------

Uint8* Ppu::get_oam() {
    return oam;
}

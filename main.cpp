#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

#include "GameBoy.h"

std::string hex(unsigned int number, int width) {
    std::string s(width, '0');
    for (int i = width - 1; i >= 0; i--, number >>= 4) {
        s[i] = "0123456789ABCDEF"[number & 0xF];
    }

    return s;
}

// ----------------------------------------------------------------------------

void draw_string(SDL_Renderer* renderer, SDL_Texture* font_texture, int text_row, int text_column, int scale, std::string str) {
    SDL_Rect font_source;
    font_source.x = 0;
    font_source.y = 0;
    font_source.w = 8;
    font_source.h = 8;

    SDL_Rect font_destination;
    font_destination.w = font_source.w * scale;
    font_destination.h = font_source.h * scale;
    font_destination.x = 0;
    font_destination.y = text_row * font_destination.h;

    for (int i = 0; i < str.size(); i++) {
        int char_index = (int)str[i];

        font_source.x = (char_index % 16) * font_source.w;
        font_source.y = (char_index / 16) * font_source.h;

        font_destination.x = font_destination.w * (text_column + i);
        SDL_RenderCopy(renderer, font_texture, &font_source, &font_destination);
    }
}

// ----------------------------------------------------------------------------

void draw_ram_page(SDL_Renderer* renderer, SDL_Texture* font_texture, int text_row, int text_column, int scale, Uint16 ram_page, Uint8* ram_contents) {
    draw_string(renderer, font_texture, text_row++, text_column, 2, "      |  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
    draw_string(renderer, font_texture, text_row++, text_column, 2, "------+------------------------------------------------");

    for (Uint16 row = 0x0000; row <= 0x00F0; row += 0x0010) {
        std::string ram_row_contents = "$" + hex(ram_page + row, 4) + " |";

        for (Uint16 column = 0x0000; column <= 0x000F; column++) {
            ram_row_contents += " " + hex(ram_contents[ram_page + row + column], 2);
        }

        draw_string(renderer, font_texture, text_row++, text_column, 2, ram_row_contents);
    }
}

void draw_cpu_info(SDL_Renderer* renderer, SDL_Texture* font_texture, int text_row, int text_column, int scale, Cpu_Info cpu_info) {
    draw_string(renderer, font_texture, text_row, text_column, 2, "STATUS: ");
    SDL_SetTextureColorMod(font_texture, cpu_info.z_flag ? 0x00 : 0xFF, cpu_info.z_flag ? 0xFF : 0x00, 0x00);
    draw_string(renderer, font_texture, text_row, text_column + 8, 2, "Z");
    SDL_SetTextureColorMod(font_texture, cpu_info.n_flag ? 0x00 : 0xFF, cpu_info.n_flag ? 0xFF : 0x00, 0x00);
    draw_string(renderer, font_texture, text_row, text_column + 10, 2, "N");
    SDL_SetTextureColorMod(font_texture, cpu_info.h_flag ? 0x00 : 0xFF, cpu_info.h_flag ? 0xFF : 0x00, 0x00);
    draw_string(renderer, font_texture, text_row, text_column + 12, 2, "H");
    SDL_SetTextureColorMod(font_texture, cpu_info.c_flag ? 0x00 : 0xFF, cpu_info.c_flag ? 0xFF : 0x00, 0x00);
    draw_string(renderer, font_texture, text_row, text_column + 14, 2, "C");
    SDL_SetTextureColorMod(font_texture, 0xFF, 0xFF, 0xFF);

    draw_string(renderer, font_texture, text_row + 2, text_column, 2, "A: $" + hex(cpu_info.a, 2));
    draw_string(renderer, font_texture, text_row + 3, text_column, 2, "B: $" + hex(cpu_info.b, 2) + "    C: $" + hex(cpu_info.c, 2));
    draw_string(renderer, font_texture, text_row + 4, text_column, 2, "D: $" + hex(cpu_info.d, 2) + "    E: $" + hex(cpu_info.e, 2));
    draw_string(renderer, font_texture, text_row + 5, text_column, 2, "H: $" + hex(cpu_info.h, 2) + "    L: $" + hex(cpu_info.l, 2));
    draw_string(renderer, font_texture, text_row + 7, text_column, 2, "PC: $" + hex(cpu_info.pc, 4));
    draw_string(renderer, font_texture, text_row + 8, text_column, 2, "SP: $" + hex(cpu_info.sp, 4));
}

// ----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    const int GAMEBOY_SCREEN_WIDTH = 160;
    const int GAMEBOY_SCREEN_HEIGHT = 144;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "ERROR: SDL could not be created!  SDL error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("geebee", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAMEBOY_SCREEN_WIDTH, GAMEBOY_SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
    if (window == NULL) {
        std::cerr << "ERROR: Window could not be created!  SDL error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        std::cerr << "ERROR: Renderer could not be created!  SDL error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return 1;
    }

    SDL_Texture* font_texture = IMG_LoadTexture(renderer, "8x8-pixel-font.png");
    if (font_texture == NULL) {
        std::cerr << "ERROR: Texture font could not be loaded!" << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return 1;
    }

    GameBoy game_boy;
    Uint8* ram_contents = game_boy.get_ram_contents();
    Uint16 initial_pc = game_boy.get_cpu_info().pc;
    Uint16 ram_page = initial_pc & 0xFF00;

    // setup ram contents for testing
    std::stringstream ss;
    ss << "31 00 00 33   31 FF FF 33   31 AA 00 33";

    uint16_t nOffset = initial_pc;
    while (!ss.eof())
    {
        std::string b;
        ss >> b;
        ram_contents[nOffset++] = (Uint8)std::stoul(b, nullptr, 16);
    }

    bool is_running = true;
    while (is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_running = false;
                } else if (event.key.keysym.sym == SDLK_SPACE) {
                    game_boy.execute_next_instruction();
                } else if (event.key.keysym.sym == SDLK_EQUALS) {
                    ram_page = ram_page + 0x0100;
                } else if (event.key.keysym.sym == SDLK_MINUS) {
                    ram_page = ram_page - 0x0100;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x44, 0xCC, 0x00);
        SDL_RenderClear(renderer);

        draw_ram_page(renderer, font_texture, 1, 1, 2, ram_page, ram_contents);
        draw_cpu_info(renderer, font_texture, 22, 1, 2, game_boy.get_cpu_info());

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(font_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}


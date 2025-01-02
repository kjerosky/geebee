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

void draw_ram_page(SDL_Renderer* renderer, SDL_Texture* font_texture, int text_row, int text_column, int scale, Uint16 ram_page, Uint8* ram_contents, Uint16 current_pc) {
    draw_string(renderer, font_texture, text_row++, text_column, 2, "      |  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
    draw_string(renderer, font_texture, text_row++, text_column, 2, "------+------------------------------------------------");

    for (Uint16 row = 0x0000; row <= 0x00F0; row += 0x0010) {
        std::string ram_row_contents = "$" + hex(ram_page + row, 4) + " |";
        std::string highlighted_ram_row_contents = "       ";

        for (Uint16 column = 0x0000; column <= 0x000F; column++) {
            ram_row_contents += " " + hex(ram_contents[ram_page + row + column], 2);

            if ((current_pc & 0xFFF0) == (ram_page | row)) {
                highlighted_ram_row_contents += " ";
                if ((current_pc & 0x000F) == column) {
                    highlighted_ram_row_contents += hex(ram_contents[ram_page + row + column], 2);
                } else {
                    highlighted_ram_row_contents += "  ";
                }
            }
        }

        draw_string(renderer, font_texture, text_row, text_column, 2, ram_row_contents);
        if ((current_pc & 0xFFF0) == (ram_page | row)) {
            SDL_SetTextureColorMod(font_texture, 0x00, 0xFF, 0x00);
            draw_string(renderer, font_texture, text_row, text_column, 2, highlighted_ram_row_contents);
            SDL_SetTextureColorMod(font_texture, 0xFF, 0xFF, 0xFF);
        }

        text_row++;
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

    draw_string(renderer, font_texture, text_row + 10, text_column, 2, "IME: " + std::string(cpu_info.ime ? "1" : "0"));
}

// ----------------------------------------------------------------------------

void fill_ram_at_location(Uint8* ram, Uint16 location, std::stringstream& ss) {
    while (!ss.eof())
    {
        std::string b;
        ss >> b;
        ram[location++] = (Uint8)std::stoul(b, nullptr, 16);
    }

    ss.str("");
    ss.clear();
}

// ----------------------------------------------------------------------------

void setup_ram_for_testing(Uint8* ram, Uint16 initial_pc) {
    std::stringstream ss;

    ss << "3E 00   D9";
    fill_ram_at_location(ram, 0x0040, ss);

    ss << "3E 02   D9";
    fill_ram_at_location(ram, 0x0048, ss);

    ss << "3E 04   D9";
    fill_ram_at_location(ram, 0x0050, ss);

    ss << "3E 06   D9";
    fill_ram_at_location(ram, 0x0058, ss);

    ss << "3E 08   D9";
    fill_ram_at_location(ram, 0x0060, ss);

    ss << "00   C3 50 01";
    fill_ram_at_location(ram, initial_pc, ss);

    ss << "FB   21 FF FF   06 05   70   3E 78   76   3C   18 FE";
    fill_ram_at_location(ram, 0x0150, ss);
}

// ----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    const int GAMEBOY_SCREEN_WIDTH = 160;
    const int GAMEBOY_SCREEN_HEIGHT = 144;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "ERROR: SDL could not be created!  SDL error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("geebee", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAMEBOY_SCREEN_WIDTH, GAMEBOY_SCREEN_HEIGHT, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
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

    setup_ram_for_testing(ram_contents, initial_pc);

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
                    ram_page = game_boy.get_cpu_info().pc & 0xFF00;
                } else if (event.key.keysym.sym == SDLK_EQUALS) {
                    ram_page = ram_page + 0x0100;
                } else if (event.key.keysym.sym == SDLK_MINUS) {
                    ram_page = ram_page - 0x0100;
                } else if (event.key.keysym.sym == SDLK_1) {
                    game_boy.generate_interrupt(0);
                } else if (event.key.keysym.sym == SDLK_2) {
                    game_boy.generate_interrupt(1);
                } else if (event.key.keysym.sym == SDLK_3) {
                    game_boy.generate_interrupt(2);
                } else if (event.key.keysym.sym == SDLK_4) {
                    game_boy.generate_interrupt(3);
                } else if (event.key.keysym.sym == SDLK_5) {
                    game_boy.generate_interrupt(4);
                }
            }
        }

        Cpu_Info current_cpu_info = game_boy.get_cpu_info();

        SDL_SetRenderDrawColor(renderer, 0x00, 0x44, 0xCC, 0x00);
        SDL_RenderClear(renderer);

        draw_ram_page(renderer, font_texture, 1, 1, 2, ram_page, ram_contents, current_cpu_info.pc);
        draw_cpu_info(renderer, font_texture, 22, 1, 2, current_cpu_info);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(font_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

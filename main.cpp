#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cstdlib>

#include "GameBoy.h"
#include "Cartridge.h"

enum View {
    CPU_AND_RAM = 0,
    CPU_AND_SCREEN,
    SCREEN_ONLY,
    NUMBER_OF_ENTRIES,
};

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

void draw_ram_page(SDL_Renderer* renderer, SDL_Texture* font_texture, int text_row, int text_column, int scale, Uint16 ram_page, Uint8* ram_page_contents, Uint16 current_pc) {
    draw_string(renderer, font_texture, text_row++, text_column, 2, "      |  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
    draw_string(renderer, font_texture, text_row++, text_column, 2, "------+------------------------------------------------");

    for (Uint16 row = 0x0000; row <= 0x00F0; row += 0x0010) {
        std::string ram_row_contents = "$" + hex(ram_page + row, 4) + " |";
        std::string highlighted_ram_row_contents = "       ";

        for (Uint16 column = 0x0000; column <= 0x000F; column++) {
            ram_row_contents += " " + hex(ram_page_contents[row + column], 2);

            if ((current_pc & 0xFFF0) == (ram_page | row)) {
                highlighted_ram_row_contents += " ";
                if ((current_pc & 0x000F) == column) {
                    highlighted_ram_row_contents += hex(ram_page_contents[row + column], 2);
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

void fill_ram_at_location(GameBoy& game_boy, Uint16 location, std::stringstream& ss) {
    while (!ss.eof())
    {
        std::string b;
        ss >> b;
        game_boy.write_to_bus(location++, (Uint8)std::stoul(b, nullptr, 16));
    }

    ss.str("");
    ss.clear();
}

// ----------------------------------------------------------------------------

void setup_ram_for_testing(GameBoy& game_boy, Uint16 initial_pc) {
    std::stringstream ss;

    ss << "3E 00   D9";
    fill_ram_at_location(game_boy, 0x0040, ss);

    ss << "3E 02   D9";
    fill_ram_at_location(game_boy, 0x0048, ss);

    ss << "3E 04   D9";
    fill_ram_at_location(game_boy, 0x0050, ss);

    ss << "3E 06   D9";
    fill_ram_at_location(game_boy, 0x0058, ss);

    ss << "3E 08   D9";
    fill_ram_at_location(game_boy, 0x0060, ss);

    ss << "00   C3 50 01";
    fill_ram_at_location(game_boy, initial_pc, ss);

    ss << "FB   21 FF FF   06 05   70   3E 78   76   3C   18 FE";
    fill_ram_at_location(game_boy, 0x0150, ss);
}

// ----------------------------------------------------------------------------

void refresh_ram_page_contents(GameBoy& game_boy, Uint8* ram_page_contents, Uint16 ram_page) {
    for (Uint16 i = 0; i < 256; i++) {
        ram_page_contents[i] = game_boy.read_from_bus(ram_page + i);
    }
}

// ----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " rom_file" << std::endl;
        return 1;
    }

    std::string rom_filename = argv[1];

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

    SDL_Texture* screen_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, GAMEBOY_SCREEN_WIDTH, GAMEBOY_SCREEN_HEIGHT);
    if (screen_texture == NULL) {
        std::cerr << "ERROR: Screen texture could not be created!  SDL error: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(font_texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }

    Uint32 texture_format;
    int screen_texture_width;
    int screen_texture_height;
    SDL_QueryTexture(screen_texture, &texture_format, NULL, &screen_texture_width, &screen_texture_height);

    SDL_PixelFormat* screen_texture_pixel_format = SDL_AllocFormat(texture_format);
    if (screen_texture_pixel_format == NULL) {
        std::cerr<< "ERROR: Screen texture pixel format could not be allocated!  SDL error: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(screen_texture);
        SDL_DestroyTexture(font_texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return 1;
    }

    Cartridge cartridge(rom_filename);
    GameBoy game_boy(&cartridge, screen_texture, screen_texture_pixel_format);
    Uint8 ram_page_contents[256];
    Uint16 initial_pc = game_boy.get_cpu_info().pc;
    Uint16 ram_page = initial_pc & 0xFF00;

    // setup_ram_for_testing(game_boy, initial_pc);

    int view = (int)View::CPU_AND_SCREEN;
    bool is_continuously_running = false;
    Uint64 current_counter;
    Uint64 previous_counter;
    double residual_time;

    bool is_main_program_running = true;
    while (is_main_program_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                is_main_program_running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_main_program_running = false;
                } else if (event.key.keysym.sym == SDLK_TAB) {
                    view = (view + 1) % View::NUMBER_OF_ENTRIES;
                } else if (event.key.keysym.sym == SDLK_SPACE && !is_continuously_running) {
                    game_boy.execute_next_instruction();
                    ram_page = game_boy.get_cpu_info().pc & 0xFF00;
                } else if (event.key.keysym.sym == SDLK_RETURN) {
                    is_continuously_running = !is_continuously_running;
                    current_counter = SDL_GetPerformanceCounter();
                    previous_counter = current_counter;
                    residual_time = 0;
                } else if (event.key.keysym.sym == SDLK_f && !is_continuously_running) {
                    game_boy.complete_frame();
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

        if (is_continuously_running) {
            ram_page = game_boy.get_cpu_info().pc & 0xFF00;

            previous_counter = current_counter;
            current_counter = SDL_GetPerformanceCounter();
            double delta_time = static_cast<double>(current_counter - previous_counter) / static_cast<double>(SDL_GetPerformanceFrequency());

            if (residual_time > 0.0) {
                residual_time -= delta_time;
            } else {
                residual_time += (1.0 / 60.0) - delta_time;
                game_boy.complete_frame();
            }
        }

        Cpu_Info current_cpu_info = game_boy.get_cpu_info();

        if (view == View::CPU_AND_RAM) {
            SDL_SetRenderDrawColor(renderer, 0x00, 0x44, 0xCC, 0x00);
            SDL_RenderClear(renderer);

            refresh_ram_page_contents(game_boy, ram_page_contents, ram_page);

            draw_ram_page(renderer, font_texture, 1, 1, 2, ram_page, ram_page_contents, current_cpu_info.pc);
            draw_cpu_info(renderer, font_texture, 22, 1, 2, current_cpu_info);
        } else if (view == View::CPU_AND_SCREEN) {
            SDL_SetRenderDrawColor(renderer, 0x00, 0x44, 0xCC, 0x00);
            SDL_RenderClear(renderer);

            draw_cpu_info(renderer, font_texture, 1, 43, 2, current_cpu_info);

            SDL_Rect screen_destination_rect;
            screen_destination_rect.x = 0;
            screen_destination_rect.y = 0;
            screen_destination_rect.w = GAMEBOY_SCREEN_WIDTH * 4;
            screen_destination_rect.h = GAMEBOY_SCREEN_HEIGHT * 4;
            SDL_RenderCopy(renderer, screen_texture, NULL, &screen_destination_rect);
        } else if (view == View::SCREEN_ONLY) {
            int window_width;
            int window_height;
            SDL_GetWindowSize(window, &window_width, &window_height);

            // Make sure the screen texture fills the window, but maintains
            // its original width/height ratio.
            float texture_width_to_height_ratio = (float)screen_texture_width / (float)screen_texture_height;
            SDL_Rect ratio_maintained_maximized_texture_rect;
            if (window_width - screen_texture_width >= window_height - screen_texture_height) {
                int new_width = texture_width_to_height_ratio * window_height;
                ratio_maintained_maximized_texture_rect.x = (window_width - new_width) / 2;
                ratio_maintained_maximized_texture_rect.y = 0;
                ratio_maintained_maximized_texture_rect.w = new_width;
                ratio_maintained_maximized_texture_rect.h = window_height;
            } else {
                int new_height = window_width / texture_width_to_height_ratio;
                ratio_maintained_maximized_texture_rect.x = 0;
                ratio_maintained_maximized_texture_rect.y = (window_height - new_height) / 2;
                ratio_maintained_maximized_texture_rect.w = window_width;
                ratio_maintained_maximized_texture_rect.h = new_height;
            }

            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, screen_texture, NULL, &ratio_maintained_maximized_texture_rect);
            SDL_RenderPresent(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_FreeFormat(screen_texture_pixel_format);
    SDL_DestroyTexture(screen_texture);
    SDL_DestroyTexture(font_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

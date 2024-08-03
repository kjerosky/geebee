#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <string>


void draw_string(SDL_Renderer* renderer, SDL_Texture* font_texture, int x, int y, int scale, std::string str) {
    SDL_Rect font_source;
    font_source.x = 0;
    font_source.y = 0;
    font_source.w = 8;
    font_source.h = 8;

    SDL_Rect font_destination;
    font_destination.x = 0;
    font_destination.y = y;
    font_destination.w = font_source.w * scale;
    font_destination.h = font_source.h * scale;

    for (int i = 0; i < str.size(); i++) {
        int char_index = (int)str[i];

        font_source.x = (char_index % 16) * 8;
        font_source.y = (char_index / 16) * 8;

        font_destination.x = x + font_destination.w * i;

        SDL_RenderCopy(renderer, font_texture, &font_source, &font_destination);
    }
}


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

    bool is_running = true;
    while (is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        draw_string(renderer, font_texture, 0,  0 * 16, 2, "      |  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
        draw_string(renderer, font_texture, 0,  1 * 16, 2, "------+------------------------------------------------");
        draw_string(renderer, font_texture, 0,  2 * 16, 2, "$0000 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
        draw_string(renderer, font_texture, 0,  3 * 16, 2, "$0010 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
        draw_string(renderer, font_texture, 0,  4 * 16, 2, "$0020 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
        draw_string(renderer, font_texture, 0,  5 * 16, 2, "$0030 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
        draw_string(renderer, font_texture, 0,  6 * 16, 2, "$0040 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
        draw_string(renderer, font_texture, 0,  7 * 16, 2, "$0050 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
        draw_string(renderer, font_texture, 0,  8 * 16, 2, "$0060 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
        draw_string(renderer, font_texture, 0,  9 * 16, 2, "$0070 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
        draw_string(renderer, font_texture, 0, 10 * 16, 2, "$0080 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
        draw_string(renderer, font_texture, 0, 11 * 16, 2, "$0090 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
        draw_string(renderer, font_texture, 0, 12 * 16, 2, "$00A0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
        draw_string(renderer, font_texture, 0, 13 * 16, 2, "$00B0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
        draw_string(renderer, font_texture, 0, 14 * 16, 2, "$00C0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
        draw_string(renderer, font_texture, 0, 15 * 16, 2, "$00D0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
        draw_string(renderer, font_texture, 0, 16 * 16, 2, "$00E0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
        draw_string(renderer, font_texture, 0, 17 * 16, 2, "$00F0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");

        draw_string(renderer, font_texture, 0, 19 * 16, 2, "STATUS: N V - B D I Z C");
        draw_string(renderer, font_texture, 0, 20 * 16, 2, "PC: $0000");
        draw_string(renderer, font_texture, 0, 21 * 16, 2, "SP: $00");
        draw_string(renderer, font_texture, 0, 22 * 16, 2, "A: $00 [0]");
        draw_string(renderer, font_texture, 0, 23 * 16, 2, "X: $00 [0]");
        draw_string(renderer, font_texture, 0, 24 * 16, 2, "Y: $00 [0]");

        SDL_SetTextureColorMod(font_texture, 0xFF, 0x00, 0x00);
        draw_string(renderer, font_texture,  8 * 16, 19 * 16, 2, "N");
        draw_string(renderer, font_texture, 10 * 16, 19 * 16, 2, "V");
        draw_string(renderer, font_texture, 14 * 16, 19 * 16, 2, "B");
        draw_string(renderer, font_texture, 16 * 16, 19 * 16, 2, "D");
        draw_string(renderer, font_texture, 18 * 16, 19 * 16, 2, "I");
        draw_string(renderer, font_texture, 22 * 16, 19 * 16, 2, "C");
        SDL_SetTextureColorMod(font_texture, 0x80, 0x80, 0x80);
        draw_string(renderer, font_texture, 12 * 16, 19 * 16, 2, "-");
        SDL_SetTextureColorMod(font_texture, 0x00, 0xFF, 0x00);
        draw_string(renderer, font_texture, 20 * 16, 19 * 16, 2, "Z");
        SDL_SetTextureColorMod(font_texture, 0xFF, 0xFF, 0xFF);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(font_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}


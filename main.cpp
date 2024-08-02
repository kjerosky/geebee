#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    const int GAMEBOY_SCREEN_WIDTH = 160;
    const int GAMEBOY_SCREEN_HEIGHT = 144;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "ERROR: SDL could not be created!  SDL error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("geebee", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAMEBOY_SCREEN_WIDTH, GAMEBOY_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
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

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, GAMEBOY_SCREEN_WIDTH, GAMEBOY_SCREEN_HEIGHT);
    if (texture == NULL) {
        std::cerr << "ERROR: Texture could not be created!  SDL error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }

    Uint32 texture_format;
    int texture_width;
    int texture_height;
    SDL_QueryTexture(texture, &texture_format, NULL, &texture_width, &texture_height);

    SDL_PixelFormat* texture_pixel_format = SDL_AllocFormat(texture_format);
    if (texture_pixel_format == NULL) {
        std::cerr<< "ERROR: Texture pixel format could not be allocated!  SDL error: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(texture);
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

        Uint32 colors[] = {
            SDL_MapRGB(texture_pixel_format, 0xFF, 0xFF, 0xFF),
            SDL_MapRGB(texture_pixel_format, 0xFF, 0x00, 0x00),
            SDL_MapRGB(texture_pixel_format, 0xFF, 0x80, 0x00),
            SDL_MapRGB(texture_pixel_format, 0xFF, 0xFF, 0x00),
            SDL_MapRGB(texture_pixel_format, 0x00, 0xFF, 0x00),
            SDL_MapRGB(texture_pixel_format, 0x00, 0x00, 0xFF),
            SDL_MapRGB(texture_pixel_format, 0x80, 0x00, 0xFF),
            SDL_MapRGB(texture_pixel_format, 0x00, 0x00, 0x00),
        };
        int colors_size = sizeof(colors) / sizeof(Uint32);
        Uint32 white_color = colors[0];

        Uint32* pixels;
        int pixels_row_length;
        SDL_LockTexture(texture, NULL, (void**)&pixels, &pixels_row_length);
        for (int y = 0; y < texture_height; y++) {
            for (int x = 0; x < texture_width; x++) {
                Uint32 pixel_index = y * (pixels_row_length / sizeof(Uint32)) + x;
                Uint32 pixel_color = colors[rand() % colors_size];
                if (x == 0 || y == 0 || x == texture_width - 1 || y == texture_height - 1) {
                    pixel_color = white_color;
                }

                pixels[pixel_index] = pixel_color;
            }
        }
        SDL_UnlockTexture(texture);

        int window_width;
        int window_height;
        SDL_GetWindowSize(window, &window_width, &window_height);

        // make sure the texture fills the window, but maintains its original width/height ratio
        float texture_width_to_height_ratio = (float)texture_width / (float)texture_height;
        SDL_Rect ratio_maintained_maximized_texture_rect;
        if (window_width - texture_width >= window_height - texture_height) {
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
        SDL_RenderCopy(renderer, texture, NULL, &ratio_maintained_maximized_texture_rect);
        SDL_RenderPresent(renderer);
    }

    SDL_FreeFormat(texture_pixel_format);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}


#ifndef GAME_BOY_H
#define GAME_BOY_H

#include "Cpu.h"

class Cpu;
class Bus;
class Cartridge;
struct SDL_Texture;
struct SDL_PixelFormat;
class Ppu;

class GameBoy {

public:

    GameBoy(Cartridge* cartridge, SDL_Texture* screen_texture, SDL_PixelFormat* screen_texture_pixel_format);
    ~GameBoy();

    void execute_next_instruction();
    void complete_frame();
    Cpu_Info get_cpu_info();
    void disassemble(Uint16 start_address, int num_of_instructions, std::vector<std::string>& output);
    void render_tiles_to_texture(SDL_Texture* texture, int texture_width, int texture_height);
    void get_palette_colors(Uint32* output);
    void render_tile_map(SDL_Renderer* renderer, int tile_map_index, SDL_Texture* tiles_texture, int tiles_texture_width, bool show_objects);
    Uint8* get_oam();

    void override_cpu_state(Cpu_Info& new_cpu_state);
    void generate_interrupt(int interrupt_bit);
    Uint8 read_from_bus(Uint16 address);
    void write_to_bus(Uint16 address, Uint8 value);

private:

    Cpu* cpu;
    Bus* bus;
    Ppu* ppu;

    Uint8 cycle_count;

    void clock();
};

#endif

#include "GameBoy.h"
#include "Bus.h"
#include "Cpu.h"
#include "Cartridge.h"
#include "Ppu.h"

GameBoy::GameBoy(Cartridge* cartridge, SDL_Texture* screen_texture, SDL_PixelFormat* screen_texture_pixel_format) {
    ppu = new Ppu(screen_texture, screen_texture_pixel_format);
    bus = new Bus(ppu, cartridge);
    cpu = new Cpu(bus);

    cycle_count = 0;
}

// ----------------------------------------------------------------------------

GameBoy::~GameBoy() {
    delete ppu;
    delete cpu;
    delete bus;
}

// ----------------------------------------------------------------------------

void GameBoy::execute_next_instruction() {
    ppu->lock_screen_texture();

    do {
        clock();
    } while (!cpu->is_current_instruction_finished());

    // Drain any remaining ppu clock cycles so that the next instruction will
    // be executed immediately.
    while (cycle_count % 4 != 0) {
        clock();
    }

    ppu->unlock_screen_texture();
}

// ----------------------------------------------------------------------------

void GameBoy::complete_frame() {
    ppu->lock_screen_texture();

    do {
        clock();
    } while (!ppu->is_frame_complete());

    // Drain any remaining ppu clock cycles so that the next instruction will
    // be executed immediately. This is okay to do, as any extra executed ppu
    // clock cycles will happen during the oam scan section of the topmost
    // scanline, which doesn't output pixels.
    while (cycle_count % 4 != 0) {
        clock();
    }

    ppu->unlock_screen_texture();
}

// ----------------------------------------------------------------------------

Cpu_Info GameBoy::get_cpu_info() {
    return cpu->get_cpu_info();
}

// ----------------------------------------------------------------------------

void GameBoy::override_cpu_state(Cpu_Info& new_cpu_state) {
    cpu->override_cpu_state(new_cpu_state);
}

// ----------------------------------------------------------------------------

void GameBoy::generate_interrupt(int interrupt_bit) {
    if (interrupt_bit < 0 || interrupt_bit > 4) {
        return;
    }

    bus->cpu_write(0xFF0F, bus->cpu_read(0xFF0F) | (0x01 << interrupt_bit));
}

// ----------------------------------------------------------------------------

Uint8 GameBoy::read_from_bus(Uint16 address) {
    return bus->cpu_read(address);
}

// ----------------------------------------------------------------------------

void GameBoy::write_to_bus(Uint16 address, Uint8 value) {
    bus->cpu_write(address, value);
}

// ----------------------------------------------------------------------------

void GameBoy::clock() {
    Uint8 ppu_interrupts_raised = ppu->clock();
    if (ppu_interrupts_raised != 0x00) {
        Uint8 new_if_flag_register_value = bus->cpu_read(0xFF0F) | ppu_interrupts_raised;
        bus->cpu_write(0xFF0F, new_if_flag_register_value);
    }

    if (cycle_count % 4 == 0) {
        cpu->clock();
    }

    cycle_count++;
    if (cycle_count == 0) {
        bus->increment_div_register();
    }
}

// ----------------------------------------------------------------------------

void GameBoy::disassemble(Uint16 start_address, int num_of_instructions, std::vector<std::string>& output) {
    cpu->disassemble(start_address, num_of_instructions, output);
}

// ----------------------------------------------------------------------------

void GameBoy::render_tiles_to_texture(SDL_Texture* texture, int texture_width, int texture_height) {
    ppu->render_tiles_to_texture(texture, texture_width, texture_height);
}

// ----------------------------------------------------------------------------

void GameBoy::get_palette_colors(Uint32* output) {
    ppu->get_palette_colors(output);
}

// ----------------------------------------------------------------------------

void GameBoy::render_tile_map(SDL_Renderer* renderer, int tile_map_index, SDL_Texture* tiles_texture, int tiles_texture_width, bool show_objects) {
    ppu->render_tile_map(renderer, tile_map_index, tiles_texture, tiles_texture_width, show_objects);
}

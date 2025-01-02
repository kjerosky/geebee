#include "GameBoy.h"
#include "Bus.h"
#include "Cpu.h"
#include "Cartridge.h"

GameBoy::GameBoy(Cartridge* cartridge) {
    bus = new Bus(cartridge);
    cpu = new Cpu(bus);
}

// ----------------------------------------------------------------------------

GameBoy::~GameBoy() {
    delete cpu;
    delete bus;
}

// ----------------------------------------------------------------------------

void GameBoy::execute_next_instruction() {
    do {
        cpu->clock();
    } while (!cpu->is_current_instruction_finished());
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

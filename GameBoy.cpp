#include "GameBoy.h"
#include "Bus.h"
#include "Cpu.h"

GameBoy::GameBoy() {
    bus = new Bus();
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

Uint8* GameBoy::get_ram_contents() {
    return bus->get_ram_contents();
}

// ----------------------------------------------------------------------------

void GameBoy::override_cpu_state(Cpu_Info& new_cpu_state) {
    cpu->override_cpu_state(new_cpu_state);
}

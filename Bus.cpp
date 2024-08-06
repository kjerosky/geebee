#include "Bus.h"
#include <cstring>

Bus::Bus() {
    std::memset(ram, 0x00, RAM_SIZE * sizeof(Uint8));
}

// ----------------------------------------------------------------------------

Bus::~Bus() {
    // nothing to do
}

// ----------------------------------------------------------------------------

Uint8 Bus::cpu_read(Uint16 address) {
    return ram[address];
}

// ----------------------------------------------------------------------------

void Bus::cpu_write(Uint16 address, Uint8 value) {
    if (address >= 0x0000 && address <= 0xFFFF) {
        ram[address] = value;
    }
}

// ----------------------------------------------------------------------------

Uint8* Bus::get_ram_contents() {
    return ram;
}

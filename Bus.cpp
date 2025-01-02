#include "Bus.h"
#include <cstring>
#include "Cartridge.h"

Bus::Bus(Cartridge* cartridge)
: cartridge(cartridge) {
    std::memset(ram, 0x00, RAM_SIZE * sizeof(Uint8));
}

// ----------------------------------------------------------------------------

Bus::~Bus() {
    // nothing to do
}

// ----------------------------------------------------------------------------

Uint8 Bus::cpu_read(Uint16 address) {
    if (address >= 0x0000 && address <= 0x7FFF) {
        return cartridge->cpu_read(address);
    } else {
        return ram[address];
    }
}

// ----------------------------------------------------------------------------

void Bus::cpu_write(Uint16 address, Uint8 value) {
    if (address >= 0x0000 && address <= 0x7FFF) {
        cartridge->cpu_write(address, value);
    } else {
        ram[address] = value;
    }
}

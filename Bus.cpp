#include "Bus.h"

#include <algorithm>

Bus::Bus() {
    std::fill(ram, ram + RAM_SIZE, 0);
}

Bus::~Bus() {
    // do nothing for now
}

uint8_t Bus::readCpu(uint16_t address) {
    if (address >= 0x0000 && address <= 0xFFFF) {
        return ram[address];
    }

    return 0x00;
}

void Bus::writeCpu(uint16_t address, uint8_t value) {
    if (address >= 0x0000 && address <= 0xFFFF) {
        ram[address] = value;
    }
}


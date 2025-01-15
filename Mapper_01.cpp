#include "Mapper_01.h"

#include <iostream>

#include "bit_utils.h"

Mapper_01::Mapper_01(std::vector<Uint8> &rom_contents)
: selected_rom_bank(1),
  rom_contents(rom_contents) {

    // do nothing
}

// ----------------------------------------------------------------------------

Mapper_01::~Mapper_01() {
    // do nothing
}

// ----------------------------------------------------------------------------

Uint8 Mapper_01::cpu_read(Uint16 address) {
    Uint8 read_value = 0xFF;

    if (address >= 0x0000 && address <= 0x3FFF) {
        read_value = rom_contents[address];
    } else if (address >= 0x4000 && address <= 0x7FFF) {
        read_value = rom_contents[(selected_rom_bank * 0x4000) + (address - 0x4000)];
    }

    return read_value;
}

// ----------------------------------------------------------------------------

void Mapper_01::cpu_write(Uint16 address, Uint8 value) {
    if (address >= 0x0000 && address <= 0x1FFF) {
        std::cout << "[INFO] Write to mapper 01 enable/disable ram occurred: [" << hex(address, 4) << "] = " << hex(value, 2) << std::endl;
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        selected_rom_bank = value == 0 ? 1 : value & 0x1F;
    } else if (address >= 0x4000 && address <= 0x5FFF) {
        std::cout << "[INFO] Write to mapper 01 ram bank number occurred: [" << hex(address, 4) << "] = " << hex(value, 2) << std::endl;
    } else if (address >= 0x6000 && address <= 0x7FFF) {
        std::cout << "[INFO] Write to mapper 01 banking mode select occurred: [" << hex(address, 4) << "] = " << hex(value, 2) << std::endl;
    }
}

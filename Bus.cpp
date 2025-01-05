#include "Bus.h"

#include <cstring>

#include "Ppu.h"
#include "Cartridge.h"

Bus::Bus(Ppu* ppu, Cartridge* cartridge)
: ppu(ppu),
  cartridge(cartridge),
  interrupt_enable_register(0x00) {
    std::memset(work_ram_bank_0, 0x00, WORK_RAM_BANK_SIZE * sizeof(Uint8));
    std::memset(work_ram_bank_1, 0x00, WORK_RAM_BANK_SIZE * sizeof(Uint8));
    std::memset(high_ram, 0x00, HIGH_RAM_SIZE * sizeof(Uint8));
}

// ----------------------------------------------------------------------------

Bus::~Bus() {
    // nothing to do
}

// ----------------------------------------------------------------------------

Uint8 Bus::cpu_read(Uint16 address) {
    Uint8 read_value = 0xFF;

    // adjust for echo ram (E000-FDFF is a mirror of C000-DDFF)
    if (address >= 0xE000 && address <= 0xFDFF) {
        address -= 0x2000;
    }

    if (address >= 0x0000 && address <= 0x7FFF) {
        read_value = cartridge->cpu_read(address);
    } else if (address >= 0x8000 && address <= 0x9FFF) {
        read_value = ppu->cpu_read(address);
    } else if (address >= 0xA000 && address <= 0xBFFF) {
        // 8 KiB external ram
    } else if (address >= 0xC000 && address <= 0xCFFF) {
        read_value = work_ram_bank_0[address - 0xC000];
    } else if (address >= 0xD000 && address <= 0xDFFF) {
        read_value = work_ram_bank_1[address - 0xD000];
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        // object attribute memory (oam)
    } else if (address >= 0xFEA0 && address <= 0xFEFF) {
        // not usable (Nintendo says use of this area is prohibited)
    } else if (address >= 0xFF00 && address <= 0xFF7F) {
        read_value = read_from_io_register(address);
    } else if (address >= 0xFF80 && address <= 0xFFFE) {
        read_value = high_ram[address - 0xFF80];
    } else if (address == 0xFFFF) {
        read_value = interrupt_enable_register;
    }

    return read_value;
}

// ----------------------------------------------------------------------------

void Bus::cpu_write(Uint16 address, Uint8 value) {
    // adjust for echo ram (E000-FDFF is a mirror of C000-DDFF)
    if (address >= 0xE000 && address <= 0xFDFF) {
        address -= 0x2000;
    }

    if (address >= 0x0000 && address <= 0x7FFF) {
        cartridge->cpu_write(address, value);
    } else if (address >= 0x8000 && address <= 0x9FFF) {
        ppu->cpu_write(address, value);
    } else if (address >= 0xA000 && address <= 0xBFFF) {
        // 8 KiB external ram
    } else if (address >= 0xC000 && address <= 0xCFFF) {
        work_ram_bank_0[address - 0xC000] = value;
    } else if (address >= 0xD000 && address <= 0xDFFF) {
        work_ram_bank_1[address - 0xD000] = value;
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        // object attribute memory (oam)
    } else if (address >= 0xFEA0 && address <= 0xFEFF) {
        // not usable (Nintendo says use of this area is prohibited)
    } else if (address >= 0xFF00 && address <= 0xFF7F) {
        write_to_io_register(address, value);
    } else if (address >= 0xFF80 && address <= 0xFFFE) {
        high_ram[address - 0xFF80] = value;
    } else if (address == 0xFFFF) {
        interrupt_enable_register = value;
    }
}

// ----------------------------------------------------------------------------

Uint8 Bus::read_from_io_register(Uint16 address) {
    Uint8 read_value = 0xFF;

    if (address >= 0xFF40 && address <= 0xFF6C) {
        read_value = ppu->cpu_read(address);
    }

    return read_value;
}

// ----------------------------------------------------------------------------

void Bus::write_to_io_register(Uint16 address, Uint8 value) {
    if (address >= 0xFF40 && address <= 0xFF6C) {
        ppu->cpu_write(address, value);
    }
}

#include "Mapper_00.h"

Mapper_00::Mapper_00(std::vector<Uint8> &rom_contents)
: rom_contents(rom_contents) {

    // do nothing
}

// ----------------------------------------------------------------------------

Mapper_00::~Mapper_00() {
    // do nothing
}

// ----------------------------------------------------------------------------

Uint8 Mapper_00::cpu_read(Uint16 address) {
    return rom_contents[address];
}

// ----------------------------------------------------------------------------

void Mapper_00::cpu_write(Uint16 address, Uint8 value) {
    // disable writes to rom, for now
}

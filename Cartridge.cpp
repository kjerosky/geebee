#include "Cartridge.h"
#include <fstream>

Cartridge::Cartridge(std::string& rom_filename) {
    std::ifstream file(rom_filename, std::ifstream::binary);
    file.unsetf(std::ios::skipws);

    std::streampos file_size;
    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    contents.resize(file_size);
    file.read((char*)contents.data(), contents.size());
}

// ----------------------------------------------------------------------------

Cartridge::~Cartridge() {
    // do nothing
}

// ----------------------------------------------------------------------------

Uint8 Cartridge::cpu_read(Uint16 address) {
    return contents[address];
}

// ----------------------------------------------------------------------------

void Cartridge::cpu_write(Uint16 address, Uint8 value) {
    // for now, disable writing to the cartridge
}

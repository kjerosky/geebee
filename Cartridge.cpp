#include "Cartridge.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "Mapper.h"
#include "Mapper_00.h"

Cartridge::Cartridge(std::string& rom_filename) {
    std::ifstream file(rom_filename, std::ifstream::binary);
    file.unsetf(std::ios::skipws);

    std::streampos file_size;
    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    contents.resize(file_size);
    file.read((char*)contents.data(), contents.size());

    Uint8 mapper_id = contents[0x0147];
    std::cout << "Mapper: " << (unsigned int)mapper_id << std::endl;
    switch (mapper_id) {
        case 0x00:
            mapper = new Mapper_00();
            break;
        default:
            std::cerr << "[ERROR] Mapper " << (unsigned int)mapper_id << " is not yet implemented!" << std::endl;
            exit(1);
    }
}

// ----------------------------------------------------------------------------

Cartridge::~Cartridge() {
    delete mapper;
}

// ----------------------------------------------------------------------------

Uint8 Cartridge::cpu_read(Uint16 address) {
    Uint16 mapped_address = mapper->map_cpu_read(address);
    return contents[mapped_address];
}

// ----------------------------------------------------------------------------

void Cartridge::cpu_write(Uint16 address, Uint8 value) {
    // for now, disable writing to the cartridge
}

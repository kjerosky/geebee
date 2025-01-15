#include "Cartridge.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "Mapper.h"
#include "Mapper_00.h"
#include "Mapper_01.h"

Cartridge::Cartridge(std::string& rom_filename) {
    std::ifstream file(rom_filename, std::ifstream::binary);
    file.unsetf(std::ios::skipws);

    std::streampos file_size;
    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    rom_contents.resize(file_size);
    file.read((char*)rom_contents.data(), rom_contents.size());

    std::cout << std::endl;

    Uint8 mapper_id = rom_contents[0x0147];
    std::cout << "Mapper: " << (unsigned int)mapper_id << std::endl;

    switch (rom_contents[0x0148]) {
        case 0x00:
            std::cout << "ROM Size: 32KB" << std::endl;
            break;
        case 0x01:
            std::cout << "ROM Size: 64KB" << std::endl;
            break;
        case 0x02:
            std::cout << "ROM Size: 128KB" << std::endl;
            break;
        case 0x03:
            std::cout << "ROM Size: 256KB" << std::endl;
            break;
        case 0x04:
            std::cout << "ROM Size: 512KB" << std::endl;
            break;
        case 0x05:
            std::cout << "ROM Size: 1MB" << std::endl;
            break;
        case 0x06:
            std::cout << "ROM Size: 2MB" << std::endl;
            break;
        case 0x07:
            std::cout << "ROM Size: 4MB" << std::endl;
            break;
        case 0x08:
            std::cout << "ROM Size: 8MB" << std::endl;
            break;
        default:
            std::cerr << "[ERROR] Unknown ROM size (" << (unsigned int)rom_contents[0x0148] << ")!" << std::endl;
            exit(1);
    }

    switch (mapper_id) {
        case 0x00:
            mapper = new Mapper_00(rom_contents);
            break;
        case 0x01:
            mapper = new Mapper_01(rom_contents);
            break;
        default:
            std::cerr << "[ERROR] Mapper " << (unsigned int)mapper_id << " is not yet implemented!" << std::endl;
            exit(1);
    }

    std::cout << std::endl;
}

// ----------------------------------------------------------------------------

Cartridge::~Cartridge() {
    delete mapper;
}

// ----------------------------------------------------------------------------

Uint8 Cartridge::cpu_read(Uint16 address) {
    return mapper->cpu_read(address);
}

// ----------------------------------------------------------------------------

void Cartridge::cpu_write(Uint16 address, Uint8 value) {
    mapper->cpu_write(address, value);
}

#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

class Mapper;

class Cartridge {

public:

    Cartridge(std::string& rom_filename);
    ~Cartridge();

    Uint8 cpu_read(Uint16 address);
    void cpu_write(Uint16 address, Uint8 value);

private:

    std::vector<Uint8> rom_contents;

    Mapper* mapper;
};

#endif

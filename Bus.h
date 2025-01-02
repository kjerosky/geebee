#ifndef BUS_H
#define BUS_H

#include <SDL2/SDL.h>

#define RAM_SIZE 65536

class Cartridge;

class Bus {

public:

    Bus(Cartridge* cartridge);
    ~Bus();

    Uint8 cpu_read(Uint16 address);
    void cpu_write(Uint16 address, Uint8 value);

private:

    Cartridge* cartridge;
    Uint8 ram[65536];
};

#endif

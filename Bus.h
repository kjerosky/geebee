#ifndef BUS_H
#define BUS_H

#include <SDL2/SDL.h>

#define RAM_SIZE 65536

class Bus {

public:

    Bus();
    ~Bus();

    Uint8 cpu_read(Uint16 address);
    void cpu_write(Uint16 address, Uint8 value);
    Uint8* get_ram_contents();

private:

    Uint8 ram[65536];
};

#endif

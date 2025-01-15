#ifndef MAPPER_H
#define MAPPER_H

#include <SDL2/SDL.h>

class Mapper {

public:

    virtual ~Mapper() {};
    virtual Uint8 cpu_read(Uint16 address) = 0;
    virtual void cpu_write(Uint16 address, Uint8 value) = 0;
};

#endif

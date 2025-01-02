#ifndef MAPPER_H
#define MAPPER_H

#include <SDL2/SDL.h>

class Mapper {

public:

    virtual ~Mapper() {};
    virtual Uint16 map_cpu_read(Uint16 address) = 0;
    virtual Uint16 map_cpu_write(Uint16 address) = 0;
};

#endif

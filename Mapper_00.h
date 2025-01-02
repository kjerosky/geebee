#ifndef MAPPER_00_H
#define MAPPER_00_H

#include "Mapper.h"

class Mapper_00 : public Mapper {

public:

    Mapper_00();
    ~Mapper_00();

    Uint16 map_cpu_read(Uint16 address) override;
    Uint16 map_cpu_write(Uint16 address) override;
};

#endif

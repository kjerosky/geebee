#ifndef MAPPER_00_H
#define MAPPER_00_H

#include <vector>

#include "Mapper.h"

class Mapper_00 : public Mapper {

public:

    Mapper_00(std::vector<Uint8>& rom_contents);
    ~Mapper_00();

    Uint8 cpu_read(Uint16 address) override;
    void cpu_write(Uint16 address, Uint8 value) override;

private:

    std::vector<Uint8> rom_contents;
};

#endif

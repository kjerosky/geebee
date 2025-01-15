#ifndef MAPPER_01_H
#define MAPPER_01_H

#include <vector>

#include "Mapper.h"

class Mapper_01 : public Mapper {

public:

    Mapper_01(std::vector<Uint8>& rom_contents);
    ~Mapper_01();

    Uint8 cpu_read(Uint16 address) override;
    void cpu_write(Uint16 address, Uint8 value) override;

private:

    int selected_rom_bank;

    std::vector<Uint8>& rom_contents;
};

#endif

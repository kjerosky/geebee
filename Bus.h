#ifndef BUS_H
#define BUS_H

#include <cstdint>

class Bus {

public:

    Bus();
    ~Bus();

    uint8_t readCpu(uint16_t address);
    void writeCpu(uint16_t address, uint8_t value);

private:

    static const int RAM_SIZE = 65536;
    uint8_t ram[RAM_SIZE];
};

#endif


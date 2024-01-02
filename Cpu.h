#ifndef CPU_H
#define CPU_H

#include "Bus.h"

class Cpu {

public:

    Cpu(Bus* bus);
    ~Cpu();

private:

    Bus* bus;
};

#endif


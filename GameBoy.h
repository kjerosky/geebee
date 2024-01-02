#ifndef GAME_BOY_H
#define GAME_BOY_H

#include "Cpu.h"
#include "Bus.h"

class GameBoy {

public:

    GameBoy();
    ~GameBoy();

private:

    Bus* bus;
    Cpu* cpu;
};

#endif


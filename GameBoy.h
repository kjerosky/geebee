#ifndef GAME_BOY_H
#define GAME_BOY_H

#include "Cpu.h"

class Cpu;
class Bus;

class GameBoy {

public:

    GameBoy();
    ~GameBoy();

    void execute_next_instruction();
    Cpu_Info get_cpu_info();
    Uint8* get_ram_contents();

    void override_cpu_state(Cpu_Info& new_cpu_state);
    void generate_interrupt(int interrupt_bit);

private:

    Cpu* cpu;
    Bus* bus;
};

#endif

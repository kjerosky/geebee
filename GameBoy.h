#ifndef GAME_BOY_H
#define GAME_BOY_H

#include "Cpu.h"

class Cpu;
class Bus;
class Cartridge;

class GameBoy {

public:

    GameBoy(Cartridge* cartridge);
    ~GameBoy();

    void execute_next_instruction();
    Cpu_Info get_cpu_info();

    void override_cpu_state(Cpu_Info& new_cpu_state);
    void generate_interrupt(int interrupt_bit);
    Uint8 read_from_bus(Uint16 address);
    void write_to_bus(Uint16 address, Uint8 value);

private:

    Cpu* cpu;
    Bus* bus;
};

#endif

#ifndef BUS_H
#define BUS_H

#include <SDL2/SDL.h>

#define WORK_RAM_BANK_SIZE 4096
#define HIGH_RAM_SIZE 127

class Ppu;
class Cartridge;

class Bus {

public:

    Bus(Ppu* ppu, Cartridge* cartridge);
    ~Bus();

    Uint8 cpu_read(Uint16 address);
    void cpu_write(Uint16 address, Uint8 value);

    bool check_and_reset_oam_dma_started(Uint16& output_oam_dma_start_address);

    Uint8 clock_machine_cycle();

private:

    Ppu* ppu;
    Cartridge* cartridge;

    Uint8 work_ram_bank_0[WORK_RAM_BANK_SIZE];
    Uint8 work_ram_bank_1[WORK_RAM_BANK_SIZE];
    Uint8 high_ram[HIGH_RAM_SIZE];

    Uint8 interrupt_flag_register;
    Uint8 interrupt_enable_register;

    bool oam_dma_started;
    Uint16 oam_dma_start_address;

    Uint8 joypad_register;
    Uint8 div_register;
    Uint8 timer_counter;
    Uint8 timer_modulo;
    Uint8 timer_control;

    Uint16 machine_cycles;

    Uint8 read_from_io_register(Uint16 address);
    void write_to_io_register(Uint16 address, Uint8 value);

    void refresh_joypad_register(Uint8 input_byte);
};

#endif

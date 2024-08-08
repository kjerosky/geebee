#ifndef CPU_H
#define CPU_H

#include <SDL2/SDL.h>
#include <vector>

class Bus;

struct Cpu_Info {
    Uint8 a;
    Uint8 b;
    Uint8 c;
    Uint8 d;
    Uint8 e;
    Uint8 h;
    Uint8 l;
    Uint16 pc;
    Uint16 sp;
    bool z_flag;
    bool n_flag;
    bool h_flag;
    bool c_flag;
};

class Cpu {

public:

    Cpu(Bus* bus);
    ~Cpu();

    void clock();
    bool is_current_instruction_finished();
    void reset();
    Cpu_Info get_cpu_info();

private:

    void initialize_opcode_tables();

    Bus* bus;

    struct Instruction {
        void (Cpu::*fetch)();
        int (Cpu::*execute)();
        void (Cpu::*store)();
        int base_cycles;
    };
    std::vector<Instruction> opcode_table;
    std::vector<Instruction> prefixed_opcode_table;

    Uint8 a;
    Uint8 b;
    Uint8 c;
    Uint8 d;
    Uint8 e;
    Uint8 h;
    Uint8 l;
    Uint16 pc;
    Uint16 sp;

    bool z_flag;
    bool n_flag;
    bool h_flag;
    bool c_flag;

    int current_instruction_cycles_remaining;

    Uint8 fetched_u8;
    Uint8 computed_u8;

    Uint16 fetched_u16;
    Uint16 computed_u16;

    // opcode fetch functions
    void fetch_from_a();
    void fetch_from_b();
    void fetch_from_c();
    void fetch_from_d();
    void fetch_from_e();
    void fetch_from_h();
    void fetch_from_l();
    void fetch_indirect_hl();
    void fetch_indirect_hl_plus();
    void fetch_indirect_hl_minus();
    void fetch_indirect_bc();
    void fetch_indirect_de();
    void fetch_direct_ff_offset();
    void fetch_indirect_ff_with_c_offset();
    void fetch_from_immediate_u8();
    void fetch_direct();

    // opcode execute functions
    int ld();

    // opcode store functions
    void store_to_a();
    void store_to_b();
    void store_to_c();
    void store_to_d();
    void store_to_e();
    void store_to_h();
    void store_to_l();
    void store_indirect_hl();
    void store_indirect_hl_plus();
    void store_indirect_hl_minus();
    void store_indirect_bc();
    void store_indirect_de();
    void store_direct_ff_offset();
    void store_indirect_ff_with_c_offset();
    void store_direct();

    Uint16 get_hl();
    Uint16 get_bc();
    Uint16 get_de();
};

#endif

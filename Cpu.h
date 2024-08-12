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

    enum FLAGS {
        Z_FLAG = 1 << 7,
        N_FLAG = 1 << 6,
        H_FLAG = 1 << 5,
        C_FLAG = 1 << 4,
    };

    void initialize_opcode_tables();
    bool get_flag(FLAGS flag);
    void set_flag(FLAGS flag, bool value);

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
    Uint8 flags;
    Uint16 pc;
    Uint16 sp;

    int current_instruction_cycles_remaining;

    Uint8 fetched_u8;
    Uint8 computed_u8;

    Uint8 fetched_u16_lsb;
    Uint8 fetched_u16_msb;
    Uint8 computed_u16_lsb;
    Uint8 computed_u16_msb;

    // opcode fetch functions
    void fetch_from_a();
    void fetch_from_b();
    void fetch_from_c();
    void fetch_from_d();
    void fetch_from_e();
    void fetch_from_h();
    void fetch_from_l();
    void fetch_from_bc();
    void fetch_from_de();
    void fetch_from_hl();
    void fetch_from_af();
    void fetch_from_sp();
    void fetch_indirect_hl();
    void fetch_indirect_hl_plus();
    void fetch_indirect_hl_minus();
    void fetch_indirect_bc();
    void fetch_indirect_de();
    void fetch_direct_ff_offset();
    void fetch_indirect_ff_with_c_offset();
    void fetch_from_immediate_u8();
    void fetch_from_immediate_u16();
    void fetch_direct();
    void pop();
    void fetch_from_adjusted_sp();

    // opcode execute functions
    int ld_8bit();
    int ld_16bit();
    int and_with_a();
    int xor_with_a();
    int or_with_a();
    int inc_8bit();
    int inc_16bit();

    // opcode store functions
    void store_to_a();
    void store_to_b();
    void store_to_c();
    void store_to_d();
    void store_to_e();
    void store_to_h();
    void store_to_l();
    void store_to_bc();
    void store_to_de();
    void store_to_hl();
    void store_to_sp();
    void store_to_af();
    void store_indirect_hl();
    void store_indirect_hl_plus();
    void store_indirect_hl_minus();
    void store_indirect_bc();
    void store_indirect_de();
    void store_direct_ff_offset();
    void store_indirect_ff_with_c_offset();
    void store_direct_8bit();
    void store_direct_16bit();
    void push();

    Uint16 get_hl();
    Uint16 get_bc();
    Uint16 get_de();

    Uint32 add_and_track_carries(Uint32 a, Uint32 b, Uint32& carries);
    Uint16 get_fetched_u16();
    void split_u16(Uint16 u16, Uint8& msb, Uint8& lsb);
    Uint16 join_to_u16(Uint8 msb, Uint8 lsb);
};

#endif

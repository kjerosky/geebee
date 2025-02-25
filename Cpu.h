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

    // not for overriding, just display
    bool ime;
};

class Cpu {

public:

    Cpu(Bus* bus);
    ~Cpu();

    void clock();
    bool is_current_instruction_finished();
    void reset();

    Cpu_Info get_cpu_info();
    void disassemble(Uint16 start_address, int num_of_instructions, std::vector<std::string>& output);
    void override_cpu_state(Cpu_Info& new_cpu_state);

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
    void set_target_bit(Uint8 prefixed_opcode_byte);
    void set_target_rst_address(Uint8 opcode_byte);
    bool is_interrupt_pending();
    bool handle_pending_interrupts();

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
    bool ime;

    int current_instruction_cycles_remaining;
    int instructions_remaining_to_enable_ime;
    bool is_halted;

    Uint8 fetched_u8;
    Uint8 computed_u8;
    int target_bit;
    Uint16 target_rst_address;

    Uint8 fetched_u16_lsb;
    Uint8 fetched_u16_msb;
    Uint8 computed_u16_lsb;
    Uint8 computed_u16_msb;

    bool is_current_opcode_prefixed;

    bool is_oam_dma_transfer_in_progress;
    Uint16 oam_dma_source_address;

    bool is_trace_enabled;

    // opcode fetch functions
    void fetch_nop();
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
    int execute_nop();
    int ld_8bit();
    int ld_16bit();
    int and_with_a();
    int xor_with_a();
    int or_with_a();
    int inc_8bit();
    int inc_16bit();
    int dec_8bit();
    int dec_16bit();
    int add_to_a();
    int add_to_a_with_carry();
    int add_to_hl();
    int add_to_sp();
    int subtract_from_a();
    int subtract_from_a_with_carry();
    int jump_absolute();
    int jump_absolute_if_z_reset();
    int jump_absolute_if_c_reset();
    int jump_absolute_if_z_set();
    int jump_absolute_if_c_set();
    int rotate_left();
    int rotate_left_with_carry();
    int rotate_right();
    int rotate_right_with_carry();
    int shift_left_arithmetically();
    int shift_right_arithmetically();
    int shift_right_logically();
    int swap();
    int test_bit();
    int reset_bit();
    int set_bit();
    int jump_relative();
    int jump_relative_if_z_reset();
    int jump_relative_if_c_reset();
    int jump_relative_if_z_set();
    int jump_relative_if_c_set();
    int complement();
    int complement_c_flag();
    int call();
    int call_if_z_reset();
    int call_if_c_reset();
    int call_if_z_set();
    int call_if_c_set();
    int ret();
    int ret_if_z_reset();
    int ret_if_c_reset();
    int ret_if_z_set();
    int ret_if_c_set();
    int set_c_flag();
    int rst();
    int disable_ime();
    int enable_ime();
    int reti();
    int daa();
    int halt();

    // opcode store functions
    void store_nop();
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
    void store_to_pc();

    Uint16 get_hl();
    Uint16 get_bc();
    Uint16 get_de();

    Uint32 full_add_8bit(Uint32 a, Uint32 b, bool carry_in, bool& half_carry_out, bool& carry_out);
    Uint32 full_add_16bit(Uint32 a, Uint32 b, bool carry_in, bool& half_carry_out, bool& carry_out);
    Uint32 full_sub_8bit(Uint32 a, Uint32 b, bool carry_in, bool& half_carry_out, bool& carry_out);
    Uint16 get_fetched_u16();
};

#endif

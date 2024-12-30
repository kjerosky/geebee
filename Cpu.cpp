#include "Cpu.h"
#include "Bus.h"

Cpu::Cpu(Bus* bus)
        : bus(bus),
          opcode_table(256),
          prefixed_opcode_table(256) {

    initialize_opcode_tables();

    reset();
}

// ----------------------------------------------------------------------------

Cpu::~Cpu() {
    // nothing to do
}

// ----------------------------------------------------------------------------

void Cpu::initialize_opcode_tables() {
    opcode_table[0x00] = { &Cpu::fetch_nop, &Cpu::execute_nop, &Cpu::store_nop, 1 };
    opcode_table[0x01] = { &Cpu::fetch_from_immediate_u16, &Cpu::ld_16bit, &Cpu::store_to_bc, 3 };
    opcode_table[0x02] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_indirect_bc, 2 };
    opcode_table[0x03] = { &Cpu::fetch_from_bc, &Cpu::inc_16bit, &Cpu::store_to_bc, 2 };
    opcode_table[0x04] = { &Cpu::fetch_from_b, &Cpu::inc_8bit, &Cpu::store_to_b, 1 };
    opcode_table[0x05] = { &Cpu::fetch_from_b, &Cpu::dec_8bit, &Cpu::store_to_b, 1 };
    opcode_table[0x06] = { &Cpu::fetch_from_immediate_u8, &Cpu::ld_8bit, &Cpu::store_to_b, 2 };
    opcode_table[0x07] = { &Cpu::fetch_from_a, &Cpu::rotate_left, &Cpu::store_to_a, 1 };
    opcode_table[0x08] = { &Cpu::fetch_from_sp, &Cpu::ld_16bit, &Cpu::store_direct_16bit, 5 };
    opcode_table[0x09] = { &Cpu::fetch_from_bc, &Cpu::add_to_hl, &Cpu::store_to_hl, 2 };
    opcode_table[0x0A] = { &Cpu::fetch_indirect_bc, &Cpu::ld_8bit, &Cpu::store_to_a, 2 };
    opcode_table[0x0B] = { &Cpu::fetch_from_bc, &Cpu::dec_16bit, &Cpu::store_to_bc, 2 };
    opcode_table[0x0C] = { &Cpu::fetch_from_c, &Cpu::inc_8bit, &Cpu::store_to_c, 1 };
    opcode_table[0x0D] = { &Cpu::fetch_from_c, &Cpu::dec_8bit, &Cpu::store_to_c, 1 };
    opcode_table[0x0E] = { &Cpu::fetch_from_immediate_u8, &Cpu::ld_8bit, &Cpu::store_to_c, 2 };
    opcode_table[0x0F] = { &Cpu::fetch_from_a, &Cpu::rotate_right, &Cpu::store_to_a, 1 };

    //opcode_table[0x10] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x11] = { &Cpu::fetch_from_immediate_u16, &Cpu::ld_16bit, &Cpu::store_to_de, 3 };
    opcode_table[0x12] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_indirect_de, 2 };
    opcode_table[0x13] = { &Cpu::fetch_from_de, &Cpu::inc_16bit, &Cpu::store_to_de, 2 };
    opcode_table[0x14] = { &Cpu::fetch_from_d, &Cpu::inc_8bit, &Cpu::store_to_d, 1 };
    opcode_table[0x15] = { &Cpu::fetch_from_d, &Cpu::dec_8bit, &Cpu::store_to_d, 1 };
    opcode_table[0x16] = { &Cpu::fetch_from_immediate_u8, &Cpu::ld_8bit, &Cpu::store_to_d, 2 };
    opcode_table[0x17] = { &Cpu::fetch_from_a, &Cpu::rotate_left_with_carry, &Cpu::store_to_a, 1 };
    //opcode_table[0x18] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x19] = { &Cpu::fetch_from_de, &Cpu::add_to_hl, &Cpu::store_to_hl, 2 };
    opcode_table[0x1A] = { &Cpu::fetch_indirect_de, &Cpu::ld_8bit, &Cpu::store_to_a, 2 };
    opcode_table[0x1B] = { &Cpu::fetch_from_de, &Cpu::dec_16bit, &Cpu::store_to_de, 2 };
    opcode_table[0x1C] = { &Cpu::fetch_from_e, &Cpu::inc_8bit, &Cpu::store_to_e, 1 };
    opcode_table[0x1D] = { &Cpu::fetch_from_e, &Cpu::dec_8bit, &Cpu::store_to_e, 1 };
    opcode_table[0x1E] = { &Cpu::fetch_from_immediate_u8, &Cpu::ld_8bit, &Cpu::store_to_e, 2 };
    opcode_table[0x1F] = { &Cpu::fetch_from_a, &Cpu::rotate_right_with_carry, &Cpu::store_to_a, 1 };

    //opcode_table[0x20] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x21] = { &Cpu::fetch_from_immediate_u16, &Cpu::ld_16bit, &Cpu::store_to_hl, 3 };
    opcode_table[0x22] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_indirect_hl_plus, 2 };
    opcode_table[0x23] = { &Cpu::fetch_from_hl, &Cpu::inc_16bit, &Cpu::store_to_hl, 2 };
    opcode_table[0x24] = { &Cpu::fetch_from_h, &Cpu::inc_8bit, &Cpu::store_to_h, 1 };
    opcode_table[0x25] = { &Cpu::fetch_from_h, &Cpu::dec_8bit, &Cpu::store_to_h, 1 };
    opcode_table[0x26] = { &Cpu::fetch_from_immediate_u8, &Cpu::ld_8bit, &Cpu::store_to_h, 2 };
    //opcode_table[0x27] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x28] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x29] = { &Cpu::fetch_from_hl, &Cpu::add_to_hl, &Cpu::store_to_hl, 2 };
    opcode_table[0x2A] = { &Cpu::fetch_indirect_hl_plus, &Cpu::ld_8bit, &Cpu::store_to_a, 2 };
    opcode_table[0x2B] = { &Cpu::fetch_from_hl, &Cpu::dec_16bit, &Cpu::store_to_hl, 2 };
    opcode_table[0x2C] = { &Cpu::fetch_from_l, &Cpu::inc_8bit, &Cpu::store_to_l, 1 };
    opcode_table[0x2D] = { &Cpu::fetch_from_l, &Cpu::dec_8bit, &Cpu::store_to_l, 1 };
    opcode_table[0x2E] = { &Cpu::fetch_from_immediate_u8, &Cpu::ld_8bit, &Cpu::store_to_l, 2 };
    //opcode_table[0x2F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //opcode_table[0x30] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x31] = { &Cpu::fetch_from_immediate_u16, &Cpu::ld_16bit, &Cpu::store_to_sp, 3 };
    opcode_table[0x32] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_indirect_hl_minus, 2 };
    opcode_table[0x33] = { &Cpu::fetch_from_sp, &Cpu::inc_16bit, &Cpu::store_to_sp, 2 };
    opcode_table[0x34] = { &Cpu::fetch_indirect_hl, &Cpu::inc_8bit, &Cpu::store_indirect_hl, 3 };
    opcode_table[0x35] = { &Cpu::fetch_indirect_hl, &Cpu::dec_8bit, &Cpu::store_indirect_hl, 3 };
    opcode_table[0x36] = { &Cpu::fetch_from_immediate_u8, &Cpu::ld_8bit, &Cpu::store_indirect_hl, 3 };
    //opcode_table[0x37] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x38] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x39] = { &Cpu::fetch_from_sp, &Cpu::add_to_hl, &Cpu::store_to_hl, 2 };
    opcode_table[0x3A] = { &Cpu::fetch_indirect_hl_minus, &Cpu::ld_8bit, &Cpu::store_to_a, 2 };
    opcode_table[0x3B] = { &Cpu::fetch_from_sp, &Cpu::dec_16bit, &Cpu::store_to_sp, 2 };
    opcode_table[0x3C] = { &Cpu::fetch_from_a, &Cpu::inc_8bit, &Cpu::store_to_a, 1 };
    opcode_table[0x3D] = { &Cpu::fetch_from_a, &Cpu::dec_8bit, &Cpu::store_to_a, 1 };
    opcode_table[0x3E] = { &Cpu::fetch_from_immediate_u8, &Cpu::ld_8bit, &Cpu::store_to_a, 2 };
    //opcode_table[0x3F] = { &Cpu::, &Cpu::, &Cpu::,  };

    opcode_table[0x40] = { &Cpu::fetch_from_b, &Cpu::ld_8bit, &Cpu::store_to_b, 1 };
    opcode_table[0x41] = { &Cpu::fetch_from_c, &Cpu::ld_8bit, &Cpu::store_to_b, 1 };
    opcode_table[0x42] = { &Cpu::fetch_from_d, &Cpu::ld_8bit, &Cpu::store_to_b, 1 };
    opcode_table[0x43] = { &Cpu::fetch_from_e, &Cpu::ld_8bit, &Cpu::store_to_b, 1 };
    opcode_table[0x44] = { &Cpu::fetch_from_h, &Cpu::ld_8bit, &Cpu::store_to_b, 1 };
    opcode_table[0x45] = { &Cpu::fetch_from_l, &Cpu::ld_8bit, &Cpu::store_to_b, 1 };
    opcode_table[0x46] = { &Cpu::fetch_indirect_hl, &Cpu::ld_8bit, &Cpu::store_to_b, 2 };
    opcode_table[0x47] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_to_b, 1 };
    opcode_table[0x48] = { &Cpu::fetch_from_b, &Cpu::ld_8bit, &Cpu::store_to_c, 1 };
    opcode_table[0x49] = { &Cpu::fetch_from_c, &Cpu::ld_8bit, &Cpu::store_to_c, 1 };
    opcode_table[0x4A] = { &Cpu::fetch_from_d, &Cpu::ld_8bit, &Cpu::store_to_c, 1 };
    opcode_table[0x4B] = { &Cpu::fetch_from_e, &Cpu::ld_8bit, &Cpu::store_to_c, 1 };
    opcode_table[0x4C] = { &Cpu::fetch_from_h, &Cpu::ld_8bit, &Cpu::store_to_c, 1 };
    opcode_table[0x4D] = { &Cpu::fetch_from_l, &Cpu::ld_8bit, &Cpu::store_to_c, 1 };
    opcode_table[0x4E] = { &Cpu::fetch_indirect_hl, &Cpu::ld_8bit, &Cpu::store_to_c, 2 };
    opcode_table[0x4F] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_to_c, 1 };

    opcode_table[0x50] = { &Cpu::fetch_from_b, &Cpu::ld_8bit, &Cpu::store_to_d, 1 };
    opcode_table[0x51] = { &Cpu::fetch_from_c, &Cpu::ld_8bit, &Cpu::store_to_d, 1 };
    opcode_table[0x52] = { &Cpu::fetch_from_d, &Cpu::ld_8bit, &Cpu::store_to_d, 1 };
    opcode_table[0x53] = { &Cpu::fetch_from_e, &Cpu::ld_8bit, &Cpu::store_to_d, 1 };
    opcode_table[0x54] = { &Cpu::fetch_from_h, &Cpu::ld_8bit, &Cpu::store_to_d, 1 };
    opcode_table[0x55] = { &Cpu::fetch_from_l, &Cpu::ld_8bit, &Cpu::store_to_d, 1 };
    opcode_table[0x56] = { &Cpu::fetch_indirect_hl, &Cpu::ld_8bit, &Cpu::store_to_d, 2 };
    opcode_table[0x57] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_to_d, 1 };
    opcode_table[0x58] = { &Cpu::fetch_from_b, &Cpu::ld_8bit, &Cpu::store_to_e, 1 };
    opcode_table[0x59] = { &Cpu::fetch_from_c, &Cpu::ld_8bit, &Cpu::store_to_e, 1 };
    opcode_table[0x5A] = { &Cpu::fetch_from_d, &Cpu::ld_8bit, &Cpu::store_to_e, 1 };
    opcode_table[0x5B] = { &Cpu::fetch_from_e, &Cpu::ld_8bit, &Cpu::store_to_e, 1 };
    opcode_table[0x5C] = { &Cpu::fetch_from_h, &Cpu::ld_8bit, &Cpu::store_to_e, 1 };
    opcode_table[0x5D] = { &Cpu::fetch_from_l, &Cpu::ld_8bit, &Cpu::store_to_e, 1 };
    opcode_table[0x5E] = { &Cpu::fetch_indirect_hl, &Cpu::ld_8bit, &Cpu::store_to_e, 2 };
    opcode_table[0x5F] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_to_e, 1 };

    opcode_table[0x60] = { &Cpu::fetch_from_b, &Cpu::ld_8bit, &Cpu::store_to_h, 1 };
    opcode_table[0x61] = { &Cpu::fetch_from_c, &Cpu::ld_8bit, &Cpu::store_to_h, 1 };
    opcode_table[0x62] = { &Cpu::fetch_from_d, &Cpu::ld_8bit, &Cpu::store_to_h, 1 };
    opcode_table[0x63] = { &Cpu::fetch_from_e, &Cpu::ld_8bit, &Cpu::store_to_h, 1 };
    opcode_table[0x64] = { &Cpu::fetch_from_h, &Cpu::ld_8bit, &Cpu::store_to_h, 1 };
    opcode_table[0x65] = { &Cpu::fetch_from_l, &Cpu::ld_8bit, &Cpu::store_to_h, 1 };
    opcode_table[0x66] = { &Cpu::fetch_indirect_hl, &Cpu::ld_8bit, &Cpu::store_to_h, 2 };
    opcode_table[0x67] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_to_h, 1 };
    opcode_table[0x68] = { &Cpu::fetch_from_b, &Cpu::ld_8bit, &Cpu::store_to_l, 1 };
    opcode_table[0x69] = { &Cpu::fetch_from_c, &Cpu::ld_8bit, &Cpu::store_to_l, 1 };
    opcode_table[0x6A] = { &Cpu::fetch_from_d, &Cpu::ld_8bit, &Cpu::store_to_l, 1 };
    opcode_table[0x6B] = { &Cpu::fetch_from_e, &Cpu::ld_8bit, &Cpu::store_to_l, 1 };
    opcode_table[0x6C] = { &Cpu::fetch_from_h, &Cpu::ld_8bit, &Cpu::store_to_l, 1 };
    opcode_table[0x6D] = { &Cpu::fetch_from_l, &Cpu::ld_8bit, &Cpu::store_to_l, 1 };
    opcode_table[0x6E] = { &Cpu::fetch_indirect_hl, &Cpu::ld_8bit, &Cpu::store_to_l, 2 };
    opcode_table[0x6F] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_to_l, 1 };

    opcode_table[0x70] = { &Cpu::fetch_from_b, &Cpu::ld_8bit, &Cpu::store_indirect_hl, 2 };
    opcode_table[0x71] = { &Cpu::fetch_from_c, &Cpu::ld_8bit, &Cpu::store_indirect_hl, 2 };
    opcode_table[0x72] = { &Cpu::fetch_from_d, &Cpu::ld_8bit, &Cpu::store_indirect_hl, 2 };
    opcode_table[0x73] = { &Cpu::fetch_from_e, &Cpu::ld_8bit, &Cpu::store_indirect_hl, 2 };
    opcode_table[0x74] = { &Cpu::fetch_from_h, &Cpu::ld_8bit, &Cpu::store_indirect_hl, 2 };
    opcode_table[0x75] = { &Cpu::fetch_from_l, &Cpu::ld_8bit, &Cpu::store_indirect_hl, 2 };
    //opcode_table[0x76] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x77] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_indirect_hl, 2 };
    opcode_table[0x78] = { &Cpu::fetch_from_b, &Cpu::ld_8bit, &Cpu::store_to_a, 1 };
    opcode_table[0x79] = { &Cpu::fetch_from_c, &Cpu::ld_8bit, &Cpu::store_to_a, 1 };
    opcode_table[0x7A] = { &Cpu::fetch_from_d, &Cpu::ld_8bit, &Cpu::store_to_a, 1 };
    opcode_table[0x7B] = { &Cpu::fetch_from_e, &Cpu::ld_8bit, &Cpu::store_to_a, 1 };
    opcode_table[0x7C] = { &Cpu::fetch_from_h, &Cpu::ld_8bit, &Cpu::store_to_a, 1 };
    opcode_table[0x7D] = { &Cpu::fetch_from_l, &Cpu::ld_8bit, &Cpu::store_to_a, 1 };
    opcode_table[0x7E] = { &Cpu::fetch_indirect_hl, &Cpu::ld_8bit, &Cpu::store_to_a, 2 };
    opcode_table[0x7F] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_to_a, 1 };

    opcode_table[0x80] = { &Cpu::fetch_from_b, &Cpu::add_to_a, &Cpu::store_to_a, 1 };
    opcode_table[0x81] = { &Cpu::fetch_from_c, &Cpu::add_to_a, &Cpu::store_to_a, 1 };
    opcode_table[0x82] = { &Cpu::fetch_from_d, &Cpu::add_to_a, &Cpu::store_to_a, 1 };
    opcode_table[0x83] = { &Cpu::fetch_from_e, &Cpu::add_to_a, &Cpu::store_to_a, 1 };
    opcode_table[0x84] = { &Cpu::fetch_from_h, &Cpu::add_to_a, &Cpu::store_to_a, 1 };
    opcode_table[0x85] = { &Cpu::fetch_from_l, &Cpu::add_to_a, &Cpu::store_to_a, 1 };
    opcode_table[0x86] = { &Cpu::fetch_indirect_hl, &Cpu::add_to_a, &Cpu::store_to_a, 2 };
    opcode_table[0x87] = { &Cpu::fetch_from_a, &Cpu::add_to_a, &Cpu::store_to_a, 1 };
    opcode_table[0x88] = { &Cpu::fetch_from_b, &Cpu::add_to_a_with_carry, &Cpu::store_to_a, 1 };
    opcode_table[0x89] = { &Cpu::fetch_from_c, &Cpu::add_to_a_with_carry, &Cpu::store_to_a, 1 };
    opcode_table[0x8A] = { &Cpu::fetch_from_d, &Cpu::add_to_a_with_carry, &Cpu::store_to_a, 1 };
    opcode_table[0x8B] = { &Cpu::fetch_from_e, &Cpu::add_to_a_with_carry, &Cpu::store_to_a, 1 };
    opcode_table[0x8C] = { &Cpu::fetch_from_h, &Cpu::add_to_a_with_carry, &Cpu::store_to_a, 1 };
    opcode_table[0x8D] = { &Cpu::fetch_from_l, &Cpu::add_to_a_with_carry, &Cpu::store_to_a, 1 };
    opcode_table[0x8E] = { &Cpu::fetch_indirect_hl, &Cpu::add_to_a_with_carry, &Cpu::store_to_a, 2 };
    opcode_table[0x8F] = { &Cpu::fetch_from_a, &Cpu::add_to_a_with_carry, &Cpu::store_to_a, 1 };

    opcode_table[0x90] = { &Cpu::fetch_from_b, &Cpu::subtract_from_a, &Cpu::store_to_a, 1 };
    opcode_table[0x91] = { &Cpu::fetch_from_c, &Cpu::subtract_from_a, &Cpu::store_to_a, 1 };
    opcode_table[0x92] = { &Cpu::fetch_from_d, &Cpu::subtract_from_a, &Cpu::store_to_a, 1 };
    opcode_table[0x93] = { &Cpu::fetch_from_e, &Cpu::subtract_from_a, &Cpu::store_to_a, 1 };
    opcode_table[0x94] = { &Cpu::fetch_from_h, &Cpu::subtract_from_a, &Cpu::store_to_a, 1 };
    opcode_table[0x95] = { &Cpu::fetch_from_l, &Cpu::subtract_from_a, &Cpu::store_to_a, 1 };
    opcode_table[0x96] = { &Cpu::fetch_indirect_hl, &Cpu::subtract_from_a, &Cpu::store_to_a, 2 };
    opcode_table[0x97] = { &Cpu::fetch_from_a, &Cpu::subtract_from_a, &Cpu::store_to_a, 1 };
    opcode_table[0x98] = { &Cpu::fetch_from_b, &Cpu::subtract_from_a_with_carry, &Cpu::store_to_a, 1 };
    opcode_table[0x99] = { &Cpu::fetch_from_c, &Cpu::subtract_from_a_with_carry, &Cpu::store_to_a, 1 };
    opcode_table[0x9A] = { &Cpu::fetch_from_d, &Cpu::subtract_from_a_with_carry, &Cpu::store_to_a, 1 };
    opcode_table[0x9B] = { &Cpu::fetch_from_e, &Cpu::subtract_from_a_with_carry, &Cpu::store_to_a, 1 };
    opcode_table[0x9C] = { &Cpu::fetch_from_h, &Cpu::subtract_from_a_with_carry, &Cpu::store_to_a, 1 };
    opcode_table[0x9D] = { &Cpu::fetch_from_l, &Cpu::subtract_from_a_with_carry, &Cpu::store_to_a, 1 };
    opcode_table[0x9E] = { &Cpu::fetch_indirect_hl, &Cpu::subtract_from_a_with_carry, &Cpu::store_to_a, 2 };
    opcode_table[0x9F] = { &Cpu::fetch_from_a, &Cpu::subtract_from_a_with_carry, &Cpu::store_to_a, 1 };

    opcode_table[0xA0] = { &Cpu::fetch_from_b, &Cpu::and_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xA1] = { &Cpu::fetch_from_c, &Cpu::and_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xA2] = { &Cpu::fetch_from_d, &Cpu::and_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xA3] = { &Cpu::fetch_from_e, &Cpu::and_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xA4] = { &Cpu::fetch_from_h, &Cpu::and_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xA5] = { &Cpu::fetch_from_l, &Cpu::and_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xA6] = { &Cpu::fetch_indirect_hl, &Cpu::and_with_a, &Cpu::store_to_a, 2 };
    opcode_table[0xA7] = { &Cpu::fetch_from_a, &Cpu::and_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xA8] = { &Cpu::fetch_from_b, &Cpu::xor_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xA9] = { &Cpu::fetch_from_c, &Cpu::xor_with_a, &Cpu::store_to_a, 2 };
    opcode_table[0xAA] = { &Cpu::fetch_from_d, &Cpu::xor_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xAB] = { &Cpu::fetch_from_e, &Cpu::xor_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xAC] = { &Cpu::fetch_from_h, &Cpu::xor_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xAD] = { &Cpu::fetch_from_l, &Cpu::xor_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xAE] = { &Cpu::fetch_indirect_hl, &Cpu::xor_with_a, &Cpu::store_to_a, 2 };
    opcode_table[0xAF] = { &Cpu::fetch_from_a, &Cpu::xor_with_a, &Cpu::store_to_a, 1 };

    opcode_table[0xB0] = { &Cpu::fetch_from_b, &Cpu::or_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xB1] = { &Cpu::fetch_from_c, &Cpu::or_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xB2] = { &Cpu::fetch_from_d, &Cpu::or_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xB3] = { &Cpu::fetch_from_e, &Cpu::or_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xB4] = { &Cpu::fetch_from_h, &Cpu::or_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xB5] = { &Cpu::fetch_from_l, &Cpu::or_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xB6] = { &Cpu::fetch_indirect_hl, &Cpu::or_with_a, &Cpu::store_to_a, 2 };
    opcode_table[0xB7] = { &Cpu::fetch_from_a, &Cpu::or_with_a, &Cpu::store_to_a, 1 };
    opcode_table[0xB8] = { &Cpu::fetch_from_b, &Cpu::subtract_from_a, &Cpu::store_nop, 1 };
    opcode_table[0xB9] = { &Cpu::fetch_from_c, &Cpu::subtract_from_a, &Cpu::store_nop, 1 };
    opcode_table[0xBA] = { &Cpu::fetch_from_d, &Cpu::subtract_from_a, &Cpu::store_nop, 1 };
    opcode_table[0xBB] = { &Cpu::fetch_from_e, &Cpu::subtract_from_a, &Cpu::store_nop, 1 };
    opcode_table[0xBC] = { &Cpu::fetch_from_h, &Cpu::subtract_from_a, &Cpu::store_nop, 1 };
    opcode_table[0xBD] = { &Cpu::fetch_from_l, &Cpu::subtract_from_a, &Cpu::store_nop, 1 };
    opcode_table[0xBE] = { &Cpu::fetch_indirect_hl, &Cpu::subtract_from_a, &Cpu::store_nop, 2 };
    opcode_table[0xBF] = { &Cpu::fetch_from_a, &Cpu::subtract_from_a, &Cpu::store_nop, 1 };

    //opcode_table[0xC0] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xC1] = { &Cpu::pop, &Cpu::ld_16bit, &Cpu::store_to_bc, 3 };
    opcode_table[0xC2] = { &Cpu::fetch_from_immediate_u16, &Cpu::jump_absolute_if_z_reset, &Cpu::store_to_pc, 3 };
    opcode_table[0xC3] = { &Cpu::fetch_from_immediate_u16, &Cpu::jump_absolute, &Cpu::store_to_pc, 4 };
    //opcode_table[0xC4] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xC5] = { &Cpu::fetch_from_bc, &Cpu::ld_16bit, &Cpu::push, 4 };
    opcode_table[0xC6] = { &Cpu::fetch_from_immediate_u8, &Cpu::add_to_a, &Cpu::store_to_a, 2 };
    //opcode_table[0xC7] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xC8] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xC9] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xCA] = { &Cpu::fetch_from_immediate_u16, &Cpu::jump_absolute_if_z_set, &Cpu::store_to_pc, 3 };
    //opcode_table[0xCB] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xCC] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xCD] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xCE] = { &Cpu::fetch_from_immediate_u8, &Cpu::add_to_a_with_carry, &Cpu::store_to_a, 2 };
    //opcode_table[0xCF] = { &Cpu::, &Cpu::, &Cpu::,  };

    //opcode_table[0xD0] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xD1] = { &Cpu::pop, &Cpu::ld_16bit, &Cpu::store_to_de, 3 };
    opcode_table[0xD2] = { &Cpu::fetch_from_immediate_u16, &Cpu::jump_absolute_if_c_reset, &Cpu::store_to_pc, 3 };
    //opcode_table[0xD3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xD4] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xD5] = { &Cpu::fetch_from_de, &Cpu::ld_16bit, &Cpu::push, 4 };
    opcode_table[0xD6] = { &Cpu::fetch_from_immediate_u8, &Cpu::subtract_from_a, &Cpu::store_to_a, 2 };
    //opcode_table[0xD7] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xD8] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xD9] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xDA] = { &Cpu::fetch_from_immediate_u16, &Cpu::jump_absolute_if_c_set, &Cpu::store_to_pc, 3 };
    //opcode_table[0xDB] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xDC] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xDD] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xDE] = { &Cpu::fetch_from_immediate_u8, &Cpu::subtract_from_a_with_carry, &Cpu::store_to_a, 2 };
    //opcode_table[0xDF] = { &Cpu::, &Cpu::, &Cpu::,  };

    opcode_table[0xE0] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_direct_ff_offset, 3 };
    opcode_table[0xE1] = { &Cpu::pop, &Cpu::ld_16bit, &Cpu::store_to_hl, 3 };
    opcode_table[0xE2] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_indirect_ff_with_c_offset, 2 };
    //opcode_table[0xE3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xE4] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xE5] = { &Cpu::fetch_from_hl, &Cpu::ld_16bit, &Cpu::push, 4 };
    opcode_table[0xE6] = { &Cpu::fetch_from_immediate_u8, &Cpu::and_with_a, &Cpu::store_to_a, 2 };
    //opcode_table[0xE7] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xE8] = { &Cpu::fetch_from_immediate_u8, &Cpu::add_to_sp, &Cpu::store_to_sp, 4 };
    opcode_table[0xE9] = { &Cpu::fetch_from_hl, &Cpu::jump_absolute, &Cpu::store_to_pc, 1 };
    opcode_table[0xEA] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_direct_8bit, 4 };
    //opcode_table[0xEB] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xEC] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xED] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xEE] = { &Cpu::fetch_from_immediate_u8, &Cpu::xor_with_a, &Cpu::store_to_a, 2 };
    //opcode_table[0xEF] = { &Cpu::, &Cpu::, &Cpu::,  };

    opcode_table[0xF0] = { &Cpu::fetch_direct_ff_offset, &Cpu::ld_8bit, &Cpu::store_to_a, 3 };
    opcode_table[0xF1] = { &Cpu::pop, &Cpu::ld_16bit, &Cpu::store_to_af, 3 };
    opcode_table[0xF2] = { &Cpu::fetch_indirect_ff_with_c_offset, &Cpu::ld_8bit, &Cpu::store_to_a, 2 };
    //opcode_table[0xF3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xF4] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xF5] = { &Cpu::fetch_from_af, &Cpu::ld_16bit, &Cpu::push, 4 };
    opcode_table[0xF6] = { &Cpu::fetch_from_immediate_u8, &Cpu::or_with_a, &Cpu::store_to_a, 2 };
    //opcode_table[0xF7] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xF8] = { &Cpu::fetch_from_adjusted_sp, &Cpu::ld_16bit, &Cpu::store_to_hl, 3 };
    opcode_table[0xF9] = { &Cpu::fetch_from_hl, &Cpu::ld_16bit, &Cpu::store_to_sp, 2 };
    opcode_table[0xFA] = { &Cpu::fetch_direct, &Cpu::ld_8bit, &Cpu::store_to_a, 4 };
    //opcode_table[0xFB] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xFC] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xFD] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xFE] = { &Cpu::fetch_from_immediate_u8, &Cpu::subtract_from_a, &Cpu::store_nop, 2 };
    //opcode_table[0xFF] = { &Cpu::, &Cpu::, &Cpu::,  };

    prefixed_opcode_table[0x00] = { &Cpu::fetch_from_b, &Cpu::rotate_left, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0x01] = { &Cpu::fetch_from_c, &Cpu::rotate_left, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0x02] = { &Cpu::fetch_from_d, &Cpu::rotate_left, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0x03] = { &Cpu::fetch_from_e, &Cpu::rotate_left, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0x04] = { &Cpu::fetch_from_h, &Cpu::rotate_left, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0x05] = { &Cpu::fetch_from_l, &Cpu::rotate_left, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0x06] = { &Cpu::fetch_indirect_hl, &Cpu::rotate_left, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0x07] = { &Cpu::fetch_from_a, &Cpu::rotate_left, &Cpu::store_to_a, 2 };
    prefixed_opcode_table[0x08] = { &Cpu::fetch_from_b, &Cpu::rotate_right, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0x09] = { &Cpu::fetch_from_c, &Cpu::rotate_right, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0x0A] = { &Cpu::fetch_from_d, &Cpu::rotate_right, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0x0B] = { &Cpu::fetch_from_e, &Cpu::rotate_right, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0x0C] = { &Cpu::fetch_from_h, &Cpu::rotate_right, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0x0D] = { &Cpu::fetch_from_l, &Cpu::rotate_right, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0x0E] = { &Cpu::fetch_indirect_hl, &Cpu::rotate_right, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0x0F] = { &Cpu::fetch_from_a, &Cpu::rotate_right, &Cpu::store_to_a, 2 };

    prefixed_opcode_table[0x10] = { &Cpu::fetch_from_b, &Cpu::rotate_left_with_carry, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0x11] = { &Cpu::fetch_from_c, &Cpu::rotate_left_with_carry, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0x12] = { &Cpu::fetch_from_d, &Cpu::rotate_left_with_carry, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0x13] = { &Cpu::fetch_from_e, &Cpu::rotate_left_with_carry, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0x14] = { &Cpu::fetch_from_h, &Cpu::rotate_left_with_carry, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0x15] = { &Cpu::fetch_from_l, &Cpu::rotate_left_with_carry, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0x16] = { &Cpu::fetch_indirect_hl, &Cpu::rotate_left_with_carry, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0x17] = { &Cpu::fetch_from_a, &Cpu::rotate_left_with_carry, &Cpu::store_to_a, 2 };
    prefixed_opcode_table[0x18] = { &Cpu::fetch_from_b, &Cpu::rotate_right_with_carry, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0x19] = { &Cpu::fetch_from_c, &Cpu::rotate_right_with_carry, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0x1A] = { &Cpu::fetch_from_d, &Cpu::rotate_right_with_carry, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0x1B] = { &Cpu::fetch_from_e, &Cpu::rotate_right_with_carry, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0x1C] = { &Cpu::fetch_from_h, &Cpu::rotate_right_with_carry, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0x1D] = { &Cpu::fetch_from_l, &Cpu::rotate_right_with_carry, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0x1E] = { &Cpu::fetch_indirect_hl, &Cpu::rotate_right_with_carry, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0x1F] = { &Cpu::fetch_from_a, &Cpu::rotate_right_with_carry, &Cpu::store_to_a, 2 };

    prefixed_opcode_table[0x20] = { &Cpu::fetch_from_b, &Cpu::shift_left_arithmetically, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0x21] = { &Cpu::fetch_from_c, &Cpu::shift_left_arithmetically, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0x22] = { &Cpu::fetch_from_d, &Cpu::shift_left_arithmetically, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0x23] = { &Cpu::fetch_from_e, &Cpu::shift_left_arithmetically, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0x24] = { &Cpu::fetch_from_h, &Cpu::shift_left_arithmetically, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0x25] = { &Cpu::fetch_from_l, &Cpu::shift_left_arithmetically, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0x26] = { &Cpu::fetch_indirect_hl, &Cpu::shift_left_arithmetically, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0x27] = { &Cpu::fetch_from_a, &Cpu::shift_left_arithmetically, &Cpu::store_to_a, 2 };
    prefixed_opcode_table[0x28] = { &Cpu::fetch_from_b, &Cpu::shift_right_arithmetically, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0x29] = { &Cpu::fetch_from_c, &Cpu::shift_right_arithmetically, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0x2A] = { &Cpu::fetch_from_d, &Cpu::shift_right_arithmetically, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0x2B] = { &Cpu::fetch_from_e, &Cpu::shift_right_arithmetically, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0x2C] = { &Cpu::fetch_from_h, &Cpu::shift_right_arithmetically, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0x2D] = { &Cpu::fetch_from_l, &Cpu::shift_right_arithmetically, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0x2E] = { &Cpu::fetch_indirect_hl, &Cpu::shift_right_arithmetically, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0x2F] = { &Cpu::fetch_from_a, &Cpu::shift_right_arithmetically, &Cpu::store_to_a, 2 };

    prefixed_opcode_table[0x30] = { &Cpu::fetch_from_b, &Cpu::swap, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0x31] = { &Cpu::fetch_from_c, &Cpu::swap, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0x32] = { &Cpu::fetch_from_d, &Cpu::swap, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0x33] = { &Cpu::fetch_from_e, &Cpu::swap, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0x34] = { &Cpu::fetch_from_h, &Cpu::swap, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0x35] = { &Cpu::fetch_from_l, &Cpu::swap, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0x36] = { &Cpu::fetch_indirect_hl, &Cpu::swap, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0x37] = { &Cpu::fetch_from_a, &Cpu::swap, &Cpu::store_to_a, 2 };
    prefixed_opcode_table[0x38] = { &Cpu::fetch_from_b, &Cpu::shift_right_logically, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0x39] = { &Cpu::fetch_from_c, &Cpu::shift_right_logically, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0x3A] = { &Cpu::fetch_from_d, &Cpu::shift_right_logically, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0x3B] = { &Cpu::fetch_from_e, &Cpu::shift_right_logically, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0x3C] = { &Cpu::fetch_from_h, &Cpu::shift_right_logically, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0x3D] = { &Cpu::fetch_from_l, &Cpu::shift_right_logically, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0x3E] = { &Cpu::fetch_indirect_hl, &Cpu::shift_right_logically, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0x3F] = { &Cpu::fetch_from_a, &Cpu::shift_right_logically, &Cpu::store_to_a, 2 };

    prefixed_opcode_table[0x40] = { &Cpu::fetch_from_b, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x41] = { &Cpu::fetch_from_c, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x42] = { &Cpu::fetch_from_d, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x43] = { &Cpu::fetch_from_e, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x44] = { &Cpu::fetch_from_h, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x45] = { &Cpu::fetch_from_l, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x46] = { &Cpu::fetch_indirect_hl, &Cpu::test_bit, &Cpu::store_nop, 3 };
    prefixed_opcode_table[0x47] = { &Cpu::fetch_from_a, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x48] = { &Cpu::fetch_from_b, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x49] = { &Cpu::fetch_from_c, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x4A] = { &Cpu::fetch_from_d, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x4B] = { &Cpu::fetch_from_e, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x4C] = { &Cpu::fetch_from_h, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x4D] = { &Cpu::fetch_from_l, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x4E] = { &Cpu::fetch_indirect_hl, &Cpu::test_bit, &Cpu::store_nop, 3 };
    prefixed_opcode_table[0x4F] = { &Cpu::fetch_from_a, &Cpu::test_bit, &Cpu::store_nop, 2 };

    prefixed_opcode_table[0x50] = { &Cpu::fetch_from_b, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x51] = { &Cpu::fetch_from_c, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x52] = { &Cpu::fetch_from_d, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x53] = { &Cpu::fetch_from_e, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x54] = { &Cpu::fetch_from_h, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x55] = { &Cpu::fetch_from_l, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x56] = { &Cpu::fetch_indirect_hl, &Cpu::test_bit, &Cpu::store_nop, 3 };
    prefixed_opcode_table[0x57] = { &Cpu::fetch_from_a, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x58] = { &Cpu::fetch_from_b, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x59] = { &Cpu::fetch_from_c, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x5A] = { &Cpu::fetch_from_d, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x5B] = { &Cpu::fetch_from_e, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x5C] = { &Cpu::fetch_from_h, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x5D] = { &Cpu::fetch_from_l, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x5E] = { &Cpu::fetch_indirect_hl, &Cpu::test_bit, &Cpu::store_nop, 3 };
    prefixed_opcode_table[0x5F] = { &Cpu::fetch_from_a, &Cpu::test_bit, &Cpu::store_nop, 2 };

    prefixed_opcode_table[0x60] = { &Cpu::fetch_from_b, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x61] = { &Cpu::fetch_from_c, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x62] = { &Cpu::fetch_from_d, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x63] = { &Cpu::fetch_from_e, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x64] = { &Cpu::fetch_from_h, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x65] = { &Cpu::fetch_from_l, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x66] = { &Cpu::fetch_indirect_hl, &Cpu::test_bit, &Cpu::store_nop, 3 };
    prefixed_opcode_table[0x67] = { &Cpu::fetch_from_a, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x68] = { &Cpu::fetch_from_b, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x69] = { &Cpu::fetch_from_c, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x6A] = { &Cpu::fetch_from_d, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x6B] = { &Cpu::fetch_from_e, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x6C] = { &Cpu::fetch_from_h, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x6D] = { &Cpu::fetch_from_l, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x6E] = { &Cpu::fetch_indirect_hl, &Cpu::test_bit, &Cpu::store_nop, 3 };
    prefixed_opcode_table[0x6F] = { &Cpu::fetch_from_a, &Cpu::test_bit, &Cpu::store_nop, 2 };

    prefixed_opcode_table[0x70] = { &Cpu::fetch_from_b, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x71] = { &Cpu::fetch_from_c, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x72] = { &Cpu::fetch_from_d, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x73] = { &Cpu::fetch_from_e, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x74] = { &Cpu::fetch_from_h, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x75] = { &Cpu::fetch_from_l, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x76] = { &Cpu::fetch_indirect_hl, &Cpu::test_bit, &Cpu::store_nop, 3 };
    prefixed_opcode_table[0x77] = { &Cpu::fetch_from_a, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x78] = { &Cpu::fetch_from_b, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x79] = { &Cpu::fetch_from_c, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x7A] = { &Cpu::fetch_from_d, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x7B] = { &Cpu::fetch_from_e, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x7C] = { &Cpu::fetch_from_h, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x7D] = { &Cpu::fetch_from_l, &Cpu::test_bit, &Cpu::store_nop, 2 };
    prefixed_opcode_table[0x7E] = { &Cpu::fetch_indirect_hl, &Cpu::test_bit, &Cpu::store_nop, 3 };
    prefixed_opcode_table[0x7F] = { &Cpu::fetch_from_a, &Cpu::test_bit, &Cpu::store_nop, 2 };

    prefixed_opcode_table[0x80] = { &Cpu::fetch_from_b, &Cpu::reset_bit, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0x81] = { &Cpu::fetch_from_c, &Cpu::reset_bit, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0x82] = { &Cpu::fetch_from_d, &Cpu::reset_bit, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0x83] = { &Cpu::fetch_from_e, &Cpu::reset_bit, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0x84] = { &Cpu::fetch_from_h, &Cpu::reset_bit, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0x85] = { &Cpu::fetch_from_l, &Cpu::reset_bit, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0x86] = { &Cpu::fetch_indirect_hl, &Cpu::reset_bit, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0x87] = { &Cpu::fetch_from_a, &Cpu::reset_bit, &Cpu::store_to_a, 2 };
    prefixed_opcode_table[0x88] = { &Cpu::fetch_from_b, &Cpu::reset_bit, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0x89] = { &Cpu::fetch_from_c, &Cpu::reset_bit, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0x8A] = { &Cpu::fetch_from_d, &Cpu::reset_bit, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0x8B] = { &Cpu::fetch_from_e, &Cpu::reset_bit, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0x8C] = { &Cpu::fetch_from_h, &Cpu::reset_bit, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0x8D] = { &Cpu::fetch_from_l, &Cpu::reset_bit, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0x8E] = { &Cpu::fetch_indirect_hl, &Cpu::reset_bit, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0x8F] = { &Cpu::fetch_from_a, &Cpu::reset_bit, &Cpu::store_to_a, 2 };

    prefixed_opcode_table[0x90] = { &Cpu::fetch_from_b, &Cpu::reset_bit, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0x91] = { &Cpu::fetch_from_c, &Cpu::reset_bit, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0x92] = { &Cpu::fetch_from_d, &Cpu::reset_bit, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0x93] = { &Cpu::fetch_from_e, &Cpu::reset_bit, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0x94] = { &Cpu::fetch_from_h, &Cpu::reset_bit, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0x95] = { &Cpu::fetch_from_l, &Cpu::reset_bit, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0x96] = { &Cpu::fetch_indirect_hl, &Cpu::reset_bit, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0x97] = { &Cpu::fetch_from_a, &Cpu::reset_bit, &Cpu::store_to_a, 2 };
    prefixed_opcode_table[0x98] = { &Cpu::fetch_from_b, &Cpu::reset_bit, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0x99] = { &Cpu::fetch_from_c, &Cpu::reset_bit, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0x9A] = { &Cpu::fetch_from_d, &Cpu::reset_bit, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0x9B] = { &Cpu::fetch_from_e, &Cpu::reset_bit, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0x9C] = { &Cpu::fetch_from_h, &Cpu::reset_bit, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0x9D] = { &Cpu::fetch_from_l, &Cpu::reset_bit, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0x9E] = { &Cpu::fetch_indirect_hl, &Cpu::reset_bit, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0x9F] = { &Cpu::fetch_from_a, &Cpu::reset_bit, &Cpu::store_to_a, 2 };

    prefixed_opcode_table[0xA0] = { &Cpu::fetch_from_b, &Cpu::reset_bit, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0xA1] = { &Cpu::fetch_from_c, &Cpu::reset_bit, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0xA2] = { &Cpu::fetch_from_d, &Cpu::reset_bit, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0xA3] = { &Cpu::fetch_from_e, &Cpu::reset_bit, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0xA4] = { &Cpu::fetch_from_h, &Cpu::reset_bit, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0xA5] = { &Cpu::fetch_from_l, &Cpu::reset_bit, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0xA6] = { &Cpu::fetch_indirect_hl, &Cpu::reset_bit, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0xA7] = { &Cpu::fetch_from_a, &Cpu::reset_bit, &Cpu::store_to_a, 2 };
    prefixed_opcode_table[0xA8] = { &Cpu::fetch_from_b, &Cpu::reset_bit, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0xA9] = { &Cpu::fetch_from_c, &Cpu::reset_bit, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0xAA] = { &Cpu::fetch_from_d, &Cpu::reset_bit, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0xAB] = { &Cpu::fetch_from_e, &Cpu::reset_bit, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0xAC] = { &Cpu::fetch_from_h, &Cpu::reset_bit, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0xAD] = { &Cpu::fetch_from_l, &Cpu::reset_bit, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0xAE] = { &Cpu::fetch_indirect_hl, &Cpu::reset_bit, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0xAF] = { &Cpu::fetch_from_a, &Cpu::reset_bit, &Cpu::store_to_a, 2 };

    prefixed_opcode_table[0xB0] = { &Cpu::fetch_from_b, &Cpu::reset_bit, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0xB1] = { &Cpu::fetch_from_c, &Cpu::reset_bit, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0xB2] = { &Cpu::fetch_from_d, &Cpu::reset_bit, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0xB3] = { &Cpu::fetch_from_e, &Cpu::reset_bit, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0xB4] = { &Cpu::fetch_from_h, &Cpu::reset_bit, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0xB5] = { &Cpu::fetch_from_l, &Cpu::reset_bit, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0xB6] = { &Cpu::fetch_indirect_hl, &Cpu::reset_bit, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0xB7] = { &Cpu::fetch_from_a, &Cpu::reset_bit, &Cpu::store_to_a, 2 };
    prefixed_opcode_table[0xB8] = { &Cpu::fetch_from_b, &Cpu::reset_bit, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0xB9] = { &Cpu::fetch_from_c, &Cpu::reset_bit, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0xBA] = { &Cpu::fetch_from_d, &Cpu::reset_bit, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0xBB] = { &Cpu::fetch_from_e, &Cpu::reset_bit, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0xBC] = { &Cpu::fetch_from_h, &Cpu::reset_bit, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0xBD] = { &Cpu::fetch_from_l, &Cpu::reset_bit, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0xBE] = { &Cpu::fetch_indirect_hl, &Cpu::reset_bit, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0xBF] = { &Cpu::fetch_from_a, &Cpu::reset_bit, &Cpu::store_to_a, 2 };

    prefixed_opcode_table[0xC0] = { &Cpu::fetch_from_b, &Cpu::set_bit, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0xC1] = { &Cpu::fetch_from_c, &Cpu::set_bit, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0xC2] = { &Cpu::fetch_from_d, &Cpu::set_bit, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0xC3] = { &Cpu::fetch_from_e, &Cpu::set_bit, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0xC4] = { &Cpu::fetch_from_h, &Cpu::set_bit, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0xC5] = { &Cpu::fetch_from_l, &Cpu::set_bit, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0xC6] = { &Cpu::fetch_indirect_hl, &Cpu::set_bit, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0xC7] = { &Cpu::fetch_from_a, &Cpu::set_bit, &Cpu::store_to_a, 2 };
    prefixed_opcode_table[0xC8] = { &Cpu::fetch_from_b, &Cpu::set_bit, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0xC9] = { &Cpu::fetch_from_c, &Cpu::set_bit, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0xCA] = { &Cpu::fetch_from_d, &Cpu::set_bit, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0xCB] = { &Cpu::fetch_from_e, &Cpu::set_bit, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0xCC] = { &Cpu::fetch_from_h, &Cpu::set_bit, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0xCD] = { &Cpu::fetch_from_l, &Cpu::set_bit, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0xCE] = { &Cpu::fetch_indirect_hl, &Cpu::set_bit, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0xCF] = { &Cpu::fetch_from_a, &Cpu::set_bit, &Cpu::store_to_a, 2 };

    prefixed_opcode_table[0xD0] = { &Cpu::fetch_from_b, &Cpu::set_bit, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0xD1] = { &Cpu::fetch_from_c, &Cpu::set_bit, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0xD2] = { &Cpu::fetch_from_d, &Cpu::set_bit, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0xD3] = { &Cpu::fetch_from_e, &Cpu::set_bit, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0xD4] = { &Cpu::fetch_from_h, &Cpu::set_bit, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0xD5] = { &Cpu::fetch_from_l, &Cpu::set_bit, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0xD6] = { &Cpu::fetch_indirect_hl, &Cpu::set_bit, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0xD7] = { &Cpu::fetch_from_a, &Cpu::set_bit, &Cpu::store_to_a, 2 };
    prefixed_opcode_table[0xD8] = { &Cpu::fetch_from_b, &Cpu::set_bit, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0xD9] = { &Cpu::fetch_from_c, &Cpu::set_bit, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0xDA] = { &Cpu::fetch_from_d, &Cpu::set_bit, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0xDB] = { &Cpu::fetch_from_e, &Cpu::set_bit, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0xDC] = { &Cpu::fetch_from_h, &Cpu::set_bit, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0xDD] = { &Cpu::fetch_from_l, &Cpu::set_bit, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0xDE] = { &Cpu::fetch_indirect_hl, &Cpu::set_bit, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0xDF] = { &Cpu::fetch_from_a, &Cpu::set_bit, &Cpu::store_to_a, 2 };

    prefixed_opcode_table[0xE0] = { &Cpu::fetch_from_b, &Cpu::set_bit, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0xE1] = { &Cpu::fetch_from_c, &Cpu::set_bit, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0xE2] = { &Cpu::fetch_from_d, &Cpu::set_bit, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0xE3] = { &Cpu::fetch_from_e, &Cpu::set_bit, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0xE4] = { &Cpu::fetch_from_h, &Cpu::set_bit, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0xE5] = { &Cpu::fetch_from_l, &Cpu::set_bit, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0xE6] = { &Cpu::fetch_indirect_hl, &Cpu::set_bit, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0xE7] = { &Cpu::fetch_from_a, &Cpu::set_bit, &Cpu::store_to_a, 2 };
    prefixed_opcode_table[0xE8] = { &Cpu::fetch_from_b, &Cpu::set_bit, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0xE9] = { &Cpu::fetch_from_c, &Cpu::set_bit, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0xEA] = { &Cpu::fetch_from_d, &Cpu::set_bit, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0xEB] = { &Cpu::fetch_from_e, &Cpu::set_bit, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0xEC] = { &Cpu::fetch_from_h, &Cpu::set_bit, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0xED] = { &Cpu::fetch_from_l, &Cpu::set_bit, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0xEE] = { &Cpu::fetch_indirect_hl, &Cpu::set_bit, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0xEF] = { &Cpu::fetch_from_a, &Cpu::set_bit, &Cpu::store_to_a, 2 };

    prefixed_opcode_table[0xF0] = { &Cpu::fetch_from_b, &Cpu::set_bit, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0xF1] = { &Cpu::fetch_from_c, &Cpu::set_bit, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0xF2] = { &Cpu::fetch_from_d, &Cpu::set_bit, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0xF3] = { &Cpu::fetch_from_e, &Cpu::set_bit, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0xF4] = { &Cpu::fetch_from_h, &Cpu::set_bit, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0xF5] = { &Cpu::fetch_from_l, &Cpu::set_bit, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0xF6] = { &Cpu::fetch_indirect_hl, &Cpu::set_bit, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0xF7] = { &Cpu::fetch_from_a, &Cpu::set_bit, &Cpu::store_to_a, 2 };
    prefixed_opcode_table[0xF8] = { &Cpu::fetch_from_b, &Cpu::set_bit, &Cpu::store_to_b, 2 };
    prefixed_opcode_table[0xF9] = { &Cpu::fetch_from_c, &Cpu::set_bit, &Cpu::store_to_c, 2 };
    prefixed_opcode_table[0xFA] = { &Cpu::fetch_from_d, &Cpu::set_bit, &Cpu::store_to_d, 2 };
    prefixed_opcode_table[0xFB] = { &Cpu::fetch_from_e, &Cpu::set_bit, &Cpu::store_to_e, 2 };
    prefixed_opcode_table[0xFC] = { &Cpu::fetch_from_h, &Cpu::set_bit, &Cpu::store_to_h, 2 };
    prefixed_opcode_table[0xFD] = { &Cpu::fetch_from_l, &Cpu::set_bit, &Cpu::store_to_l, 2 };
    prefixed_opcode_table[0xFE] = { &Cpu::fetch_indirect_hl, &Cpu::set_bit, &Cpu::store_indirect_hl, 4 };
    prefixed_opcode_table[0xFF] = { &Cpu::fetch_from_a, &Cpu::set_bit, &Cpu::store_to_a, 2 };
}

// ----------------------------------------------------------------------------

bool Cpu::get_flag(FLAGS flag) {
    return (flags & flag) != 0;
}

// ----------------------------------------------------------------------------

void Cpu::set_flag(FLAGS flag, bool value) {
    if (value) {
        flags |= flag;
    } else {
        flags &= ~flag;
    }
}

// ----------------------------------------------------------------------------

void Cpu::set_target_bit(Uint8 prefixed_opcode_byte) {
    target_bit = ((prefixed_opcode_byte & 0x30) >> 4) << 1;
    if (prefixed_opcode_byte & 0x08) {
        target_bit++;
    }
}

// ----------------------------------------------------------------------------

void Cpu::clock() {
    if (current_instruction_cycles_remaining <= 0) {
        Uint8 opcode_byte = bus->cpu_read(pc++);

        Instruction opcode;
        is_current_opcode_prefixed = opcode_byte == 0xCB;
        if (is_current_opcode_prefixed) {
            opcode_byte = bus->cpu_read(pc++);
            opcode = prefixed_opcode_table[opcode_byte];

            set_target_bit(opcode_byte);
        } else {
            opcode = opcode_table[opcode_byte];
        }

        current_instruction_cycles_remaining = opcode.base_cycles;

        (this->*opcode.fetch)();
        current_instruction_cycles_remaining += (this->*opcode.execute)();
        (this->*opcode.store)();
    }

    current_instruction_cycles_remaining--;
}

// ----------------------------------------------------------------------------

bool Cpu::is_current_instruction_finished() {
    return current_instruction_cycles_remaining <= 0;
}

// ----------------------------------------------------------------------------

void Cpu::reset() {
    a = 0x00;
    b = 0x00;
    c = 0x00;
    d = 0x00;
    e = 0x00;
    h = 0x00;
    l = 0x00;
    flags = 0x00;
    pc = 0x0100;
    sp = 0xFFFE;
}

// ----------------------------------------------------------------------------

Cpu_Info Cpu::get_cpu_info() {
    return {
        a,
        b,
        c,
        d,
        e,
        h,
        l,
        pc,
        sp,
        get_flag(Z_FLAG),
        get_flag(N_FLAG),
        get_flag(H_FLAG),
        get_flag(C_FLAG),
    };
}

// ----------------------------------------------------------------------------

void Cpu::fetch_nop() {
    // intentionally do nothing
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_a() {
    fetched_u8 = a;
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_b() {
    fetched_u8 = b;
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_c() {
    fetched_u8 = c;
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_d() {
    fetched_u8 = d;
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_e() {
    fetched_u8 = e;
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_h() {
    fetched_u8 = h;
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_l() {
    fetched_u8 = l;
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_bc() {
    fetched_u16_msb = b;
    fetched_u16_lsb = c;
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_de() {
    fetched_u16_msb = d;
    fetched_u16_lsb = e;
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_hl() {
    fetched_u16_msb = h;
    fetched_u16_lsb = l;
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_af() {
    fetched_u16_msb = a;
    fetched_u16_lsb = flags;
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_sp() {
    split_u16(sp, fetched_u16_msb, fetched_u16_lsb);
}

// ----------------------------------------------------------------------------

void Cpu::fetch_indirect_hl() {
    fetched_u8 = bus->cpu_read(get_hl());
}

// ----------------------------------------------------------------------------

void Cpu::fetch_indirect_hl_plus() {
    fetched_u8 = bus->cpu_read(get_hl());

    Uint16 new_hl = get_hl() + 0x01;
    split_u16(new_hl, h, l);
}

// ----------------------------------------------------------------------------

void Cpu::fetch_indirect_hl_minus() {
    fetched_u8 = bus->cpu_read(get_hl());

    Uint16 new_hl = get_hl() - 0x01;
    split_u16(new_hl, h, l);
}

// ----------------------------------------------------------------------------

void Cpu::fetch_indirect_bc() {
    fetched_u8 = bus->cpu_read(get_bc());
}

// ----------------------------------------------------------------------------

void Cpu::fetch_indirect_de() {
    fetched_u8 = bus->cpu_read(get_de());
}

// ----------------------------------------------------------------------------

void Cpu::fetch_direct_ff_offset() {
    Uint16 source_address = 0xFF00 + bus->cpu_read(pc++);
    fetched_u8 = bus->cpu_read(source_address);
}

// ----------------------------------------------------------------------------

void Cpu::fetch_indirect_ff_with_c_offset() {
    Uint16 source_address = 0xFF00 + c;
    fetched_u8 = bus->cpu_read(source_address);
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_immediate_u8() {
    fetched_u8 = bus->cpu_read(pc++);
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_immediate_u16() {
    fetched_u16_lsb = bus->cpu_read(pc++);
    fetched_u16_msb = bus->cpu_read(pc++);
}

// ----------------------------------------------------------------------------

void Cpu::fetch_direct() {
    Uint16 least_significant_address_byte = bus->cpu_read(pc++);
    Uint16 most_significant_address_byte = bus->cpu_read(pc++);
    Uint16 destination_address = join_to_u16(most_significant_address_byte, least_significant_address_byte);
    fetched_u8 = bus->cpu_read(destination_address);
}

// ----------------------------------------------------------------------------

void Cpu::pop() {
    fetched_u16_lsb = bus->cpu_read(sp++);
    fetched_u16_msb = bus->cpu_read(sp++);
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_adjusted_sp() {
    Uint8 offset_byte = bus->cpu_read(pc++);
    Uint16 sp_offset = static_cast<Sint8>(offset_byte);

    bool half_carry_8bit;
    bool carry_8bit;
    full_add_8bit(sp, sp_offset, 0, half_carry_8bit, carry_8bit);

    bool half_carry;
    bool carry;
    Uint32 result = full_add_16bit(sp, sp_offset, 0, half_carry, carry);

    split_u16(result, fetched_u16_msb, fetched_u16_lsb);
    set_flag(Z_FLAG, false);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, half_carry_8bit);
    set_flag(C_FLAG, carry_8bit);
}

// ----------------------------------------------------------------------------

int Cpu::execute_nop() {
    // intentionally do nothing
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::ld_8bit() {
    computed_u8 = fetched_u8;
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::ld_16bit() {
    computed_u16_msb = fetched_u16_msb;
    computed_u16_lsb = fetched_u16_lsb;
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::and_with_a() {
    computed_u8 = a & fetched_u8;
    set_flag(Z_FLAG, computed_u8 == 0);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, true);
    set_flag(C_FLAG, false);
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::xor_with_a() {
    computed_u8 = a ^ fetched_u8;
    set_flag(Z_FLAG, computed_u8 == 0);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, false);
    set_flag(C_FLAG, false);
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::or_with_a() {
    computed_u8 = a | fetched_u8;
    set_flag(Z_FLAG, computed_u8 == 0);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, false);
    set_flag(C_FLAG, false);
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::inc_8bit() {
    bool half_carry;
    bool carry;
    Uint32 result = full_add_8bit(fetched_u8, 1, 0, half_carry, carry);

    computed_u8 = result & 0x000000FF;
    set_flag(Z_FLAG, computed_u8 == 0);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, half_carry);
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::inc_16bit() {
    Uint16 computed_u16 = get_fetched_u16() + 1;
    split_u16(computed_u16, computed_u16_msb, computed_u16_lsb);
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::dec_8bit() {
    bool half_carry;
    bool carry;
    Uint32 result = full_sub_8bit(fetched_u8, 1, 0, half_carry, carry);

    computed_u8 = result & 0x000000FF;
    set_flag(Z_FLAG, computed_u8 == 0);
    set_flag(N_FLAG, true);
    set_flag(H_FLAG, half_carry);
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::dec_16bit() {
    Uint16 computed_u16 = get_fetched_u16() - 1;
    split_u16(computed_u16, computed_u16_msb, computed_u16_lsb);
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::add_to_a() {
    bool half_carry;
    bool carry;
    Uint32 result = full_add_8bit(fetched_u8, a, 0, half_carry, carry);

    computed_u8 = result & 0x000000FF;
    set_flag(Z_FLAG, computed_u8 == 0);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, half_carry);
    set_flag(C_FLAG, carry);
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::add_to_a_with_carry() {
    bool half_carry;
    bool carry;
    Uint32 result = full_add_8bit(fetched_u8, a, get_flag(C_FLAG), half_carry, carry);

    computed_u8 = result & 0x000000FF;
    set_flag(Z_FLAG, computed_u8 == 0);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, half_carry);
    set_flag(C_FLAG, carry);
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::add_to_hl() {
    bool half_carry;
    bool carry;
    Uint32 result = full_add_16bit(get_fetched_u16(), get_hl(), 0, half_carry, carry);

    split_u16(result, computed_u16_msb, computed_u16_lsb);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, half_carry);
    set_flag(C_FLAG, carry);
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::add_to_sp() {
    Uint16 sp_offset = static_cast<Sint8>(fetched_u8);

    bool half_carry_8bit;
    bool carry_8bit;
    full_add_8bit(sp, sp_offset, 0, half_carry_8bit, carry_8bit);

    bool half_carry;
    bool carry;
    Uint32 result = full_add_16bit(sp, sp_offset, 0, half_carry, carry);

    split_u16(result, computed_u16_msb, computed_u16_lsb);
    set_flag(Z_FLAG, false);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, half_carry_8bit);
    set_flag(C_FLAG, carry_8bit);
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::subtract_from_a() {
    bool half_carry;
    bool carry;
    Uint32 result = full_sub_8bit(a, fetched_u8, 0, half_carry, carry);

    computed_u8 = result & 0x000000FF;
    set_flag(Z_FLAG, computed_u8 == 0);
    set_flag(N_FLAG, true);
    set_flag(H_FLAG, half_carry);
    set_flag(C_FLAG, carry);
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::subtract_from_a_with_carry() {
    bool half_carry;
    bool carry;
    Uint32 result = full_sub_8bit(a, fetched_u8, get_flag(C_FLAG), half_carry, carry);

    computed_u8 = result & 0x000000FF;
    set_flag(Z_FLAG, computed_u8 == 0);
    set_flag(N_FLAG, true);
    set_flag(H_FLAG, half_carry);
    set_flag(C_FLAG, carry);
    return 0;
    //todo
}

// ----------------------------------------------------------------------------

int Cpu::jump_absolute() {
    computed_u16_msb = fetched_u16_msb;
    computed_u16_lsb = fetched_u16_lsb;
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::jump_absolute_if_z_reset() {
    int additional_cycle = 0;

    if (!get_flag(Z_FLAG)) {
        computed_u16_msb = fetched_u16_msb;
        computed_u16_lsb = fetched_u16_lsb;
        additional_cycle = 1;
    } else {
        split_u16(pc, computed_u16_msb, computed_u16_lsb);
    }

    return additional_cycle;
}

// ----------------------------------------------------------------------------

int Cpu::jump_absolute_if_c_reset() {
    int additional_cycle = 0;

    if (!get_flag(C_FLAG)) {
        computed_u16_msb = fetched_u16_msb;
        computed_u16_lsb = fetched_u16_lsb;
        additional_cycle = 1;
    } else {
        split_u16(pc, computed_u16_msb, computed_u16_lsb);
    }

    return additional_cycle;
}

// ----------------------------------------------------------------------------

int Cpu::jump_absolute_if_z_set() {
    int additional_cycle = 0;

    if (get_flag(Z_FLAG)) {
        computed_u16_msb = fetched_u16_msb;
        computed_u16_lsb = fetched_u16_lsb;
        additional_cycle = 1;
    } else {
        split_u16(pc, computed_u16_msb, computed_u16_lsb);
    }

    return additional_cycle;
}

// ----------------------------------------------------------------------------

int Cpu::jump_absolute_if_c_set() {
    int additional_cycle = 0;

    if (get_flag(C_FLAG)) {
        computed_u16_msb = fetched_u16_msb;
        computed_u16_lsb = fetched_u16_lsb;
        additional_cycle = 1;
    } else {
        split_u16(pc, computed_u16_msb, computed_u16_lsb);
    }

    return additional_cycle;
}

// ----------------------------------------------------------------------------

int Cpu::rotate_left() {
    Uint8 bit7 = (fetched_u8 & 0x80) == 0 ? 0 : 1;
    computed_u8 = (fetched_u8 << 1) | bit7;

    bool is_z_flag_set = is_current_opcode_prefixed && computed_u8 == 0;

    set_flag(Z_FLAG, is_z_flag_set);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, false);
    set_flag(C_FLAG, bit7 == 1);

    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::rotate_left_with_carry() {
    Uint8 bit7 = (fetched_u8 & 0x80) == 0 ? 0 : 1;
    computed_u8 = (fetched_u8 << 1) | (get_flag(C_FLAG) ? 1 : 0);

    bool is_z_flag_set = is_current_opcode_prefixed && computed_u8 == 0;

    set_flag(Z_FLAG, is_z_flag_set);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, false);
    set_flag(C_FLAG, bit7 == 1);

    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::rotate_right() {
    Uint8 bit0 = fetched_u8 & 0x01;
    computed_u8 = (fetched_u8 >> 1) | (bit0 << 7);

    bool is_z_flag_set = is_current_opcode_prefixed && computed_u8 == 0;

    set_flag(Z_FLAG, is_z_flag_set);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, false);
    set_flag(C_FLAG, bit0 == 1);

    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::rotate_right_with_carry() {
    Uint8 bit0 = fetched_u8 & 0x01;
    computed_u8 = (fetched_u8 >> 1) | ((get_flag(C_FLAG) ? 1 : 0) << 7);

    bool is_z_flag_set = is_current_opcode_prefixed && computed_u8 == 0;

    set_flag(Z_FLAG, is_z_flag_set);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, false);
    set_flag(C_FLAG, bit0 == 1);

    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::shift_left_arithmetically() {
    Uint8 bit7 = (fetched_u8 & 0x80) == 0 ? 0 : 1;
    computed_u8 = (fetched_u8 << 1) & 0xFE;

    set_flag(Z_FLAG, computed_u8 == 0);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, false);
    set_flag(C_FLAG, bit7 == 1);

    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::shift_right_arithmetically() {
    Uint8 preserved_bit7 = fetched_u8 & 0x80;
    Uint8 bit0 = fetched_u8 & 0x01;
    computed_u8 = ((fetched_u8 >> 1) & 0x7F) | preserved_bit7;

    set_flag(Z_FLAG, computed_u8 == 0);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, false);
    set_flag(C_FLAG, bit0 == 1);

    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::shift_right_logically() {
    Uint8 bit0 = fetched_u8 & 0x01;
    computed_u8 = (fetched_u8 >> 1) & 0x7F;

    set_flag(Z_FLAG, computed_u8 == 0);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, false);
    set_flag(C_FLAG, bit0 == 1);

    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::swap() {
    Uint8 lsn = fetched_u8 & 0x0F;
    computed_u8 = ((fetched_u8 >> 4) & 0x0F) | (lsn << 4);

    set_flag(Z_FLAG, computed_u8 == 0);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, false);
    set_flag(C_FLAG, false);

    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::test_bit() {
    set_flag(Z_FLAG, (fetched_u8 & (1 << target_bit)) == 0);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, true);

    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::reset_bit() {
    computed_u8 = fetched_u8 & ~(1 << target_bit);
    return 0;
}

// ----------------------------------------------------------------------------

int Cpu::set_bit() {
    computed_u8 = fetched_u8 | (1 << target_bit);
    return 0;
}

// ----------------------------------------------------------------------------

void Cpu::store_nop() {
    // intentionally do nothing
}

// ----------------------------------------------------------------------------

void Cpu::store_to_a() {
    a = computed_u8;
}

// ----------------------------------------------------------------------------

void Cpu::store_to_b() {
    b = computed_u8;
}

// ----------------------------------------------------------------------------

void Cpu::store_to_c() {
    c = computed_u8;
}

// ----------------------------------------------------------------------------

void Cpu::store_to_d() {
    d = computed_u8;
}

// ----------------------------------------------------------------------------

void Cpu::store_to_e() {
    e = computed_u8;
}

// ----------------------------------------------------------------------------

void Cpu::store_to_h() {
    h = computed_u8;
}

// ----------------------------------------------------------------------------

void Cpu::store_to_l() {
    l = computed_u8;
}

// ----------------------------------------------------------------------------

void Cpu::store_to_bc() {
    b = computed_u16_msb;
    c = computed_u16_lsb;
}

// ----------------------------------------------------------------------------

void Cpu::store_to_de() {
    d = computed_u16_msb;
    e = computed_u16_lsb;
}

// ----------------------------------------------------------------------------

void Cpu::store_to_hl() {
    h = computed_u16_msb;
    l = computed_u16_lsb;
}

// ----------------------------------------------------------------------------

void Cpu::store_to_sp() {
    sp = join_to_u16(computed_u16_msb, computed_u16_lsb);
}

// ----------------------------------------------------------------------------

void Cpu::store_to_af() {
    a = computed_u16_msb;
    flags = computed_u16_lsb;
}

// ----------------------------------------------------------------------------

void Cpu::store_indirect_hl() {
    bus->cpu_write(get_hl(), computed_u8);
}

// ----------------------------------------------------------------------------

void Cpu::store_indirect_hl_plus() {
    store_indirect_hl();

    Uint16 new_hl = get_hl() + 0x01;
    split_u16(new_hl, h, l);
}

// ----------------------------------------------------------------------------

void Cpu::store_indirect_hl_minus() {
    store_indirect_hl();

    Uint16 new_hl = get_hl() - 0x01;
    split_u16(new_hl, h, l);
}

// ----------------------------------------------------------------------------

void Cpu::store_indirect_bc() {
    bus->cpu_write(get_bc(), computed_u8);
}

// ----------------------------------------------------------------------------

void Cpu::store_indirect_de() {
    bus->cpu_write(get_de(), computed_u8);
}

// ----------------------------------------------------------------------------

void Cpu::store_direct_ff_offset() {
    Uint16 destination_address = 0xFF00 + bus->cpu_read(pc++);
    bus->cpu_write(destination_address, fetched_u8);
}

// ----------------------------------------------------------------------------

void Cpu::store_indirect_ff_with_c_offset() {
    Uint16 destination_address = 0xFF00 + c;
    bus->cpu_write(destination_address, fetched_u8);
}

// ----------------------------------------------------------------------------

void Cpu::store_direct_8bit() {
    Uint16 least_significant_address_byte = bus->cpu_read(pc++);
    Uint16 most_significant_address_byte = bus->cpu_read(pc++);
    Uint16 destination_address = join_to_u16(most_significant_address_byte, least_significant_address_byte);
    bus->cpu_write(destination_address, fetched_u8);
}

// ----------------------------------------------------------------------------

void Cpu::store_direct_16bit() {
    Uint16 least_significant_address_byte = bus->cpu_read(pc++);
    Uint16 most_significant_address_byte = bus->cpu_read(pc++);
    Uint16 destination_address = join_to_u16(most_significant_address_byte, least_significant_address_byte);
    bus->cpu_write(destination_address, fetched_u16_lsb);
    bus->cpu_write(destination_address + 0x0001, fetched_u16_msb);
}

// ----------------------------------------------------------------------------

void Cpu::push() {
    sp--;
    bus->cpu_write(sp, computed_u16_msb);
    sp--;
    bus->cpu_write(sp, computed_u16_lsb);
}

// ----------------------------------------------------------------------------

void Cpu::store_to_pc() {
    pc = join_to_u16(computed_u16_msb, computed_u16_lsb);
}

// ----------------------------------------------------------------------------

Uint16 Cpu::get_hl() {
    return join_to_u16(h, l);
}

// ----------------------------------------------------------------------------

Uint16 Cpu::get_bc() {
    return join_to_u16(b, c);
}

// ----------------------------------------------------------------------------

Uint16 Cpu::get_de() {
    return join_to_u16(d, e);
}

// ----------------------------------------------------------------------------

Uint32 Cpu::full_add_8bit(Uint32 a, Uint32 b, bool carry_in, bool& half_carry_out, bool& carry_out) {
    Uint32 carry = carry_in ? 1 : 0;
    half_carry_out = (((a & 0xF) + (b & 0xF) + carry) & 0x10) != 0;
    carry_out = (((a & 0xFF) + (b & 0xFF) + carry) & 0x100) != 0;

    return a + b + carry_in;
}

// ----------------------------------------------------------------------------

Uint32 Cpu::full_add_16bit(Uint32 a, Uint32 b, bool carry_in, bool& half_carry_out, bool& carry_out) {
    Uint32 carry = carry_in ? 1 : 0;
    half_carry_out = (((a & 0xFFF) + (b & 0xFFF) + carry) & 0x1000) != 0;
    carry_out = (((a & 0xFFFF) + (b & 0xFFFF) + carry) & 0x10000) != 0;

    return a + b + carry_in;
}

// ----------------------------------------------------------------------------

Uint32 Cpu::full_sub_8bit(Uint32 a, Uint32 b, bool carry_in, bool& half_carry_out, bool& carry_out) {
    Uint32 carry = carry_in ? 1 : 0;
    half_carry_out = (((a & 0xF) - (b & 0xF) - carry) & 0x10) != 0;
    carry_out = (((a & 0xFF) - (b & 0xFF) - carry) & 0x100) != 0;

    return a - b - carry_in;
}

// ----------------------------------------------------------------------------

Uint16 Cpu::get_fetched_u16() {
    return join_to_u16(fetched_u16_msb, fetched_u16_lsb);
}

// ----------------------------------------------------------------------------

void Cpu::split_u16(Uint16 u16, Uint8& msb, Uint8& lsb) {
    msb = static_cast<Uint8>((u16 >> 8) & 0x00FF);
    lsb = static_cast<Uint8>(u16 & 0x00FF);
}

// ----------------------------------------------------------------------------

Uint16 Cpu::join_to_u16(Uint8 msb, Uint8 lsb) {
    return (static_cast<Uint16>(msb) << 8) | static_cast<Uint16>(lsb);
}

// ----------------------------------------------------------------------------

void Cpu::override_cpu_state(Cpu_Info& new_cpu_state) {
    pc = new_cpu_state.pc;
    sp = new_cpu_state.sp;
    a = new_cpu_state.a;
    b = new_cpu_state.b;
    c = new_cpu_state.c;
    d = new_cpu_state.d;
    e = new_cpu_state.e;
    h = new_cpu_state.h;
    l = new_cpu_state.l;

    flags = 0x00;
    if (new_cpu_state.z_flag) {
        flags |= Z_FLAG;
    }
    if (new_cpu_state.n_flag) {
        flags |= N_FLAG;
    }
    if (new_cpu_state.h_flag) {
        flags |= H_FLAG;
    }
    if (new_cpu_state.c_flag) {
        flags |= C_FLAG;
    }
}

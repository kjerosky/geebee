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
    //opcode_table[0x00] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x01] = { &Cpu::fetch_from_immediate_u16, &Cpu::ld_16bit, &Cpu::store_to_bc, 3 };
    opcode_table[0x02] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_indirect_bc, 2 };
    //opcode_table[0x03] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x04] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x05] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x06] = { &Cpu::fetch_from_immediate_u8, &Cpu::ld_8bit, &Cpu::store_to_b, 2 };
    //opcode_table[0x07] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x08] = { &Cpu::fetch_from_sp, &Cpu::ld_16bit, &Cpu::store_direct_16bit, 5 };
    //opcode_table[0x09] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x0A] = { &Cpu::fetch_indirect_bc, &Cpu::ld_8bit, &Cpu::store_to_a, 2 };
    //opcode_table[0x0B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x0C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x0D] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x0E] = { &Cpu::fetch_from_immediate_u8, &Cpu::ld_8bit, &Cpu::store_to_c, 2 };
    //opcode_table[0x0F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //opcode_table[0x10] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x11] = { &Cpu::fetch_from_immediate_u16, &Cpu::ld_16bit, &Cpu::store_to_de, 3 };
    opcode_table[0x12] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_indirect_de, 2 };
    //opcode_table[0x13] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x14] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x15] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x16] = { &Cpu::fetch_from_immediate_u8, &Cpu::ld_8bit, &Cpu::store_to_d, 2 };
    //opcode_table[0x17] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x18] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x19] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x1A] = { &Cpu::fetch_indirect_de, &Cpu::ld_8bit, &Cpu::store_to_a, 2 };
    //opcode_table[0x1B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x1C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x1D] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x1E] = { &Cpu::fetch_from_immediate_u8, &Cpu::ld_8bit, &Cpu::store_to_e, 2 };
    //opcode_table[0x1F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //opcode_table[0x20] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x21] = { &Cpu::fetch_from_immediate_u16, &Cpu::ld_16bit, &Cpu::store_to_hl, 3 };
    opcode_table[0x22] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_indirect_hl_plus, 2 };
    //opcode_table[0x23] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x24] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x25] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x26] = { &Cpu::fetch_from_immediate_u8, &Cpu::ld_8bit, &Cpu::store_to_h, 2 };
    //opcode_table[0x27] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x28] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x29] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x2A] = { &Cpu::fetch_indirect_hl_plus, &Cpu::ld_8bit, &Cpu::store_to_a, 2 };
    //opcode_table[0x2B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x2C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x2D] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x2E] = { &Cpu::fetch_from_immediate_u8, &Cpu::ld_8bit, &Cpu::store_to_l, 2 };
    //opcode_table[0x2F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //opcode_table[0x30] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x31] = { &Cpu::fetch_from_immediate_u16, &Cpu::ld_16bit, &Cpu::store_to_sp, 3 };
    opcode_table[0x32] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_indirect_hl_minus, 2 };
    //opcode_table[0x33] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x34] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x35] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x36] = { &Cpu::fetch_from_immediate_u8, &Cpu::ld_8bit, &Cpu::store_indirect_hl, 3 };
    //opcode_table[0x37] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x38] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x39] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x3A] = { &Cpu::fetch_indirect_hl_minus, &Cpu::ld_8bit, &Cpu::store_to_a, 2 };
    //opcode_table[0x3B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x3C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x3D] = { &Cpu::, &Cpu::, &Cpu::,  };
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

    //opcode_table[0x80] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x81] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x82] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x83] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x84] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x85] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x86] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x87] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x88] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x89] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x8A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x8B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x8C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x8D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x8E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x8F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //opcode_table[0x90] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x91] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x92] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x93] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x94] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x95] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x96] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x97] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x98] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x99] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x9A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x9B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x9C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x9D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x9E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x9F] = { &Cpu::, &Cpu::, &Cpu::,  };

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

    //opcode_table[0xB0] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xB1] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xB2] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xB3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xB4] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xB5] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xB6] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xB7] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xB8] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xB9] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xBA] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xBB] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xBC] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xBD] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xBE] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xBF] = { &Cpu::, &Cpu::, &Cpu::,  };

    //opcode_table[0xC0] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xC1] = { &Cpu::pop, &Cpu::ld_16bit, &Cpu::store_to_bc, 3 };
    //opcode_table[0xC2] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xC3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xC4] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xC5] = { &Cpu::fetch_from_bc, &Cpu::ld_16bit, &Cpu::push, 4 };
    //opcode_table[0xC6] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xC7] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xC8] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xC9] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xCA] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xCB] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xCC] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xCD] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xCE] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xCF] = { &Cpu::, &Cpu::, &Cpu::,  };

    //opcode_table[0xD0] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xD1] = { &Cpu::pop, &Cpu::ld_16bit, &Cpu::store_to_de, 3 };
    //opcode_table[0xD2] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xD3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xD4] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xD5] = { &Cpu::fetch_from_de, &Cpu::ld_16bit, &Cpu::push, 4 };
    //opcode_table[0xD6] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xD7] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xD8] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xD9] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xDA] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xDB] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xDC] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xDD] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xDE] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xDF] = { &Cpu::, &Cpu::, &Cpu::,  };

    opcode_table[0xE0] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_direct_ff_offset, 3 };
    opcode_table[0xE1] = { &Cpu::pop, &Cpu::ld_16bit, &Cpu::store_to_hl, 3 };
    opcode_table[0xE2] = { &Cpu::fetch_from_a, &Cpu::ld_8bit, &Cpu::store_indirect_ff_with_c_offset, 2 };
    //opcode_table[0xE3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xE4] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xE5] = { &Cpu::fetch_from_hl, &Cpu::ld_16bit, &Cpu::push, 4 };
    opcode_table[0xE6] = { &Cpu::fetch_from_immediate_u8, &Cpu::and_with_a, &Cpu::store_to_a, 2 };
    //opcode_table[0xE7] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xE8] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xE9] = { &Cpu::, &Cpu::, &Cpu::,  };
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
    //opcode_table[0xF6] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xF7] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0xF8] = { &Cpu::fetch_from_adjusted_sp, &Cpu::ld_16bit, &Cpu::store_to_hl, 3 };
    opcode_table[0xF9] = { &Cpu::fetch_from_hl, &Cpu::ld_16bit, &Cpu::store_to_sp, 2 };
    opcode_table[0xFA] = { &Cpu::fetch_direct, &Cpu::ld_8bit, &Cpu::store_to_a, 4 };
    //opcode_table[0xFB] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xFC] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xFD] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xFE] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xFF] = { &Cpu::, &Cpu::, &Cpu::,  };

    //prefixed_opcode_table[0x00] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x01] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x02] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x03] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x04] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x05] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x06] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x07] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x08] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x09] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x0A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x0B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x0C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x0D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x0E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x0F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //prefixed_opcode_table[0x10] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x11] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x12] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x13] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x14] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x15] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x16] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x17] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x18] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x19] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x1A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x1B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x1C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x1D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x1E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x1F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //prefixed_opcode_table[0x20] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x21] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x22] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x23] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x24] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x25] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x26] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x27] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x28] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x29] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x2A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x2B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x2C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x2D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x2E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x2F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //prefixed_opcode_table[0x30] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x31] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x32] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x33] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x34] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x35] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x36] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x37] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x38] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x39] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x3A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x3B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x3C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x3D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x3E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x3F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //prefixed_opcode_table[0x40] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x41] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x42] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x43] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x44] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x45] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x46] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x47] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x48] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x49] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x4A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x4B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x4C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x4D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x4E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x4F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //prefixed_opcode_table[0x50] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x51] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x52] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x53] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x54] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x55] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x56] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x57] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x58] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x59] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x5A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x5B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x5C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x5D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x5E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x5F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //prefixed_opcode_table[0x60] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x61] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x62] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x63] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x64] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x65] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x66] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x67] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x68] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x69] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x6A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x6B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x6C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x6D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x6E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x6F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //prefixed_opcode_table[0x70] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x71] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x72] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x73] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x74] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x75] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x76] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x77] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x78] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x79] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x7A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x7B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x7C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x7D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x7E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x7F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //prefixed_opcode_table[0x80] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x81] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x82] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x83] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x84] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x85] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x86] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x87] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x88] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x89] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x8A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x8B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x8C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x8D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x8E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x8F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //prefixed_opcode_table[0x90] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x91] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x92] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x93] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x94] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x95] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x96] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x97] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x98] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x99] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x9A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x9B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x9C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x9D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x9E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0x9F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //prefixed_opcode_table[0xA0] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xA1] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xA2] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xA3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xA4] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xA5] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xA6] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xA7] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xA8] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xA9] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xAA] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xAB] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xAC] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xAD] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xAE] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xAF] = { &Cpu::, &Cpu::, &Cpu::,  };

    //prefixed_opcode_table[0xB0] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xB1] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xB2] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xB3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xB4] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xB5] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xB6] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xB7] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xB8] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xB9] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xBA] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xBB] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xBC] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xBD] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xBE] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xBF] = { &Cpu::, &Cpu::, &Cpu::,  };

    //prefixed_opcode_table[0xC0] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xC1] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xC2] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xC3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xC4] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xC5] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xC6] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xC7] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xC8] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xC9] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xCA] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xCB] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xCC] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xCD] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xCE] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xCF] = { &Cpu::, &Cpu::, &Cpu::,  };

    //prefixed_opcode_table[0xD0] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xD1] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xD2] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xD3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xD4] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xD5] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xD6] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xD7] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xD8] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xD9] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xDA] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xDB] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xDC] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xDD] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xDE] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xDF] = { &Cpu::, &Cpu::, &Cpu::,  };

    //prefixed_opcode_table[0xE0] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xE1] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xE2] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xE3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xE4] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xE5] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xE6] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xE7] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xE8] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xE9] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xEA] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xEB] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xEC] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xED] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xEE] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xEF] = { &Cpu::, &Cpu::, &Cpu::,  };

    //prefixed_opcode_table[0xF0] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xF1] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xF2] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xF3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xF4] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xF5] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xF6] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xF7] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xF8] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xF9] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xFA] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xFB] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xFC] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xFD] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xFE] = { &Cpu::, &Cpu::, &Cpu::,  };
    //prefixed_opcode_table[0xFF] = { &Cpu::, &Cpu::, &Cpu::,  };
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

void Cpu::clock() {
    if (current_instruction_cycles_remaining <= 0) {
        Uint8 opcode_byte = bus->cpu_read(pc++);

        Instruction opcode;
        if (opcode_byte == 0xCB) {
            opcode_byte = bus->cpu_read(pc++);
            opcode = prefixed_opcode_table[opcode_byte];
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
    fetched_u16_msb = static_cast<Uint8>((sp >> 8) & 0x00FF);
    fetched_u16_lsb = static_cast<Uint8>(sp & 0x00FF);
}

// ----------------------------------------------------------------------------

void Cpu::fetch_indirect_hl() {
    fetched_u8 = bus->cpu_read(get_hl());
}

// ----------------------------------------------------------------------------

void Cpu::fetch_indirect_hl_plus() {
    fetched_u8 = bus->cpu_read(get_hl());

    Uint16 new_hl = get_hl() + 0x01;
    h = static_cast<Uint8>((new_hl >> 8) & 0x00FF);
    l = static_cast<Uint8>(new_hl & 0x00FF);
}

// ----------------------------------------------------------------------------

void Cpu::fetch_indirect_hl_minus() {
    fetched_u8 = bus->cpu_read(get_hl());

    Uint16 new_hl = get_hl() - 0x01;
    h = static_cast<Uint8>((new_hl >> 8) & 0x00FF);
    l = static_cast<Uint8>(new_hl & 0x00FF);
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
    Uint16 destination_address = (most_significant_address_byte << 8) + least_significant_address_byte;
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

    Uint32 carries;
    Uint32 result = add_and_track_carries(sp, sp_offset, carries);
    fetched_u16_msb = (result >> 8) & 0x000000FF;
    fetched_u16_lsb = result & 0x000000FF;
    set_flag(Z_FLAG, false);
    set_flag(N_FLAG, false);
    set_flag(H_FLAG, (carries & (1 << 3)) != 0);
    set_flag(C_FLAG, (carries & (1 << 7)) != 0);
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
    sp = (static_cast<Uint16>(computed_u16_msb) << 8) | static_cast<Uint16>(computed_u16_lsb);
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
    h = static_cast<Uint8>((new_hl >> 8) & 0x00FF);
    l = static_cast<Uint8>(new_hl & 0x00FF);
}

// ----------------------------------------------------------------------------

void Cpu::store_indirect_hl_minus() {
    store_indirect_hl();

    Uint16 new_hl = get_hl() - 0x01;
    h = static_cast<Uint8>((new_hl >> 8) & 0x00FF);
    l = static_cast<Uint8>(new_hl & 0x00FF);
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
    Uint16 destination_address = (most_significant_address_byte << 8) + least_significant_address_byte;
    bus->cpu_write(destination_address, fetched_u8);
}

// ----------------------------------------------------------------------------

void Cpu::store_direct_16bit() {
    Uint16 least_significant_address_byte = bus->cpu_read(pc++);
    Uint16 most_significant_address_byte = bus->cpu_read(pc++);
    Uint16 destination_address = (most_significant_address_byte << 8) + least_significant_address_byte;
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

Uint16 Cpu::get_hl() {
    return (static_cast<Uint16>(h) << 8) | static_cast<Uint16>(l);
}

// ----------------------------------------------------------------------------

Uint16 Cpu::get_bc() {
    return (static_cast<Uint16>(b) << 8) | static_cast<Uint16>(c);
}

// ----------------------------------------------------------------------------

Uint16 Cpu::get_de() {
    return (static_cast<Uint16>(d) << 8) | static_cast<Uint16>(e);
}

// ----------------------------------------------------------------------------

Uint32 Cpu::add_and_track_carries(Uint32 a, Uint32 b, Uint32& carries) {
    carries = 0;
    Uint32 carry_bit = 0;
    for (Uint32 bit_place = 0; bit_place < 32; bit_place++) {
        Uint32 bit_mask = 1 << bit_place;
        Uint32 a_bit = (a & bit_mask) == 0 ? 0 : 1;
        Uint32 b_bit = (b & bit_mask) == 0 ? 0 : 1;
        Uint32 result = a_bit + b_bit + carry_bit;

        carry_bit = result > 1 ? 1 : 0;
        carries |= carry_bit << bit_place;
    }

    return a + b;
}

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
    //opcode_table[0x01] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x02] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x03] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x04] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x05] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x06] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x07] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x08] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x09] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x0A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x0B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x0C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x0D] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x0E] = { &Cpu::fetch_from_immediate_u8, &Cpu::ld, &Cpu::store_to_c, 2};
    //opcode_table[0x0F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //opcode_table[0x10] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x11] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x12] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x13] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x14] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x15] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x16] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x17] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x18] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x19] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x1A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x1B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x1C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x1D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x1E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x1F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //opcode_table[0x20] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x21] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x22] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x23] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x24] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x25] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x26] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x27] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x28] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x29] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x2A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x2B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x2C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x2D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x2E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x2F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //opcode_table[0x30] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x31] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x32] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x33] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x34] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x35] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x36] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x37] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x38] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x39] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x3A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x3B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x3C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x3D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x3E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x3F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //opcode_table[0x40] = { &Cpu::, &Cpu::, &Cpu::,  };
    opcode_table[0x41] = { &Cpu::fetch_from_c, &Cpu::ld, &Cpu::store_to_b, 1 };
    //opcode_table[0x42] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x43] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x44] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x45] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x46] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x47] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x48] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x49] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x4A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x4B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x4C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x4D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x4E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x4F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //opcode_table[0x50] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x51] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x52] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x53] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x54] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x55] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x56] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x57] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x58] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x59] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x5A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x5B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x5C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x5D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x5E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x5F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //opcode_table[0x60] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x61] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x62] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x63] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x64] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x65] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x66] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x67] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x68] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x69] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x6A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x6B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x6C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x6D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x6E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x6F] = { &Cpu::, &Cpu::, &Cpu::,  };

    //opcode_table[0x70] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x71] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x72] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x73] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x74] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x75] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x76] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x77] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x78] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x79] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x7A] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x7B] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x7C] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x7D] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x7E] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0x7F] = { &Cpu::, &Cpu::, &Cpu::,  };

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

    //opcode_table[0xA0] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xA1] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xA2] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xA3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xA4] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xA5] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xA6] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xA7] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xA8] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xA9] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xAA] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xAB] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xAC] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xAD] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xAE] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xAF] = { &Cpu::, &Cpu::, &Cpu::,  };

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
    //opcode_table[0xC1] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xC2] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xC3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xC4] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xC5] = { &Cpu::, &Cpu::, &Cpu::,  };
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
    //opcode_table[0xD1] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xD2] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xD3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xD4] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xD5] = { &Cpu::, &Cpu::, &Cpu::,  };
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

    //opcode_table[0xE0] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xE1] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xE2] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xE3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xE4] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xE5] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xE6] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xE7] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xE8] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xE9] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xEA] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xEB] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xEC] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xED] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xEE] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xEF] = { &Cpu::, &Cpu::, &Cpu::,  };

    //opcode_table[0xF0] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xF1] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xF2] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xF3] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xF4] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xF5] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xF6] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xF7] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xF8] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xF9] = { &Cpu::, &Cpu::, &Cpu::,  };
    //opcode_table[0xFA] = { &Cpu::, &Cpu::, &Cpu::,  };
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
    pc = 0x0100;
    sp = 0xFFFE;

    z_flag = false;
    n_flag = false;
    h_flag = false;
    c_flag = false;
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
        z_flag,
        n_flag,
        h_flag,
        c_flag,
    };
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_c() {
    fetched_u8 = c;
}

// ----------------------------------------------------------------------------

void Cpu::fetch_from_immediate_u8() {
    fetched_u8 = bus->cpu_read(pc++);
}

// ----------------------------------------------------------------------------

int Cpu::ld() {
    computed_u8 = fetched_u8;
    return 0;
}

// ----------------------------------------------------------------------------

void Cpu::store_to_b() {
    b = computed_u8;
}

// ----------------------------------------------------------------------------

void Cpu::store_to_c() {
    c = computed_u8;
}

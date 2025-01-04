#include "bit_utils.h"

std::string hex(unsigned int number, int width) {
    std::string s(width, '0');
    for (int i = width - 1; i >= 0; i--, number >>= 4) {
        s[i] = "0123456789ABCDEF"[number & 0xF];
    }

    return s;
}

// ----------------------------------------------------------------------------

void split_u16(Uint16 u16, Uint8& msb, Uint8& lsb) {
    msb = static_cast<Uint8>((u16 >> 8) & 0x00FF);
    lsb = static_cast<Uint8>(u16 & 0x00FF);
}

// ----------------------------------------------------------------------------

Uint16 join_to_u16(Uint8 msb, Uint8 lsb) {
    return (static_cast<Uint16>(msb) << 8) | static_cast<Uint16>(lsb);
}

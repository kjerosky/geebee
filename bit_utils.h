#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <string>
#include <SDL2/SDL.h>

std::string hex(unsigned int number, int width);

void split_u16(Uint16 u16, Uint8& msb, Uint8& lsb);
Uint16 join_to_u16(Uint8 msb, Uint8 lsb);

#endif

#include "GameBoy.h"

GameBoy::GameBoy() {
    bus = new Bus();
    cpu = new Cpu(bus);
}

GameBoy::~GameBoy() {
    delete cpu;
    delete bus;
}


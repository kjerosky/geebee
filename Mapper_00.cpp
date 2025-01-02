#include "Mapper_00.h"

Mapper_00::Mapper_00() {
    // do nothing
}

// ----------------------------------------------------------------------------

Mapper_00::~Mapper_00() {
    // do nothing
}

// ----------------------------------------------------------------------------

Uint16 Mapper_00::map_cpu_read(Uint16 address) {
    return address;
}

// ----------------------------------------------------------------------------

Uint16 Mapper_00::map_cpu_write(Uint16 address) {
    return address;
}

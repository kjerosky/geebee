#include "Bus.h"

#include <cstring>

#include "Ppu.h"
#include "Cartridge.h"

Bus::Bus(Ppu* ppu, Cartridge* cartridge)
: ppu(ppu),
  cartridge(cartridge),
  interrupt_flag_register(0x00),
  interrupt_enable_register(0x00),
  oam_dma_started(false),
  oam_dma_start_address(0x0000),
  joypad_register(0x00),
  div_register(0x00),
  timer_counter(0x00),
  timer_modulo(0x00),
  timer_control(0x00),
  machine_cycles(0x0000) {

    std::memset(work_ram_bank_0, 0x00, WORK_RAM_BANK_SIZE * sizeof(Uint8));
    std::memset(work_ram_bank_1, 0x00, WORK_RAM_BANK_SIZE * sizeof(Uint8));
    std::memset(high_ram, 0x00, HIGH_RAM_SIZE * sizeof(Uint8));
}

// ----------------------------------------------------------------------------

Bus::~Bus() {
    // nothing to do
}

// ----------------------------------------------------------------------------

Uint8 Bus::cpu_read(Uint16 address) {
    Uint8 read_value = 0xFF;

    // adjust for echo ram (E000-FDFF is a mirror of C000-DDFF)
    if (address >= 0xE000 && address <= 0xFDFF) {
        address -= 0x2000;
    }

    if (address >= 0x0000 && address <= 0x7FFF) {
        read_value = cartridge->cpu_read(address);
    } else if (address >= 0x8000 && address <= 0x9FFF) {
        read_value = ppu->cpu_read(address);
    } else if (address >= 0xA000 && address <= 0xBFFF) {
        // 8 KiB external ram
    } else if (address >= 0xC000 && address <= 0xCFFF) {
        read_value = work_ram_bank_0[address - 0xC000];
    } else if (address >= 0xD000 && address <= 0xDFFF) {
        read_value = work_ram_bank_1[address - 0xD000];
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        read_value = ppu->cpu_read(address);
    } else if (address >= 0xFEA0 && address <= 0xFEFF) {
        // not usable (Nintendo says use of this area is prohibited)
    } else if (address >= 0xFF00 && address <= 0xFF7F) {
        read_value = read_from_io_register(address);
    } else if (address >= 0xFF80 && address <= 0xFFFE) {
        read_value = high_ram[address - 0xFF80];
    } else if (address == 0xFFFF) {
        read_value = interrupt_enable_register;
    }

    return read_value;
}

// ----------------------------------------------------------------------------

void Bus::cpu_write(Uint16 address, Uint8 value) {
    // adjust for echo ram (E000-FDFF is a mirror of C000-DDFF)
    if (address >= 0xE000 && address <= 0xFDFF) {
        address -= 0x2000;
    }

    if (address >= 0x0000 && address <= 0x7FFF) {
        cartridge->cpu_write(address, value);
    } else if (address >= 0x8000 && address <= 0x9FFF) {
        ppu->cpu_write(address, value);
    } else if (address >= 0xA000 && address <= 0xBFFF) {
        // 8 KiB external ram
    } else if (address >= 0xC000 && address <= 0xCFFF) {
        work_ram_bank_0[address - 0xC000] = value;
    } else if (address >= 0xD000 && address <= 0xDFFF) {
        work_ram_bank_1[address - 0xD000] = value;
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        ppu->cpu_write(address, value);
    } else if (address >= 0xFEA0 && address <= 0xFEFF) {
        // not usable (Nintendo says use of this area is prohibited)
    } else if (address >= 0xFF00 && address <= 0xFF7F) {
        write_to_io_register(address, value);
    } else if (address >= 0xFF80 && address <= 0xFFFE) {
        high_ram[address - 0xFF80] = value;
    } else if (address == 0xFFFF) {
        interrupt_enable_register = value;
    }
}

// ----------------------------------------------------------------------------

Uint8 Bus::read_from_io_register(Uint16 address) {
    Uint8 read_value = 0xFF;

    if (address == 0xFF00) {
        read_value = joypad_register;
    } else if (address == 0xFF04) {
        read_value = div_register;
    } else if (address == 0xFF05) {
        read_value = timer_counter;
    } else if (address == 0xFF06) {
        read_value = timer_modulo;
    } else if (address == 0xFF07) {
        read_value = timer_control;
    } else if (address == 0xFF0F) {
        read_value = interrupt_flag_register;
    } else if (address >= 0xFF40 && address <= 0xFF6C) {
        read_value = ppu->cpu_read(address);
    }

    return read_value;
}

// ----------------------------------------------------------------------------

void Bus::write_to_io_register(Uint16 address, Uint8 value) {
    if (address == 0xFF00) {
        refresh_joypad_register(value);
    } else if (address == 0xFF04) {
        div_register = 0x00;
    } else if (address == 0xFF05) {
        timer_counter = value;
    } else if (address == 0xFF06) {
        timer_modulo = value;
    } else if (address == 0xFF07) {
        timer_control = value;
    } else if (address == 0xFF0F) {
        interrupt_flag_register = value;
    } else if (address == 0xFF46) {
        oam_dma_start_address = static_cast<Uint16>(value) << 8;
        oam_dma_started = true;
    } else if (address >= 0xFF40 && address <= 0xFF6C) {
        ppu->cpu_write(address, value);
    }
}

// ----------------------------------------------------------------------------

bool Bus::check_and_reset_oam_dma_started(Uint16& output_oam_dma_start_address) {
    if (oam_dma_started) {
        output_oam_dma_start_address = oam_dma_start_address;
        oam_dma_started = false;
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------

void Bus::refresh_joypad_register(Uint8 input_byte) {
    // NOTE: Keep in mind that the joypad register is active low, so the
    //       associated button's bit is cleared when the button is pressed.
    //       This also applies to bits 4 and 5 for selecting either buttons
    //       or directions.

    joypad_register = input_byte | 0x0F;

    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

    switch (input_byte & 0x30) {
        case 0x20:
            if (keyboard_state[SDL_SCANCODE_S]) {
                joypad_register &= ~0x08;
            }
            if (keyboard_state[SDL_SCANCODE_W]) {
                joypad_register &= ~0x04;
            }
            if (keyboard_state[SDL_SCANCODE_A]) {
                joypad_register &= ~0x02;
            }
            if (keyboard_state[SDL_SCANCODE_D]) {
                joypad_register &= ~0x01;
            }
            break;

        case 0x10:
            if (keyboard_state[SDL_SCANCODE_I]) {
                joypad_register &= ~0x08;
            }
            if (keyboard_state[SDL_SCANCODE_U]) {
                joypad_register &= ~0x04;
            }
            if (keyboard_state[SDL_SCANCODE_J]) {
                joypad_register &= ~0x02;
            }
            if (keyboard_state[SDL_SCANCODE_K]) {
                joypad_register &= ~0x01;
            }
            break;
    }
}

// ----------------------------------------------------------------------------

Uint8 Bus::clock_machine_cycle() {
    Uint8 interrupts_raised = 0x00;

    machine_cycles = (machine_cycles + 1) % 256;

    if (machine_cycles % 64 == 0) {
        div_register++;
    }

    Uint16 timer_machine_cycle_count;
    switch (timer_control & 0x03) {
        case 0x00:
            timer_machine_cycle_count = 256;
            break;
        case 0x01:
            timer_machine_cycle_count = 4;
            break;
        case 0x02:
            timer_machine_cycle_count = 16;
            break;
        case 0x03:
            timer_machine_cycle_count = 64;
            break;
    }

    bool is_timer_enabled = (timer_control & 0x04) != 0x00;
    if (is_timer_enabled && machine_cycles % timer_machine_cycle_count == 0) {
        timer_counter++;
        if (timer_counter == 0x00) {
            timer_counter = timer_modulo;
            interrupts_raised |= 0x04;
        }
    }

    return interrupts_raised;
}

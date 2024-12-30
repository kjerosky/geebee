#include <iostream>
#include <string>

#include "GameBoy.h"

const Uint8 Z_FLAG = 0x01 << 7;
const Uint8 N_FLAG = 0x01 << 6;
const Uint8 H_FLAG = 0x01 << 5;
const Uint8 C_FLAG = 0x01 << 4;

void read_cpu_state_from_stdin(Cpu_Info& cpu_state) {
    Uint16 pc;
    Uint16 sp;
    Uint8 a;
    Uint8 b;
    Uint8 c;
    Uint8 d;
    Uint8 e;
    Uint8 flags;
    Uint8 h;
    Uint8 l;

    std::string next_token;
    std::cin >> next_token;
    pc = static_cast<Uint16>(std::stoi(next_token));
    std::cin >> next_token;
    sp = static_cast<Uint16>(std::stoi(next_token));
    std::cin >> next_token;
    a = static_cast<Uint8>(std::stoi(next_token));
    std::cin >> next_token;
    b = static_cast<Uint8>(std::stoi(next_token));
    std::cin >> next_token;
    c = static_cast<Uint8>(std::stoi(next_token));
    std::cin >> next_token;
    d = static_cast<Uint8>(std::stoi(next_token));
    std::cin >> next_token;
    e = static_cast<Uint8>(std::stoi(next_token));
    std::cin >> next_token;
    flags = static_cast<Uint8>(std::stoi(next_token));
    std::cin >> next_token;
    h = static_cast<Uint8>(std::stoi(next_token));
    std::cin >> next_token;
    l = static_cast<Uint8>(std::stoi(next_token));

    cpu_state = {
        a,
        b,
        c,
        d,
        e,
        h,
        l,
        pc,
        sp,
        (flags & Z_FLAG) != 0,
        (flags & N_FLAG) != 0,
        (flags & H_FLAG) != 0,
        (flags & C_FLAG) != 0,
    };
}

// ----------------------------------------------------------------------------

void override_ram_entries_from_stdin(Uint8* ram) {
    std::string next_token;
    std::cin >> next_token;
    int ram_entries_count = std::stoi(next_token);
    for (int i = 0; i < ram_entries_count; i++) {
        std::cin >> next_token;
        Uint16 address = static_cast<Uint16>(std::stoi(next_token));
        std::cin >> next_token;
        Uint8 value = static_cast<Uint8>(std::stoi(next_token));

        ram[address] = value;
    }
}

// ----------------------------------------------------------------------------

bool verify_cpu_state(Cpu_Info& expected, Cpu_Info& actual) {
    bool is_cpu_verified = true;

    if (expected.a != actual.a) {
        is_cpu_verified = false;
        std::cout << "[ASSERTION FAILED] a: expected=" << static_cast<int>(expected.a) << " actual=" << static_cast<int>(actual.a) << std::endl;
    }
    if (expected.b != actual.b) {
        is_cpu_verified = false;
        std::cout << "[ASSERTION FAILED] b: expected=" << static_cast<int>(expected.b) << " actual=" << static_cast<int>(actual.b) << std::endl;
    }
    if (expected.c != actual.c) {
        is_cpu_verified = false;
        std::cout << "[ASSERTION FAILED] c: expected=" << static_cast<int>(expected.c) << " actual=" << static_cast<int>(actual.c) << std::endl;
    }
    if (expected.d != actual.d) {
        is_cpu_verified = false;
        std::cout << "[ASSERTION FAILED] d: expected=" << static_cast<int>(expected.d) << " actual=" << static_cast<int>(actual.d) << std::endl;
    }
    if (expected.e != actual.e) {
        is_cpu_verified = false;
        std::cout << "[ASSERTION FAILED] e: expected=" << static_cast<int>(expected.e) << " actual=" << static_cast<int>(actual.e) << std::endl;
    }
    if (expected.h != actual.h) {
        is_cpu_verified = false;
        std::cout << "[ASSERTION FAILED] h: expected=" << static_cast<int>(expected.h) << " actual=" << static_cast<int>(actual.h) << std::endl;
    }
    if (expected.l != actual.l) {
        is_cpu_verified = false;
        std::cout << "[ASSERTION FAILED] l: expected=" << static_cast<int>(expected.l) << " actual=" << static_cast<int>(actual.l) << std::endl;
    }
    if (expected.pc != actual.pc) {
        is_cpu_verified = false;
        std::cout << "[ASSERTION FAILED] pc: expected=" << static_cast<int>(expected.pc) << " actual=" << static_cast<int>(actual.pc) << std::endl;
    }
    if (expected.sp != actual.sp) {
        is_cpu_verified = false;
        std::cout << "[ASSERTION FAILED] sp: expected=" << static_cast<int>(expected.sp) << " actual=" << static_cast<int>(actual.sp) << std::endl;
    }
    if (expected.z_flag != actual.z_flag) {
        is_cpu_verified = false;
        std::cout << "[ASSERTION FAILED] z_flag: expected=" << static_cast<int>(expected.z_flag) << " actual=" << static_cast<int>(actual.z_flag) << std::endl;
    }
    if (expected.n_flag != actual.n_flag) {
        is_cpu_verified = false;
        std::cout << "[ASSERTION FAILED] n_flag: expected=" << static_cast<int>(expected.n_flag) << " actual=" << static_cast<int>(actual.n_flag) << std::endl;
    }
    if (expected.h_flag != actual.h_flag) {
        is_cpu_verified = false;
        std::cout << "[ASSERTION FAILED] h_flag: expected=" << static_cast<int>(expected.h_flag) << " actual=" << static_cast<int>(actual.h_flag) << std::endl;
    }
    if (expected.c_flag != actual.c_flag) {
        is_cpu_verified = false;
        std::cout << "[ASSERTION FAILED] c_flag: expected=" << static_cast<int>(expected.c_flag) << " actual=" << static_cast<int>(actual.c_flag) << std::endl;
    }

    return is_cpu_verified;
}

// ----------------------------------------------------------------------------

bool verify_ram_entries_from_stdin(Uint8* ram) {
    bool are_all_entries_verified = true;

    std::string next_token;
    std::cin >> next_token;
    int ram_entries_count = std::stoi(next_token);
    for (int i = 0; i < ram_entries_count; i++) {
        std::cin >> next_token;
        Uint16 address = static_cast<Uint16>(std::stoi(next_token));
        std::cin >> next_token;
        Uint8 value = static_cast<Uint8>(std::stoi(next_token));

        if (ram[address] != value) {
            are_all_entries_verified = false;
            std::cout << "[ASSERTION FAILED] ram[" << static_cast<int>(address) << "]: expected=" << static_cast<int>(value) << " actual=" << static_cast<int>(ram[address]) << std::endl;
        }
    }

    return are_all_entries_verified;
}

// ----------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    int return_code = 0;

    Cpu_Info initial_cpu_state;
    read_cpu_state_from_stdin(initial_cpu_state);

    GameBoy gameboy;
    Uint8* ram = gameboy.get_ram_contents();
    gameboy.override_cpu_state(initial_cpu_state);
    override_ram_entries_from_stdin(ram);

    gameboy.execute_next_instruction();

    Cpu_Info expected_final_cpu_state;
    read_cpu_state_from_stdin(expected_final_cpu_state);
    Cpu_Info actual_final_cpu_state = gameboy.get_cpu_info();
    if (!verify_cpu_state(expected_final_cpu_state, actual_final_cpu_state)) {
        return_code = 1;
    }

    if (!verify_ram_entries_from_stdin(ram)) {
        return_code = 1;
    }

    return return_code;
}

//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include <stdint.h>
#include <stdlib.h>

// Registers memory
int32_t registers[32] = {
    //zero      ra          sp
    0x00000000, 0x00000000, 0x00000000,
    //gp        tp          t0
    0x00000000, 0x00000000, 0x00000000,
    //t1        t2          s0
    0x00000004, 0x00000004, 0x00000000,
    //s1        a0          a1
    0x00000000, 0x00000000, 0x00000000,
    //a2        a3          a4
    0x00000000, 0x00000000, 0x00000000,
    //a5        a6          a7
    0x00000000, 0x00000000, 0x00000000,
    //s2        s3          s4
    0x00000000, 0x00000000, 0x00000000,
    //s5        s6          s7
    0x00000000, 0x00000000, 0x00000000,
    //s8        s9          s10
    0x00000000, 0x00000000, 0x00000000,
    //s11       t3          t4
    0x00000000, 0x00000000, 0x00000000,
    //t5        t6
    0x00000000, 0x00000000

};

int32_t getValueRegister(const uint32_t registerNumber) {
    if (registerNumber >= 32) return 0;

    return registers[registerNumber];
}

bool writeRegister(const uint32_t registerNumber, const int32_t value) {
    if (registerNumber >= 32) return false;

    registers[registerNumber] = value;

    return true;

}
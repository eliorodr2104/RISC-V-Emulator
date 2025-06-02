//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include <stdint.h>
#include <stdlib.h>

// Registers memory
int32_t registers[32] = {
    0x00000000, 0x00000000,
    0x00000000, 0x00000000,
    0x00000000, 0x00000000,
    0x00000000, 0x00000000,
    0x00000000, 0x00000000,
    0x00000000, 0x00000000,
    0x00000000, 0x00000000,
    0x00000000, 0x00000000,
    0x00000000, 0x00000000,
    0x00000000, 0x00000000,
    0x00000000, 0x00000000,
    0x00000000, 0x00000000,
    0x00000000, 0x00000000,
    0x00000000, 0x00000000,
    0x00000000, 0x00000000,
    0x00000000

};

bool* getValueRegister(const uint32_t registerNumber) {
    if (registerNumber >= 32) return nullptr;

    bool* bits = malloc(32 * sizeof(bool));
    if (!bits) return nullptr;

    for (int i = 0; i < 32; i++) {
        bits[i] = (registers[registerNumber] >> (31 - i)) & 1;
    }

    return bits;
}

bool writeRegister(const uint32_t registerNumber, const int32_t value) {
    if (registerNumber >= 32) return false;

    registers[registerNumber] = value;

    return true;

}
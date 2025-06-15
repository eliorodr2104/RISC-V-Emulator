//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef REGISTERMEMORY_H
#define REGISTERMEMORY_H
#include <stdint.h>

extern int32_t registers[32];

static int32_t getValueRegister(const uint32_t registerNumber) {
    return (registerNumber >= 32 ? 0 : registers[registerNumber]);
}

static bool writeRegister(
    const uint32_t registerNumber,
    const int32_t value
) {
    if (registerNumber == 0) return false;
    return registerNumber < 32 ? (registers[registerNumber] = value, true) : false;
}

#endif //REGISTERMEMORY_H

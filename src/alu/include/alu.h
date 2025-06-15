//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef ALU_H
#define ALU_H
#include <stdint.h>

typedef struct {
    bool result;
    bool carryOut;
} AluResult;

typedef struct {
    int32_t result;
    bool zero;
    bool overflow;
} Alu32BitResult;

AluResult alu1bit (
    bool a,
    bool b,
    bool less,
    bool carryIn,
    uint8_t invertControl,
    uint8_t aluOp
);

Alu32BitResult alu32bit(
    int32_t a,
    int32_t b,
    bool less,
    uint8_t operation
);

static int bitsToShiftAmount(const int32_t b) {
    return b & 0x1F;
}

static int twoBitsToDecimal(const bool a, const bool b) {
    return (a << 1) | b;
}

#endif //ALU_H

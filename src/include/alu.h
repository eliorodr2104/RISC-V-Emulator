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
    uint32_t result;
    bool zero;
    bool overflow;
} Alu32BitResult;

int twoBitsToDecimal(bool a, bool b);

AluResult alu1bit (bool a, bool b, bool less, bool carryIn, const bool operation[5], uint32_t aluOp);

Alu32BitResult alu32bit(
    uint32_t a,
    uint32_t b,
    bool less,
    bool operation[5]
);

int bitsToLeftShiftAmount(uint32_t b);
int bitsToRightShiftAmount(uint32_t b);

#endif //ALU_H

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
    bool result[32];
    bool zero;
    bool overflow;
} Alu32BitResult;

int twoBitsToDecimal(bool a, bool b);

AluResult alu1bit (bool a, bool b, bool less, bool carryIn, const bool operation[5], uint32_t aluOp);

Alu32BitResult alu32bit(
    const bool a[32],
    const bool b[32],
    bool less,
    bool operation[5]
);

int bitsToLeftShiftAmount(const bool b[32]);
int bitsToRightShiftAmount(const bool b[32]);

#endif //ALU_H

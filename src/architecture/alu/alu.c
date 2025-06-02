//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include "alu.h"

#include <stdint.h>
#include <stdio.h>

#include "tools.h"
#include "tui.h"

inline int twoBitsToDecimal(const bool a, const bool b) {
    return (a << 1) | b;
}

AluResult alu1bit(
    bool a,
    bool b,
    const bool less,
    const bool carryIn,
    const bool operation[5],
    const uint32_t aluOp
) {
    const bool binvert           = operation[1];
    const bool ainvert           = operation[0];

    AluResult result;

    switch (aluOp) {

        // Operation
        // 0000 -> AND
        // 1100 -> NOR
        case 0:

            if (ainvert) a = a ^ 1;
            if (binvert) b = b ^ 1;

            result.result   = a & b;
            result.carryOut = false;

            break;

        // Operation
        // 0001 -> OR
        case 1:

            result.result   = a | b;
            result.carryOut = false;

            break;

        // Operation
        // 0010 -> AND
        // 0110 -> SUB
        case 2:

            if (ainvert) a = a ^ 1;
            if (binvert) b = b ^ 1;

            result.result   = a ^ b ^ carryIn;
            result.carryOut = (a & b) | (carryIn & (a ^ b));

            break;
        // Operation
        // 0011 -> Set less than
        case 3:
            result.result   = less;
            result.carryOut = false;

            break;

        case 7:

            result.result   = a ^ b;
            result.carryOut = false;

            break;

        default:
            result.result = false;
            result.carryOut = false;

            break;

    }

    return result;
}

Alu32BitResult alu32bit(
    uint32_t a,
    uint32_t b,
    const bool less,
          bool operation[5]
) {
    Alu32BitResult alu32BitResult;
    bool carryIn = operation[1];
    const uint32_t  aluOperation = bitsToInt(3, (bool[]){ operation[2], operation[3], operation[4] });

    // Shift Left Aritmetic
    if (aluOperation == 4) {
        const int shamt = bitsToLeftShiftAmount(b);
        alu32BitResult.result = a << shamt;
        alu32BitResult.overflow = false;
        goto CONTROL_ZERO;
    }

    // Shift Right Logical
    if (aluOperation == 5) {
        const int shamt = bitsToRightShiftAmount(b);
        alu32BitResult.result = a >> shamt;
        alu32BitResult.overflow = false;
        goto CONTROL_ZERO;
    }

    // Shift Right Aritmetic
    if (aluOperation == 6) {
        const int shamt = bitsToRightShiftAmount(b);
        const bool sign = getBit(a, 31);
        uint32_t shifted = a >> shamt;

        if (sign) {
            const uint32_t mask = ~((1u << (32 - shamt)) - 1);
            shifted |= mask;
        }

        printf("b: %d\n", b);
        printf("a: %d\n", a);
        printf("sign: %d\n", sign);
        printf("new a: %d\n", shifted);

        alu32BitResult.result = shifted;
        alu32BitResult.overflow = false;
        goto CONTROL_ZERO;
    }

    uint32_t resultBits = 0;
    bool carryInOverflowControl  = 0;
    bool carryOutOverflowControl = 0;

    for (int i = 0; i < 32; i++) {
        const bool bitA = getBit(a, i);
        const bool bitB = getBit(b, i);
        const bool lessInput = (i == 31) ? less : false;

        const AluResult res = alu1bit(bitA, bitB, lessInput, carryIn, operation, aluOperation);
        setBit(&resultBits, i, res.result);

        carryIn = res.carryOut;

        if (i == 1) carryInOverflowControl = carryIn;
        if (i == 0) carryOutOverflowControl = res.carryOut;
    }

    alu32BitResult.result = resultBits;
    alu32BitResult.overflow = carryInOverflowControl ^ carryOutOverflowControl;

CONTROL_ZERO:
    alu32BitResult.zero = alu32BitResult.result == 0;

    return alu32BitResult;
}

int bitsToLeftShiftAmount(const uint32_t b) {
    return b & 0x1F;
}

int bitsToRightShiftAmount(const uint32_t b) {
    return b & 0x1F;
}
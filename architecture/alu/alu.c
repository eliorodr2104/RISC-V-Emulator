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
    const bool a[32],
    const bool b[32],
    const bool less,
          bool operation[5]
) {
    Alu32BitResult alu32BitResult;
    bool carryIn = operation[1];

    const uint32_t  aluOperation = bitsToInt(3, (bool[]){ operation[2], operation[3], operation[4] });

    // Shift Left Aritmetic
    if (aluOperation == 4) {
        const int shamt = bitsToLeftShiftAmount(b);
        for (int i = 0; i < 32; i++) {
            if (i + shamt < 32)
                alu32BitResult.result[i] = a[i + shamt];
            else
                alu32BitResult.result[i] = 0;
        }

        alu32BitResult.overflow = false;

        goto CONTROL_ZERO;
    }

    // Shift Right Logical
    if (aluOperation == 5) {
        const int shamt = bitsToRightShiftAmount(b);
        for (int i = 31; i >= 0; i--) {
            if (i - shamt >= 0)
                alu32BitResult.result[i] = a[i - shamt];
            else
                alu32BitResult.result[i] = 0;
        }

        alu32BitResult.overflow = false;
        goto CONTROL_ZERO;
    }

    // Shift Right Aritmetic
    if (aluOperation == 6) {
        const int shamt = bitsToRightShiftAmount(b);
        const bool sign = a[0];

        for (int i = 31; i >= 0; i--) {
            if (i - shamt >= 0)
                alu32BitResult.result[i] = a[i - shamt];
            else
                alu32BitResult.result[i] = sign;
        }

        alu32BitResult.overflow = false;
        goto CONTROL_ZERO;
    }

    bool carryInOverflowControl  = 0;
    bool carryOutOverflowControl = 0;

    for (int i = 31; i >= 0; i--) {

        const bool lessInput = (i == 31) ? less : false;
        const AluResult result1Bit = alu1bit(a[i], b[i], lessInput, carryIn, operation, aluOperation);
        alu32BitResult.result[i] = result1Bit.result;

        carryIn = result1Bit.carryOut;

        if (i == 1) carryInOverflowControl  = carryIn;
        if (i == 0) carryOutOverflowControl = result1Bit.carryOut;
    }

    alu32BitResult.overflow = carryInOverflowControl ^ carryOutOverflowControl;

    CONTROL_ZERO:

    alu32BitResult.zero     = true;
    for (int i = 0; i < 32; i++) {
        if (alu32BitResult.result[i]) {
            alu32BitResult.zero = false;
            break;

        }
    }

    return alu32BitResult;
}

int bitsToLeftShiftAmount(const bool b[32]) {
    int shamt = 0;
    for (int i = 27; i < 32; i++) {
        shamt <<= 1;
        shamt |= b[i];
    }
    return shamt;
}

int bitsToRightShiftAmount(const bool b[32]) {
    int shamt = 0;
    for (int i = 27; i < 32; i++) {
        shamt <<= 1;
        shamt |= b[i];
    }
    return shamt;
}
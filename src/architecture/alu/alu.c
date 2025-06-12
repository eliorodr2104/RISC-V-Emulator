//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include "alu.h"

#include <stdio.h>

#include "tools.h"
#include "../../include/tui/cpu/tuiCpu.h"

AluResult alu1bit(
    bool a,
    bool b,
    const bool less,
    const bool carryIn,
    const uint8_t invertControl,
    const uint8_t aluOp
) {
    const bool binvert = getBit(invertControl, 3); //operation[1];
    const bool ainvert = getBit(invertControl, 4); //operation[0];

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
        // 0010 -> ADD
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
    const int32_t a,
    const int32_t b,
    const bool less,
    const uint8_t operation
) {
    Alu32BitResult alu32BitResult = { 0, 0, 0};

    if (operation == 0xE) return alu32BitResult;

          bool carryIn          = getBit(operation, 3);
    const uint32_t aluOperation = (operation & 0b111); //bitsToInt(3, (bool[]){ operation[2], operation[3], operation[4] });

    // Shift Left Arithmetic
    if (aluOperation == 4) {
        alu32BitResult.result = a << bitsToShiftAmount(b);
        goto CONTROL_ZERO;
    }

    // Shift Right Logical
    if (aluOperation == 5) {
        alu32BitResult.result = a >> bitsToShiftAmount(b);
        goto CONTROL_ZERO;
    }

    // Shift Right Arithmetic
    if (aluOperation == 6) {
        const int shiftAmount = bitsToShiftAmount(b);

        alu32BitResult.result = a >> shiftAmount;
        goto CONTROL_ZERO;
    }

    int32_t resultBits = 0;
    bool carryInOverflowControl  = 0;
    bool carryOutOverflowControl = 0;

    for (int i = 0; i < 32; i++) {
        const bool bitA      = getBit(a, i);
        const bool bitB      = getBit(b, i);
        const bool lessInput = (i == 31) ? less : false;

        const AluResult res  = alu1bit(bitA, bitB, lessInput, carryIn, operation, aluOperation);
        setBit(&resultBits, i, res.result);

        carryIn = res.carryOut;

        if (i == 1) carryInOverflowControl  = carryIn;
        if (i == 0) carryOutOverflowControl = res.carryOut;
    }

    alu32BitResult.result = resultBits;

CONTROL_ZERO:
    alu32BitResult.zero     = alu32BitResult.result == 0;
    alu32BitResult.overflow = (aluOperation >= 4 && aluOperation <= 6 ? false : carryInOverflowControl ^ carryOutOverflowControl);

    return alu32BitResult;
}
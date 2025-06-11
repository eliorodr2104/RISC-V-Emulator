//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include <stdint.h>
#include "controlUnit.h"

ControlSignals getControlSignals(
    const uint8_t opcode
) {
    ControlSignals signals = { 0 };

    switch (opcode) {

        // R-Type: ADD, SUB, AND, OR, etc. (opcode = 0x33)
        case 0x33:
            signals.branch    = false;
            signals.memRead   = false;
            signals.memToReg  = false;
            signals.operation = 0x33;      // Operation code
            signals.memWrite  = false;
            signals.aluSrc    = false;     // usa registro rs2
            signals.regWrite  = true;      // scrive risultato in rd
            break;

        // I-Type Arithmetic: ADDI, ANDI, ORI, etc. (opcode = 0x13)
        case 0x67:
        case 0x13:
            signals.branch    = false;
            signals.memRead   = false;
            signals.memToReg  = false;
            signals.operation = 0x13;      // Operation code
            signals.memWrite  = false;
            signals.aluSrc    = true;      // usa immediato
            signals.regWrite  = true;      // scrive risultato in rd
            break;

        // UJ-Type: jal. (opcode = 0x6F)
        case 0x6F:
            signals.branch    = false;
            signals.memRead   = false;
            signals.memToReg  = false;
            signals.operation = 0x6F;      // Operation code
            signals.memWrite  = false;
            signals.aluSrc    = true;      // usa immediato
            signals.regWrite  = true;      // scrive risultato in rd
            break;

        // Store: SW, SH, SB, etc. (opcode = 0x23)
        case 0x23: // STORE
            signals.branch    = false;
            signals.memRead   = false;
            signals.memToReg  = false;     // non importa, non scrive nel registro
            signals.operation = 0x23;
            signals.memWrite  = true;      // scrive in memoria
            signals.aluSrc    = true;      // usa immediato per offset
            signals.regWrite  = false;     // non scrive nei registri
            break;

        case 0x73:
            signals.branch    = false;
            signals.memRead   = false;
            signals.memToReg  = false;     // non importa, non scrive nel registro
            signals.operation = 0x73;
            signals.memWrite  = false;      // scrive in memoria
            signals.aluSrc    = false;      // usa immediato per offset
            signals.regWrite  = false;     // non scrive nei registri
            break;

        // Branch: BEQ, BNE, BLT, etc. (opcode = 0x63)
        //case 0x67: // BRANCH
        //    signals.branch    = true;      // istruzione di branch
        //    signals.memRead   = false;
        //    signals.memToReg  = false;     // non importa, non scrive nel registro
        //    signals.operation = 0b1100111; // 01 = SUB per confronto
        //    signals.memWrite  = false;
        //    signals.aluSrc    = false;     // usa registro rs2 per confronto
        //    signals.regWrite  = false;     // non scrive nei registri
        //    break;

        default:
            break;
    }

    return signals;
}

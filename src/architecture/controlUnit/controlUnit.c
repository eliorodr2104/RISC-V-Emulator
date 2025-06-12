//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include <stdint.h>
#include "controlUnit.h"

/**
 * @brief Get control signals based on the opcode
 *
 * @param opcode The opcode of the instruction
 * @return ControlSignals structure with the control signals set
 */
ControlSignals getControlSignals(
    const uint8_t opcode
) {

    // Initialize control signals to default values
    ControlSignals signals = { 0 };

    // Set default values for control signals
    switch (opcode) {

        // R-Type: ADD, SUB, AND, OR, etc. (opcode = 0x33)
        case 0x33:
            signals.branch    = false; // non-branch instruction
            signals.memRead   = false; // non-memory read instruction
            signals.memToReg  = false; // non-memory to register instruction
            signals.operation = 0x33;  // Operation code
            signals.memWrite  = false; // non-memory write instruction
            signals.aluSrc    = false; // non immediate value, uses rs2
            signals.regWrite  = true;  // writes result in rd register
            break;

        // I-Type Arithmetic: ADDI, ANDI, ORI, etc. (opcode = 0x13)
        case 0x67:
        case 0x13:
            signals.branch    = false; // non-branch instruction
            signals.memRead   = false; // non-memory read instruction
            signals.memToReg  = false; // non-memory to register instruction
            signals.operation = 0x13;  // Operation code
            signals.memWrite  = false; // non-memory write instruction
            signals.aluSrc    = true;  // uses immediate value for offset
            signals.regWrite  = true;  // writes result in rd register
            break;

        // UJ-Type: jal. (opcode = 0x6F)
        case 0x6F:
            signals.branch    = false; // non-branch instruction
            signals.memRead   = false; // non-memory read instruction
            signals.memToReg  = false; // non-memory to register instruction
            signals.operation = 0x6F;  // Operation code
            signals.memWrite  = false; // non-memory write instruction
            signals.aluSrc    = true;  // uses immediate value for offset
            signals.regWrite  = true;  // writes result in rd register
            break;

        // Store: SW, SH, SB, etc. (opcode = 0x23)
        case 0x23: // STORE
            signals.branch    = false; // non-branch instruction
            signals.memRead   = false; // non-memory read instruction
            signals.memToReg  = false; // non-memory to register instruction
            signals.operation = 0x23;  // Operation code
            signals.memWrite  = true;  // writes data to memory
            signals.aluSrc    = true;  // uses immediate value for offset
            signals.regWrite  = false; // does not write to registers
            break;

        case 0x73:
            signals.branch    = false; // non-branch instruction
            signals.memRead   = false; // non-memory read instruction
            signals.memToReg  = false; // non-memory to register instruction
            signals.operation = 0x73;  // Operation code for ecall
            signals.memWrite  = false; // non-memory write instruction
            signals.aluSrc    = false; // does not use immediate value
            signals.regWrite  = false; // does not write to registers
            break;

        // Default case for unrecognized opcodes -> instructions
        default:
            break;
    }

    return signals;
}

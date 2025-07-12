//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include <stdint.h>
#include "include/control_unit.h"

/**
 * @brief Get control signals based on the opcode
 *
 * @param opcode The opcode of the instruction
 *
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
            signals.branch     = false; // non-branch instruction
            signals.mem_read   = false; // non-memory read instruction
            signals.mem_to_reg = false; // non-memory to register instruction
            signals.operation  = 0x33;  // Operation code
            signals.mem_write  = false; // non-memory write instruction
            signals.alu_src    = false; // non immediate value, uses rs2
            signals.reg_write  = true;  // writes result in rd register
            break;

        // I-Type Arithmetic: ADDI, ANDI, ORI, etc. (opcode = 0x13)
        case 0x67:
        case 0x13:
            signals.branch     = false; // non-branch instruction
            signals.mem_read   = false; // non-memory read instruction
            signals.mem_to_reg = false; // non-memory to register instruction
            signals.operation  = 0x13;  // Operation code
            signals.mem_write  = false; // non-memory write instruction
            signals.alu_src    = true;  // uses immediate value for offset
            signals.reg_write  = true;  // writes result in rd register
            break;

        // I-Type Load: LW, LH, LB, etc. (opcode = 0x03)
        case 0x03:
            signals.branch     = false; // non-branch instruction
            signals.mem_read   = true;  // memory read instruction
            signals.mem_to_reg = false; // non-memory to register instruction
            signals.operation  = 0x03;  // Operation code
            signals.mem_write  = false; // non-memory write instruction
            signals.alu_src    = true;  // uses immediate value for offset
            signals.reg_write  = false; // writes result in rd register
            break;

        // UJ-Type: jal. (opcode = 0x6F)
        case 0x6F:
            signals.branch     = false; // non-branch instruction
            signals.mem_read   = false; // non-memory read instruction
            signals.mem_to_reg = false; // non-memory to register instruction
            signals.operation  = 0x6F;  // Operation code
            signals.mem_write  = false; // non-memory write instruction
            signals.alu_src    = true;  // uses immediate value for offset
            signals.reg_write  = true;  // writes result in rd register
            break;

        // Store: SW, SH, SB, etc. (opcode = 0x23)
        case 0x23: // STORE
            signals.branch     = false; // non-branch instruction
            signals.mem_read   = false; // non-memory read instruction
            signals.mem_to_reg = false; // non-memory to register instruction
            signals.operation  = 0x23;  // Operation code
            signals.mem_write  = true;  // writes data to memory
            signals.alu_src    = true;  // uses immediate value for offset
            signals.reg_write  = false; // does not write to registers
            break;

        case 0x73:
            signals.branch     = false; // non-branch instruction
            signals.mem_read   = false; // non-memory read instruction
            signals.mem_to_reg = false; // non-memory to register instruction
            signals.operation  = 0x73;  // Operation code for ecall
            signals.mem_write  = false; // non-memory write instruction
            signals.alu_src    = false; // does not use immediate value
            signals.reg_write  = false; // does not write to registers
            break;

        case 0x37:
            signals.branch     = false; // non-branch instruction
            signals.mem_read   = false; // non-memory read instruction
            signals.mem_to_reg = false; // non-memory to register instruction
            signals.operation  = 0x37;  // Operation code for ecall
            signals.mem_write  = false; // non-memory write instruction
            signals.alu_src    = false;  // use immediate value
            signals.reg_write  = true;  // write to registers

            break;

        case 0x17:
            signals.branch     = false; // non-branch instruction
            signals.mem_read   = false; // non-memory read instruction
            signals.mem_to_reg = false; // non-memory to register instruction
            signals.operation  = 0x17;  // Operation code for ecall
            signals.mem_write  = false; // non-memory write instruction
            signals.alu_src    = true;  // use immediate value
            signals.reg_write  = true;  // write to registers

            break;

        // Default case for unrecognized opcodes -> instructions
        default:
            break;
    }

    return signals;
}

//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H
#include <stdint.h>

/**
 * @brief Struct to hold control signals for the CPU
 *
 * @var ControlSignals::branch
 * Indicates if the instruction is a branch instruction
 * @var ControlSignals::memRead
 * Indicates if the instruction reads from memory
 * @var ControlSignals::memToReg
 * Indicates if the instruction writes to a register from memory
 * @var ControlSignals::operation
 * The operation code for ALU operations
 * @var ControlSignals::memWrite
 * Indicates if the instruction writes to memory
 * @var ControlSignals::aluSrc
 * Indicates if the ALU uses an immediate value as source
 * @var ControlSignals::regWrite
 * Indicates if the instruction writes to a register
 */
typedef struct {
    bool    branch;
    bool    memRead;
    bool    memToReg;
    uint8_t operation;
    bool    memWrite;
    bool    aluSrc;
    bool    regWrite;

} ControlSignals;

/**
 * @brief Get control signals based on the opcode
 *
 * @param opcode The opcode of the instruction
 * @return ControlSignals structure with the control signals set
 */
ControlSignals getControlSignals(
    uint8_t opcode
);

#endif //CONTROLUNIT_H

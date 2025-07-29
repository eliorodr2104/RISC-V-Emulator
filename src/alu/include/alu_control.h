/**
 * @file alu_control.h
 * @brief Header file for the control of the ALU operations.
 *
 * @author eliorodr2104
 * @date 02/06/25
 *
 * This file defines the ALU operations and provides functions to get the
 */

#ifndef ALUCONTROL_H
#define ALUCONTROL_H

#include <stdint.h>

/**
 * @brief Enum to represent the ALU operations.
 *
 * This enum defines the various operations that the ALU can perform,
 * including arithmetic, logical, and shift operations.
 */
typedef enum {
    ALU_ADD,
    ALU_SUB,
    ALU_AND,
    ALU_OR,
    ALU_XOR,
    ALU_SLT,
    ALU_SLL,
    ALU_SRL,
    ALU_SRA,

    ALU_ADDI,
    ALU_ANDI,
    ALU_ORI,
    ALU_XORI,
    ALU_SLTI,
    ALU_SLLI,
    ALU_SRLI,
    ALU_SRAI,

    ALU_LB,
    ALU_LH,
    ALU_LW,
    ALU_LBU,
    ALU_LHU,

    LUI,
    ALU_AUIPC,

    ALU_SKIP,
    ALU_UNKNOWN

} AluEnumOperation;

/**
 * @brief Struct for representing current information and state on the ALU.
 */
typedef struct  {
    AluEnumOperation enumOperation;
    uint8_t          operation;

} AluTuple;

/**
 * @brief Get the ALU operation bits and enum.
 *
 * @param alu_op The opcode of the instruction.
 * @param funz3 The funct3 field of the instruction.
 * @param funz7_bit30 The 30th bit of the funct7 field.
 * @param alu_tuple Contains alu operation information
 */
void get_alu_operation(
    uint8_t   alu_op,
    uint8_t   funz3,
    uint8_t   funz7_bit30,
    AluTuple* alu_tuple
);

#endif //ALUCONTROL_H

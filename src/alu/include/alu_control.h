/**
 * @file alu-control.h
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

    ALU_SKIP,
    ALU_UNKNOWN

} AluOp;

/**
 * @brief Get the ALU control operation based on the opcode, funct3, and funct7 bit 30.
 *
 * @param alu_op The opcode of the instruction.
 * @param funz3 The funct3 field of the instruction.
 * @param funz7_bit30 The 30th bit of the funct7 field.
 *
 * @return AluOp The ALU operation to be performed.
 */
AluOp get_alu_control(
    uint8_t alu_op,
    uint8_t funz3,
    uint8_t funz7_bit30
);

/**
 * @brief Get the ALU operation bits based on the ALU operation enum.
 *
 * @param alu_op_enum The ALU operation enum.
 * @param operation Pointer to store the operation bits.
 */
void get_alu_operation_bits(
    AluOp    alu_op_enum,
    uint8_t* operation

);

#endif //ALUCONTROL_H

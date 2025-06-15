//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

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
 * @param aluOp The opcode of the instruction.
 * @param funct3 The funct3 field of the instruction.
 * @param funct7Bit30 The 30th bit of the funct7 field.
 * @return AluOp The ALU operation to be performed.
 */
AluOp getAluControl(
    uint8_t aluOp,
    uint8_t funct3,
    uint8_t funct7Bit30
);

/**
 * @brief Get the ALU operation bits based on the ALU operation enum.
 *
 * @param aluOpEnum The ALU operation enum.
 * @param operation Pointer to store the operation bits.
 */
void getAluOperationBits(AluOp aluOpEnum, uint8_t* operation);

#endif //ALUCONTROL_H

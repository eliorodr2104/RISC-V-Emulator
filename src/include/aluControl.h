//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef ALUCONTROL_H
#define ALUCONTROL_H
#include <stdint.h>

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
    ALU_UNKNOWN
} AluOp;

AluOp getAluControl(
    uint8_t aluOp,
    uint8_t funct3,
    uint8_t funct7Bit30
);

void getAluOperationBits(AluOp aluOpEnum, uint8_t* operation);

#endif //ALUCONTROL_H

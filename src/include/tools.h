//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef TOOLS_H
#define TOOLS_H

#include "aluControl.h"
#include "instructionMemory.h"

static bool getBit(const uint32_t x, const int i) {
    return (x >> i) & 1u;
}

static void setBit(int32_t* x, const int i, const bool val) {
    *x = (val ? *x | (1u << i) : *x & ~(1u << i)) ;
}

static uint32_t extractBits(const uint32_t instruction, const int start, const int end) {
    return instruction >> start & ((1U << (end - start + 1)) - 1);
}

// Estensione del segno per immediati
static int32_t signExtend(const uint32_t value, const int bits) {
    const int shift = 32 - bits;
    return (int32_t)(value << shift) >> shift;
}

void print_binary(uint32_t bin);
void formatInstruction(DecodedInstruction decoded, char* buffer, size_t size);
const char* getInstructionName(uint8_t opcode, uint8_t funct3, uint8_t funct7Bit30);

AluOp getInstructionEnum(uint8_t opcode, uint8_t funct3, uint8_t funct7Bit30);

#endif //TOOLS_H

//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef TUI_H
#define TUI_H
#include "instructionMemory.h"

extern const char* register_names[32];

void printBinWord(bool word[32]);
void printHexWord(const bool word[32]);
void printDecodedInstruction(uint32_t instruction, DecodedInstruction decoded);

void printDebugInfo(
    int aluOp,
    bool operation[4],
    bool numberA[32],
    bool numberB[32],
    bool result[32],
    int overflow,
    int zero
);

void printRegisterTable();
void printRegisterTableCompact();
void printNonZeroRegisters();

#endif //TUI_H

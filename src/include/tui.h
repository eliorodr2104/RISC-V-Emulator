//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef TUI_H
#define TUI_H
#include "instructionMemory.h"

extern const char* register_names[32];

// ########################
// # Main print functions #
// ########################
void headerProgram();
void userChoices();

// ########################
// # CPU print functions  #
// ########################
void printRegisterTable();
void printFinalRegisterTable();
void commandList();

void printStepProgramCounter(int32_t pc);

void printHeaderInstructionStatus(int32_t pc);
void printInstructionStatus(DecodedInstruction decoded, int32_t input1, int32_t input2, int32_t output, int32_t pc);

void printProgramWithCurrentInstruction(
    int32_t input1,
    int32_t input2,
    int32_t result,
    int32_t pc
);

// #########################
// # Debug print functions #
// #########################

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

void printRegisterTableCompact();
void printNonZeroRegisters();

#endif //TUI_H

//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef TUI_H
#define TUI_H
#include "../../instructionMemory.h"
#include "ncurses.h"

extern const char* register_names[32];

// ########################
// # CPU print functions  #
// ########################
void printRegisterTable(
    WINDOW* winRegs
);

void printProgramWithCurrentInstruction(
    WINDOW* winProg,
    WINDOW* winRegs,
    int32_t input1,
    int32_t input2,
    int32_t result,
    int32_t pc
);

#endif //TUI_H

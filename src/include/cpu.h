//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef CPU_H
#define CPU_H
#include <ncurses.h>

#include "windows.h"

typedef struct {
    int32_t pc;

} Cpu;

Cpu* newCpu();

void runCpuFull(
    WINDOW* winProg,
    WINDOW* winRegs,
    WINDOW*  winStatus,
    WINDOW*  winCmd,
    Windows* window,
    Cpu* cpu
);

void runCpuStepByStep(
    WINDOW*  winProg,
    WINDOW*  winRegs,
    WINDOW*  winStatus,
    WINDOW*  winCmd,
    Windows* window,
    Cpu* cpu
);

int executeSingleStep(
    WINDOW*  winProg,
    WINDOW*  winRegs,
    WINDOW*  winStatus,
    WINDOW*  winCmd,
    Windows* window,
    int    * currentChar,
    Cpu* cpu,
    bool interactive
);

#endif //CPU_H

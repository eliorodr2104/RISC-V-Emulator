//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef CPU_H
#define CPU_H
#include <ncurses.h>

#include "args_handler.h"
#include "assemblyData.h"
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
    Cpu* cpu,
    options_t options
);

void runCpuStepByStep(
    WINDOW*  winProg,
    WINDOW*  winRegs,
    WINDOW*  winStatus,
    WINDOW*  winCmd,
    Windows* window,
    Cpu* cpu,
    options_t options
);

int executeSingleStep(
    WINDOW*  winProg,
    WINDOW*  winRegs,
    WINDOW*  winStatus,
    WINDOW*  winCmd,
    Windows* window,
    int    * currentChar,
    Cpu* cpu,
    options_t options,
    AssemblyData* data,
    bool interactive
);

#endif //CPU_H

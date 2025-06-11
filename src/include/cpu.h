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
    int32_t resetFlag;

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
    const AssemblyData* data,
    bool interactive
);

void reExecuteUntilTarget(Cpu* cpu, options_t options);

void executeInstructionSilently(Cpu* cpu, options_t options);

void resetCpuState(Cpu* cpu);

#endif //CPU_H

//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef CPU_H
#define CPU_H
#include <ncurses.h>

typedef struct {
    int32_t pc;

} Cpu;

Cpu* newCpu();

void runCpuFull(
    WINDOW* winProg,
    WINDOW* winRegs,
    Cpu* cpu
);

void runCpuStepByStep(
    WINDOW* winProg,
    WINDOW* winRegs,
    WINDOW* winStatus,
    Cpu* cpu
);

int executeSingleStep(
    WINDOW* winProg,
    WINDOW* winRegs,
    WINDOW* winStatus,
    Cpu* cpu,
    bool interactive
);

void runCpu(Cpu* cpu);

#endif //CPU_H

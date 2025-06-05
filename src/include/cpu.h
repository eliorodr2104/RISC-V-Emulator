//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef CPU_H
#define CPU_H
#include <stdlib.h>

typedef struct {
    int32_t pc;

} Cpu;

Cpu* newCpu();

void runCpuFull(Cpu* cpu);
void runCpuStepByStep(Cpu* cpu);
int executeSingleStep(Cpu* cpu, bool interactive);

void runCpu(Cpu* cpu);

#endif //CPU_H

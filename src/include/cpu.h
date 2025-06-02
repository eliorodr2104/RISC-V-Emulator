//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef CPU_H
#define CPU_H
#include <stdlib.h>

typedef struct {
    uint32_t pc;

} Cpu;

Cpu* newCpu();
void runCpu(Cpu* cpu);

#endif //CPU_H

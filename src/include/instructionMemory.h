//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef INSTRUCTIONMEMORY_H
#define INSTRUCTIONMEMORY_H

#define MAX_INSTRUCTIONS 1
#include "cpu.h"

typedef struct {
    uint8_t  opcode;        // [6-0]   - Unità di controllo
    uint8_t  rs1;           // [19-15] - Registro di lettura 1
    uint8_t  rs2;           // [24-20] - Registro di lettura 2
    uint8_t  rd;            // [11-7]  - Registro di scrittura
    int32_t  immediate;     // [31-0]  - Estensione del segno (varia per tipo)
    uint8_t  funct3;        // [14-12] - Parte del controllore ALU
    uint8_t  funct7Bit30;   // [30]    - Bit 30 per controllore ALU
} DecodedInstruction;

extern uint32_t instructions[];

uint32_t           fetchInstruction (Cpu* cpu);
DecodedInstruction decodeInstruction(uint32_t instruction);

#endif //INSTRUCTIONMEMORY_H

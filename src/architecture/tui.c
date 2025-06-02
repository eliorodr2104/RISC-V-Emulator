//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include <stdint.h>
#include <stdio.h>

#include "../include/instructionMemory.h"
#include "../include/registerMemory.h"

const char* register_names[32] = {
    "zero", "ra",  "sp",  "gp",  "tp",  "t0",  "t1",  "t2",
    "s0",   "s1",  "a0",  "a1",  "a2",  "a3",  "a4",  "a5",
    "a6",   "a7",  "s2",  "s3",  "s4",  "s5",  "s6",  "s7",
    "s8",   "s9",  "s10", "s11", "t3",  "t4",  "t5",  "t6"
};

void printBinWord(bool word[32]) {
    for (int i = 0; i < 32; i++) {
        printf("%d", word[i]);
    }
}

void printHexWord(const bool word[32]) {
    printf("0x");

    for (int i = 0; i < 32; i += 4) {
        int hexDigit = 0;
        for (int j = 0; j < 4; j++) {
            if (word[i + j]) {
                hexDigit |= (1 << (3 - j));
            }
        }

        printf("%X", hexDigit);
    }
}

void printDebugInfo(
    const int aluOp,
          bool operation[4],
          bool numberA[32],
          bool numberB[32],
          bool result[32],
    const int overflow,
    const int zero
) {
    printf("\n=== DEBUG INFO ===\n");
    printf("AluOp: %d, binvert: %d, carryIn iniziale: %d\n", aluOp, operation[1], operation[1]);

    printf("\n%-20s", "A (bit 0-31):");
    printHexWord(numberA);

    printf("\n%-20s", "B (bit 0-31):");
    printHexWord(numberB);

    printf("\n%-20s", "Result (bit 0-31):");
    printHexWord(result);

    printf("\n\n%-25s%d", "Output Overflow:", overflow);
    printf("\n%-25s%d", "Output Zero:", zero);
    printf("\n==================\n");
}

void printDecodedInstruction(uint32_t instruction, DecodedInstruction decoded) {
    printf("\n=== DECODED INSTRUCTION ===\n");
    printf("Raw instruction:     0x%08X\n", instruction);
    printf("Opcode [6-0]:        0x%02X (%d)\n", decoded.opcode, decoded.opcode);
    printf("RS1 [19-15]:         %d\n", decoded.rs1);
    printf("RS2 [24-20]:         %d\n", decoded.rs2);
    printf("RD [11-7]:           %d\n", decoded.rd);
    printf("Immediate:           %d (0x%08X)\n", decoded.immediate, (uint32_t)decoded.immediate);
    printf("Funct3 [14-12]:      0x%X (%d)\n", decoded.funct3, decoded.funct3);
    printf("Funct7[30]:          %d\n", decoded.funct7Bit30);
    printf("===========================\n");
}

void printRegisterTable() {
    printf("┌───────────┬──────────┬────────────┬────────────┐\n");
    printf("│ Registers │   Nome   │    Hex     │    Dec     │\n");
    printf("├───────────┼──────────┼────────────┼────────────┤\n");

    for (int i = 0; i < 32; i++) {
        const char* bg = (i % 2 == 0) ? "\033[100m" : "\033[0m"; // sfondo alternato
        const char* green = (registers[i] != 0) ? "\033[32m" : "";
        const char* reset = "\033[0m";

        printf("│%s x%-7d  │ %-8s │ %s0x%08X%s%s │ %s%10d%s%s %s│\n",
               bg, i, register_names[i],
               green, registers[i], reset, bg,
               green, registers[i], reset, bg, reset
        );
    }


    printf("└───────────┴──────────┴────────────┴────────────┘\n");
}

// Versione alternativa più compatta
void printRegisterTableCompact() {
    printf("RV32I Register Values:\n");
    printf("----------------------\n");

    for (int i = 0; i < 32; i++) {
        printf("x%-2d (%-4s): 0x%08X (%d)\n",
               i, register_names[i], registers[i], registers[i]);
    }
    printf("\n");
}

// Versione che mostra solo i registri non zero
void printNonZeroRegisters() {
    printf("Non-zero RV32I Registers:\n");
    printf("-------------------------\n");

    int found = 0;
    for (int i = 0; i < 32; i++) {
        if (registers[i] != 0) {
            printf("x%-2d (%-4s): 0x%08X (%d)\n",
                   i, register_names[i], registers[i], registers[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("Tutti i registri sono a zero.\n");
    }
    printf("\n");
}
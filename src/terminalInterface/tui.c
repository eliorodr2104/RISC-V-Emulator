//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include <stdint.h>
#include <stdio.h>

#include "instructionMemory.h"
#include "registerMemory.h"
#include "tools.h"

const char* register_names[32] = {
    "zero", "ra",  "sp",  "gp",  "tp",  "t0",  "t1",  "t2",
    "s0",   "s1",  "a0",  "a1",  "a2",  "a3",  "a4",  "a5",
    "a6",   "a7",  "s2",  "s3",  "s4",  "s5",  "s6",  "s7",
    "s8",   "s9",  "s10", "s11", "t3",  "t4",  "t5",  "t6"
};

// ########################
// # Main print functions #
// ########################

void headerProgram() {
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║          RISC-V CPU SIMULATOR          ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("\n");
}

void userChoices() {
    printf("Select execution mode:\n");
    printf("┌────────────────────────────────────────┐\n");
    printf("│  [1] Full execution         (Full)     │\n");
    printf("│  [2] Step by step execution (Step)     │\n");
    printf("│  [3] Exit                              │\n");
    printf("└────────────────────────────────────────┘\n");
    printf("\nSelection (1 - 3): ");
}


// ########################
// # CPU print functions  #
// ########################

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

void printFinalRegisterTable() {
    printf("\n");
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║              Final State Registers             ║\n");
    printf("╚════════════════════════════════════════════════╝\n");

    printRegisterTable();

    // Statistiche finali
    int nonZeroCount = 0;
    for (int i = 1; i < 32; i++) { // Escludi x0
        if (registers[i] != 0) nonZeroCount++;
    }

    printf("\nStats: %d/%d registers modified (%.1f%%)\n",
           nonZeroCount, 31, (nonZeroCount * 100.0) / 31);
}

void commandList() {
    printf("\nComandi:\n\tEnter = next step\n\tq     = esc\n\tr     = show registers\n");
    printf("════════════════════════════════════════\n\n");
}

void printHeaderInstructionStatus(const int32_t pc) {
    printf("┌──────────────────────────────────────┐\n");
    printf("│        Instruction 0x%08X        │\n", pc);
    printf("├──────────────────────────────────────┤\n");
}

void printInstructionStatus(const DecodedInstruction decoded, const int32_t input1, const int32_t input2, const int32_t output, int32_t pc) {
    printHeaderInstructionStatus(pc);

    char instruction[100];
    formatInstruction(decoded, instruction, sizeof(instruction));

    const char* instrLines[10];
    int maxLines = 0;

    char instrBuffer[10][60];

    snprintf(instrBuffer[0], sizeof(instrBuffer[0]), "%s", instruction);
    snprintf(instrBuffer[1], sizeof(instrBuffer[1]), " ");
    snprintf(instrBuffer[2], sizeof(instrBuffer[2]), "Input A:  0x%08X (%d)", input1, input1);
    snprintf(instrBuffer[3], sizeof(instrBuffer[3]), "Input B:  0x%08X (%d)", input2, input2);
    snprintf(instrBuffer[4], sizeof(instrBuffer[4]), "Output:   0x%08X (%d)", output, output);
    snprintf(instrBuffer[5], sizeof(instrBuffer[5]), " ");
    snprintf(instrBuffer[6], sizeof(instrBuffer[6]), "Opcode:   0x%02X", decoded.opcode);
    snprintf(instrBuffer[7], sizeof(instrBuffer[7]), "Funct3:   0x%X", decoded.funct3);

    for (int i = 0; i < 8; i++) {
        instrLines[i] = instrBuffer[i];
    }

    for (int i = 0; i < 8; i++) {
        printf("│ %-36s │\n", instrLines[i]);
    }

    printf("└──────────────────────────────────────┘\n");
}

void printProgramWithCurrentInstruction(
    const int32_t input1,
    const int32_t input2,
    const int32_t result,
    const int32_t pc
) {

    printf("\n══════════════════════════════════════════\n");
    printf("         PROGRAM EXECUTION STATUS\n"           );
    printf("══════════════════════════════════════════\n\n");

    // Stampa tutto il programma
    for (int i = 0; i < MAX_INSTRUCTIONS; i++) {
        const int address                       = i * 4;
        const DecodedInstruction currentDecoded = decodeInstruction(instructions[i]);

        char instrStr[100];
        formatInstruction(currentDecoded, instrStr, sizeof(instrStr));

        // Indica l'istruzione corrente con una freccia
        if (address == pc) {
            printf(" -> \033[1;33m0x%08X: %-30s\033[0m", address, instrStr);
            switch (currentDecoded.opcode) {
                case 0x33: // R-type (add, sub, etc.)
                    if (currentDecoded.funct3 == 0x0 && currentDecoded.funct7Bit30 == 0x00) { // ADD
                        printf("// 0x%08X + 0x%08X = 0x%08X", input1, input2, result);

                    } else if (currentDecoded.funct3 == 0x0 && currentDecoded.funct7Bit30 == 0x20) { // SUB
                        printf("// 0x%08X - 0x%08X = 0x%08X", input1, input2, result);

                    } else if (currentDecoded.funct3 == 0x7) { // AND
                        printf("// 0x%08X & 0x%08X = 0x%08X", input1, input2, result);

                    } else if (currentDecoded.funct3 == 0x6) { // OR
                        printf("// 0x%08X | 0x%08X = 0x%08X", input1, input2, result);

                    } else if (currentDecoded.funct3 == 0x4) { // XOR
                        printf("// 0x%08X ^ 0x%08X = 0x%08X", input1, input2, result);

                    } else if (currentDecoded.funct3 == 0x1) { // SLL
                        printf("// 0x%08X << %d = 0x%08X", input1, input2 & 0x1F, result);

                    } else if (currentDecoded.funct3 == 0x5 && currentDecoded.funct7Bit30 == 0x00) { // SRL
                        printf("// 0x%08X >> %d = 0x%08X", input1, input2 & 0x1F, result);

                    } else if (currentDecoded.funct3 == 0x5 && currentDecoded.funct7Bit30 == 0x20) { // SRA
                        printf("// 0x%08X >>> %d = 0x%08X", input1, input2 & 0x1F, result);

                    }
                    break;

                case 0x13: // I-type (addi, etc.)
                    if (currentDecoded.funct3 == 0x0) { // ADDI
                        printf("// 0x%08X + %d = 0x%08X", input1, (int16_t)currentDecoded.immediate, result);
                    } else if (currentDecoded.funct3 == 0x7) { // ANDI
                        printf("// 0x%08X & 0x%08X = 0x%08X", input1, currentDecoded.immediate, result);
                    } else if (currentDecoded.funct3 == 0x6) { // ORI
                        printf("// 0x%08X | 0x%08X = 0x%08X", input1, currentDecoded.immediate, result);
                    } else if (currentDecoded.funct3 == 0x4) { // XORI
                        printf("// 0x%08X ^ 0x%08X = 0x%08X", input1, currentDecoded.immediate, result);
                    }
                    break;

                case 0x03: // Load instructions
                    printf("// MEM[0x%08X] = 0x%08X", input1 + (int16_t)currentDecoded.immediate, result);
                    break;

                case 0x23: // Store instructions
                    printf("// MEM[0x%08X] = 0x%08X", input1 + (int16_t)currentDecoded.immediate, input2);
                    break;

                case 0x63: // Branch instructions
                    if (result) {
                        printf("// 0x%08X vs 0x%08X -> BRANCH TAKEN", input1, input2);
                    } else {
                        printf("// 0x%08X vs 0x%08X -> BRANCH NOT TAKEN", input1, input2);
                    }
                    break;

                default:
                    printf("// Result: 0x%08X", result);
                    break;
            }
            printf("\n");
        } else {
            // Istruzioni non correnti senza freccia
            printf("    0x%08X: %-30s\n", address, instrStr);
        }
    }

    printf("\n");

    // Mostra lo stato dei registri più importanti
    printf("┌─────────────────── REGISTER STATE ───────────────────┐\n");
    printf("│ ra = 0x%08X  sp = 0x%08X  gp = 0x%08X    │\n",
           registers[1], registers[2], registers[3]);
    printf("│ tp = 0x%08X  pc = 0x%08X                     │\n",
                   registers[4], pc);
    printf("└──────────────────────────────────────────────────────┘\n");

}

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

// Funzione per mostrare solo i registri modificati
void printNonZeroRegisters() {
    printf("\n  Registri modificati:\n");
    printf("─────────────────────────\n");

    int found = 0;
    for (int i = 1; i < 32; i++) { // Salta x0
        if (registers[i] != 0) {
            printf(" x%-2d (%-4s): 0x%08X (%d)\n",
                   i, register_names[i], registers[i], registers[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("🔍 Nessun registro modificato (tutti a zero).\n");
    }
    printf("\n");
}
//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include <stdint.h>
#include <stdio.h>

#include "instructionMemory.h"
#include "registerMemory.h"
#include "tools.h"
#include  "ncurses.h"

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

void printRegisterTable(
    WINDOW* winRegs
) {

    // Refresh registers windows
    werase(winRegs);
    box(winRegs, 0, 0);


    wattron(winRegs, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(winRegs, 0, 2, " REGISTER STATE ");
    wattroff(winRegs, COLOR_PAIR(1) | A_BOLD);

    int row = 2;

    constexpr int column = 3;

    wattron  (winRegs, COLOR_WHITE | A_NORMAL);
    mvwprintw(winRegs, row, column, "╭ Registers │ Value ╮");
    wattroff (winRegs, COLOR_WHITE | A_NORMAL);

    row += 2;

    for (int i = 0; i < 32; i++) {

        if (row >= getmaxy(winRegs) - 1) break; // break if the window is full

        // Change color to green if the register is not zero
        if (registers[i] != 0) {
            wattron(winRegs, COLOR_PAIR(2) | A_BOLD);
        }

        mvwprintw(winRegs, row, column, "%-6s     %08X",
                  register_names[i], registers[i]);
        wattroff(winRegs, COLOR_PAIR(2) | A_BOLD);


        row++;
    }

    // Refresh registers windows
    wnoutrefresh(winRegs);

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
    WINDOW* winProg,
    WINDOW* winRegs,
    const int32_t input1,
    const int32_t input2,
    const int32_t result,
    const int32_t pc
) {

    // Header definition
    werase(winProg);
    box(winProg, 0, 0);
    wattron(winProg, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(winProg, 0, 2, " PROGRAM EXECUTION STATUS ");
    wattroff(winProg, COLOR_PAIR(1) | A_BOLD);

    // Print all instructions
    for (int i = 0; i < MAX_INSTRUCTIONS; i++) {
        const int address                       = i * 4;
        const DecodedInstruction currentDecoded = decodeInstruction(instructions[i]);

        char instrStr[100];
        formatInstruction(currentDecoded, instrStr, sizeof(instrStr));

        // Calc index, with overflow stop prints instructions
        const int row = 1 + i;
        if (row >= getmaxy(winProg) - 1) break;


        if (address == pc) {

            // Current instruction with arrow and color background
            wattron(winProg, COLOR_PAIR(3) | A_BOLD);
            mvwprintw(winProg, row, 2, "-> 0x%08X: %-30s", address, instrStr);
            wattroff(winProg, COLOR_PAIR(3) | A_BOLD);

            // Print comment
            wattron(winProg, COLOR_PAIR(4));
            int colComment = 2 + 4 + 10 + 2 + 30;

            switch (currentDecoded.opcode) {

                // Instruction R-type
                case 0x33:

                    switch (currentDecoded.funct3) {

                        // Instruction ADD or SUB
                        case 0x0:

                            // Instruction ADD
                            if (!currentDecoded.funct7Bit30) {
                                mvwprintw(
                                    winProg,
                                    row,
                                    colComment,
                                    "// add: 0x%08X + 0x%08X = 0x%08X",
                                    input1,
                                    input2,
                                    result
                                );

                            // Instruction SUB
                            } else {
                                mvwprintw(
                                    winProg,
                                    row,
                                    colComment,
                                    "// sub: 0x%08X - 0x%08X = 0x%08X",
                                    input1,
                                    input2,
                                    result
                                );

                            }

                        break;

                        // Instruction SLL "Shift Left Logical"
                        case 0x1:
                            mvwprintw(winProg, row, colComment,
                                      "// sll: 0x%08X << %d = 0x%08X",
                                      input1, input2 & 0x1F, result);

                        break;

                        // Instruction SLT "Set Less Than"
                        case 0x2:
                            mvwprintw(winProg, row, colComment,
                                      "// slt: 0x%08X < 0x%08X = 0x%08X",
                                      input1, input2, result);

                        break;

                        // Instruction XOR
                        case 0x4:
                            mvwprintw(winProg, row, colComment,
                                      "// xor: 0x%08X ^ 0x%08X = 0x%08X",
                                      input1, input2, result);

                        break;

                        // Instruction SRL or SRA
                        case 0x5:

                            // Instruction SRA
                            if (currentDecoded.funct7Bit30) {
                                mvwprintw(winProg, row, colComment,
                                          "// sra: 0x%08X >>> %d = 0x%08X",
                                          input1, input2 & 0x1F, result);

                            // Instruction SRL
                            } else {
                                mvwprintw(winProg, row, colComment,
                                          "// srl: 0x%08X >> %d = 0x%08X",
                                          input1, input2 & 0x1F, result);

                            }

                        break;

                        // Instruction OR
                        case 0x6:
                            mvwprintw(winProg, row, colComment,
                                      "// or:  0x%08X | 0x%08X = 0x%08X",
                                      input1, input2, result);

                        break;

                        // Instruction AND
                        case 0x7:
                            mvwprintw(winProg, row, colComment,
                                      "// and: 0x%08X & 0x%08X = 0x%08X",
                                      input1, input2, result);

                        break;

                        default: mvwprintw(winProg, row, colComment, "// op non riconosciuta"); break;
                    }

                    break;

                case 0x67:
                    mvwprintw(
                        winProg,
                        row,
                        colComment,
                        "// jalr: ra -> 0x%08X, (rs1=0x%08X + %d)",
                        pc + 4,
                        input1,
                        (int16_t)currentDecoded.immediate
                    );

                break;

                // Instructions I-Type
                case 0x13:

                    switch (currentDecoded.funct3) {

                        // Instruction ADDI
                        case 0x0:
                            mvwprintw(winProg, row, colComment,
                                      "// addi: 0x%08X + %d = 0x%08X",
                                      input1,
                                      (int16_t)currentDecoded.immediate,
                                      result);

                        break;

                        // Instruction SLLI
                        case 0x1:
                            mvwprintw(winProg, row, colComment,
                                      "// slli: 0x%08X << %d = 0x%08X",
                                      input1,
                                      currentDecoded.immediate,
                                      result);

                        break;

                        // Instruction XORI
                        case 0x4:
                            mvwprintw(winProg, row, colComment,
                                      "// xori: 0x%08X ^ 0x%08X = 0x%08X",
                                      input1,
                                      currentDecoded.immediate,
                                      result);

                        break;

                        // Instruction SRLI or SRAI
                        case 0x5:

                            if (currentDecoded.funct7Bit30) {
                                mvwprintw(winProg, row, colComment,
                                          "// srai: 0x%08X >>> %d = 0x%08X",
                                          input1,
                                          currentDecoded.immediate,
                                          result);

                            } else {
                                mvwprintw(winProg, row, colComment,
                                          "// srli: 0x%08X >> %d = 0x%08X",
                                          input1,
                                          currentDecoded.immediate,
                                          result);

                            }

                        break;

                        // Instruction ORI
                        case 0x6:
                            mvwprintw(winProg, row, colComment,
                                      "// ori:  0x%08X | 0x%08X = 0x%08X",
                                      input1,
                                      currentDecoded.immediate,
                                      result);

                        break;

                        // Instruction ANDI
                        case 0x7:
                            mvwprintw(winProg, row, colComment,
                                      "// andi: 0x%08X & 0x%08X = 0x%08X",
                                      input1,
                                      currentDecoded.immediate,
                                      result);

                        break;

                        default: mvwprintw(winProg, row, colComment, "// op non riconosciuta"); break;
                    }

                    break;

                default: mvwprintw(winProg, row, colComment, "// op non riconosciuta"); break;

            }

            wattroff(winProg, COLOR_PAIR(4));

        } else {
            // All instructions
            wattron(winProg, COLOR_PAIR(0));
            mvwprintw(winProg, row, 2, "   0x%08X: %-30s", address, instrStr);
            wattroff(winProg, COLOR_PAIR(0));
        }
    }

    printRegisterTable(winRegs);

    // Refresh instructions windows
    wnoutrefresh(winProg);

    // Update all windows
    doupdate();

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

void printRegisterTableCompact() {
    printf("RV32I Register Values:\n");
    printf("----------------------\n");

    for (int i = 0; i < 32; i++) {
        printf("x%-2d (%-4s): 0x%08X (%d)\n",
               i, register_names[i], registers[i], registers[i]);
    }
    printf("\n");
}

void printNonZeroRegisters() {
    printf("\n  Registri modificati:\n");
    printf("─────────────────────────\n");

    int found = 0;
    for (int i = 1; i < 32; i++) { // Jump x0
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
//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include "tuiCpu.h"

#include <stdlib.h>

#include "aluControl.h"
#include "instructionMemory.h"
#include "registerMemory.h"
#include "tools.h"
#include  "ncurses.h"
#include "tuiNcurses.h"

const char* register_names[32] = {
    "zero", "ra",  "sp",  "gp",  "tp",  "t0",  "t1",  "t2",
    "s0",   "s1",  "a0",  "a1",  "a2",  "a3",  "a4",  "a5",
    "a6",   "a7",  "s2",  "s3",  "s4",  "s5",  "s6",  "s7",
    "s8",   "s9",  "s10", "s11", "t3",  "t4",  "t5",  "t6"
};

// ########################
// # CPU print functions  #
// ########################

void printRegisterTable(
    WINDOW* winRegs
) {

    // Refresh registers windows
    werase(winRegs);
    wbkgd(winRegs, COLOR_PAIR(0));
    box(winRegs, 0, 0);

    wattron(winRegs,  COLOR_PAIR(2) | A_BOLD);
    mvwprintw(winRegs, 0, 2, " T");
    wattroff(winRegs, COLOR_PAIR(2) | A_BOLD);

    wattron(winRegs, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(winRegs, 0, 4, "able Registers ");
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

}


void drawPipeline(
          WINDOW* winStatus,
    const DecodedInstruction currentDecoded,
    const int32_t pc,
    const int step

) {

    werase(winStatus);
    wbkgd(winStatus, COLOR_PAIR(0));
    box(winStatus, 0, 0);

    wattron(winStatus,  COLOR_PAIR(2) | A_BOLD);
    mvwprintw(winStatus, 0, 2, " R");
    wattroff(winStatus, COLOR_PAIR(2) | A_BOLD);

    wattron(winStatus, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(winStatus, 0, 4, "ISC-V State ");
    wattroff(winStatus, COLOR_PAIR(1) | A_BOLD);

    if (step == 0) {
        wattron(winStatus, COLOR_PAIR(4));
        mvwprintw(winStatus, 4, 8, "0x%08X", pc);
        wattroff(winStatus, COLOR_PAIR(4));

    } else {
        mvwprintw(winStatus, 4, 8, "0x%08X", pc);

    }

    mvwprintw(winStatus, 5,  4, "PC");
    mvwprintw(winStatus, 5, 7,  "------------>");

    mvwaddch(winStatus, 2, 20, ACS_ULCORNER);

    for(int i = 0; i < 14; ++i)
        mvwaddch(winStatus, 2, 21 + i, ACS_HLINE);
    mvwaddch(winStatus, 2, 35, ACS_URCORNER);

    mvwprintw(winStatus, 3, 22, "Instruction");
    mvwprintw(winStatus, 4, 22, "Mem");

    mvwaddch(winStatus, 8, 20, ACS_LLCORNER);
    for(int i = 0; i < 14; ++i)
        mvwaddch(winStatus, 8, 21 + i, ACS_HLINE);
    mvwaddch(winStatus, 8, 35, ACS_LRCORNER);

    for(int y = 3; y <= 7; ++y) {
        mvwaddch(winStatus, y, 20, ACS_VLINE);
        mvwaddch(winStatus, y, 35, ACS_VLINE);
    }

    if (step == 1) {
        wattron(winStatus, COLOR_PAIR(4));
        mvwprintw(winStatus, 2, 38, "0x%08X", currentDecoded.rs1);
        wattroff(winStatus, COLOR_PAIR(4));

    }

    mvwprintw(winStatus, 3, 37,  "------------>");
    mvwprintw(winStatus, 4, 41,  "rs1");

    if (step == 1) {
        wattron(winStatus, COLOR_PAIR(4));
        mvwprintw(winStatus, 6, 38, "0x%08X", currentDecoded.rs2);
        wattroff(winStatus, COLOR_PAIR(4));

    }

    mvwprintw(winStatus, 7, 37,  "------------>");

    mvwprintw(winStatus, 9, 2, "Explain:");

    switch (step) {
        case 0: mvwprintwWrap(winStatus, 10, 5, "PC sends address to instruction memory"); break;
        case 1: mvwprintwWrap(winStatus, 10, 5, "Memory instruction provides the addresses of the registers"); break;
        case 2: mvwprintw(winStatus, 11, 2, "3. REGISTRI leggono i valori sorgente"); break;
        case 3: mvwprintw(winStatus, 11, 2, "4. ALU esegue l'operazione ADD sui valori"); break;
        case 4: mvwprintw(winStatus, 11, 2, "5. Il risultato viene scritto nei REGISTRI (write back)"); break;
        case 5: mvwprintw(winStatus, 11, 2, "6. Il PC viene aggiornato a PC+4"); break;
        default: break;
    }
}

bool printProgramWithCurrentInstruction(
    WINDOW* winProg,
    WINDOW* winRegs,
    WINDOW* winStatus,
    const int32_t input1,
    const int32_t input2,
    const int32_t result,
    const int32_t pc
) {
    int step = 0;
    DecodedInstruction usageInstruction = {};

    // Header definition
    werase(winProg);
    wbkgd(winProg, COLOR_PAIR(0)); // background black, text white default
    box(winProg, 0, 0);

    wattron(winProg,  COLOR_PAIR(2) | A_BOLD);
    mvwprintw(winProg, 0, 2, " E");
    wattroff(winProg, COLOR_PAIR(2) | A_BOLD);

    wattron(winProg, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(winProg, 0, 4, "xecution status ");
    wattroff(winProg, COLOR_PAIR(1) | A_BOLD);

    // Print all instructions
    for (int i = 0; i < MAX_INSTRUCTIONS; i++) {
        const int address                       = i * 4;
        const DecodedInstruction currentDecoded = decodeInstruction(instructions[i]);

        char instrStr[100];
        formatInstruction(currentDecoded, instrStr, sizeof(instrStr));

        // Calc index, with overflow stop prints instructions
        const int row = 2 + i;
        if (row >= getmaxy(winProg) - 1) break;


        if (address == pc) {
            usageInstruction = currentDecoded;

            // Current instruction with arrow and color background
            wattron(winProg, COLOR_PAIR(4) | A_BOLD);
            mvwprintw(winProg, row, 2, "-> 0x%08X: %-30s", address, instrStr);
            wattroff(winProg, COLOR_PAIR(4) | A_BOLD);

            // Print comment
            wattron(winProg, COLOR_PAIR(5));
            const int colComment = 2 + 4 + 10 + 2 + 30;

            const AluOp aluOp = getInstructionEnum(currentDecoded.opcode, currentDecoded.funct3, currentDecoded.funct7Bit30);
            switch (currentDecoded.opcode) {

                // Instruction R-type
                case 0x33:

                    switch (aluOp) {

                        // Instruction ADD
                        case ALU_ADD:
                            mvwprintw(
                                    winProg,
                                    row,
                                    colComment,
                                    "// add: 0x%08X + 0x%08X = 0x%08X",
                                    input1,
                                    input2,
                                    result
                                );

                        break;

                        // Instruction SUB
                        case ALU_SUB:
                            mvwprintw(
                                    winProg,
                                    row,
                                    colComment,
                                    "// sub: 0x%08X - 0x%08X = 0x%08X",
                                    input1,
                                    input2,
                                    result
                                );

                        break;

                        // Instruction SLL "Shift Left Logical"
                        case ALU_SLL:
                            mvwprintw(winProg, row, colComment,
                                      "// sll: 0x%08X << %d = 0x%08X",
                                      input1, input2 & 0x1F, result);

                        break;

                        // Instruction SLT "Set Less Than"
                        case ALU_SLT:
                            mvwprintw(winProg, row, colComment,
                                      "// slt: 0x%08X < 0x%08X = 0x%08X",
                                      input1, input2, result);

                        break;

                        // Instruction XOR
                        case ALU_XOR:
                            mvwprintw(winProg, row, colComment,
                                      "// xor: 0x%08X ^ 0x%08X = 0x%08X",
                                      input1, input2, result);

                        break;

                        // // Instruction SRA
                        case ALU_SRA:
                            mvwprintw(winProg, row, colComment,
                                          "// sra: 0x%08X >>> %d = 0x%08X",
                                          input1, input2 & 0x1F, result);

                        break;

                        // Instruction SRL
                        case ALU_SRL:
                            mvwprintw(winProg, row, colComment,
                                          "// srl: 0x%08X >> %d = 0x%08X",
                                          input1, input2 & 0x1F, result);

                        break;

                        // Instruction OR
                        case ALU_OR:
                            mvwprintw(winProg, row, colComment,
                                      "// or:  0x%08X | 0x%08X = 0x%08X",
                                      input1, input2, result);

                        break;

                        // Instruction AND
                        case ALU_AND:
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

                    switch (aluOp) {

                        // Instruction ADDI
                        case ALU_ADDI:
                            mvwprintw(winProg, row, colComment,
                                      "// addi: 0x%08X + %d = 0x%08X",
                                      input1,
                                      (int16_t)currentDecoded.immediate,
                                      result);

                        break;

                        // Instruction SLLI
                        case ALU_SLLI:
                            mvwprintw(winProg, row, colComment,
                                      "// slli: 0x%08X << %d = 0x%08X",
                                      input1,
                                      currentDecoded.immediate,
                                      result);

                        break;

                        // Instruction XORI
                        case ALU_XORI:
                            mvwprintw(winProg, row, colComment,
                                      "// xori: 0x%08X ^ 0x%08X = 0x%08X",
                                      input1,
                                      currentDecoded.immediate,
                                      result);

                        break;

                        // Instruction SRAI
                        case ALU_SRAI:

                            mvwprintw(winProg, row, colComment,
                                          "// srai: 0x%08X >>> %d = 0x%08X",
                                          input1,
                                          currentDecoded.immediate,
                                          result);

                        break;

                        // Instruction SRLI
                        case ALU_SRLI:
                            mvwprintw(winProg, row, colComment,
                                          "// srli: 0x%08X >> %d = 0x%08X",
                                          input1,
                                          currentDecoded.immediate,
                                          result);

                        break;

                        // Instruction ORI
                        case ALU_ORI:
                            mvwprintw(winProg, row, colComment,
                                      "// ori:  0x%08X | 0x%08X = 0x%08X",
                                      input1,
                                      currentDecoded.immediate,
                                      result);

                        break;

                        // Instruction ANDI
                        case ALU_ANDI:
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

            wattroff(winProg, COLOR_PAIR(5));

        } else {
            // All instructions
            wattron(winProg, COLOR_PAIR(0));
            mvwprintw(winProg, row, 2, "   0x%08X: %-30s", address, instrStr);
            wattroff(winProg, COLOR_PAIR(0));
        }
    }

    // Refresh instructions windows
    printRegisterTable(winRegs);
    wnoutrefresh(winProg);
    wnoutrefresh(winRegs);

    nodelay(winStatus, FALSE);

    bool quitRequested = false;
    while (1) {
        drawPipeline(winStatus, usageInstruction, pc, step);

        wnoutrefresh(winStatus);
        doupdate();

        const int ch = wgetch(winStatus);
        if (ch == 'q' || ch == 'Q') {
            quitRequested = true;
            break;

        }

        if (ch == 10 || ch == 13) break;


        step++;

        if (step > 5) break;
    }

    return quitRequested;
}

//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include "tuiCpu.h"

#include <stdlib.h>
#include <string.h>

#include "aluControl.h"
#include "instructionMemory.h"
#include "registerMemory.h"
#include "tools.h"
#include  "ncurses.h"
#include "tuiMain.h"
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
    WINDOW*   winRegs,
    const int currentSetting,
    const int offset

) {

    int row = 2;
    constexpr int column = 3;

    const int maxRows = getmaxy(winRegs) - row - 3;

    // Refresh registers windows
    werase(winRegs);
    wbkgd(winRegs, COLOR_PAIR(0));
    box(winRegs, 0, 0);

    wattron  (winRegs,  COLOR_PAIR(2) | A_BOLD);
    mvwprintw(winRegs, 0, 2, " T");
    wattroff (winRegs, COLOR_PAIR(2) | A_BOLD);

    wattron  (winRegs, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(winRegs, 0, 4, "able Registers ");
    wattroff (winRegs, COLOR_PAIR(1) | A_BOLD);

    mvwprintw(winRegs, row, column, "╭");
    mvwaddch (winRegs, row, column + 1, ACS_HLINE);

    wattron  (winRegs, COLOR_WHITE | A_NORMAL);
    mvwprintw(winRegs, row, column + 2, " Registers        Value ");
    wattroff (winRegs, COLOR_WHITE | A_NORMAL);

    mvwprintw(winRegs, row, column + 27, "╮");
    mvwaddch (winRegs, row, column + 26, ACS_HLINE);

    row ++;

    for (int i = offset; i < 32 && i - offset < maxRows; i++) {
        mvwaddch(winRegs, row, column, ACS_VLINE);

        // Change color to green if the register is not zero
        if (registers[i] != 0) {
            wattron(winRegs, COLOR_PAIR(2) | A_BOLD);
        }

        if (currentSetting == 'd') {
            mvwprintw(winRegs, row, column + 3, "%-6s        %d",
                  register_names[i], registers[i]);

        }

        if (currentSetting == 'h') {
            mvwprintw(winRegs, row, column + 3, "%-6s      0x%08X",
                  register_names[i], registers[i]);
        }

        wattroff(winRegs, COLOR_PAIR(2) | A_BOLD);
        mvwaddch(winRegs, row, column + 27, ACS_VLINE);

        row++;
    }

    mvwprintw(winRegs, row, column, "╰");

    for (uint8_t i = 0; i < 26; i++) {

        mvwaddch (winRegs, row, column + 1 + i, ACS_HLINE);

    }

    mvwprintw(winRegs, row, column + 27, "╯");

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
          WINDOW*  winProg,
          WINDOW*  winRegs,
          WINDOW*  winStatus,
          WINDOW*  winCmd,
          Windows* selectCurrent,
          int*     charCurrent,
    const int32_t  input1,
    const int32_t  input2,
    const int32_t  result,
    const int32_t  pc,
    const options_t options
) {
    int step       = 0;
    int offset     = 0; // Offset for registers
    DecodedInstruction usageInstruction = {};

    // Array to save string asm RV32I
    char **asmLines = nullptr;
    int lineCount = 0;

    // Header definition
    werase(winProg);
    wbkgd(winProg, COLOR_PAIR(0));
    box(winProg, 0, 0);

    if (*selectCurrent == PROG_WINDOW) {

        wattron(winProg,  COLOR_PAIR(2) | A_BOLD);
        mvwprintw(winProg, 0, 2, " E");
        wattroff(winProg, COLOR_PAIR(2) | A_BOLD);

        wattron(winProg, COLOR_PAIR(1) | A_BOLD);
        mvwprintw(winProg, 0, 4, "xecution status ");
        wattroff(winProg, COLOR_PAIR(1) | A_BOLD);

    } else {

        wattron(winProg,  COLOR_PAIR(2) | A_DIM);
        mvwprintw(winProg, 0, 2, " E");
        wattroff(winProg, COLOR_PAIR(2) | A_DIM);

        wattron(winProg, COLOR_PAIR(1) | A_DIM);
        mvwprintw(winProg, 0, 4, "xecution status ");
        wattroff(winProg, COLOR_PAIR(1) | A_DIM);

    }

    // Read the file header
    FILE *file = fopen(options.binary_file, "r");
    if (file == nullptr) {
        mvwprintw(winProg, 2, 2, "Error: impossible open the file %s", options.binary_file);
        goto cleanup;

    }

    // Count file line
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        lineCount++;

    }

    rewind(file);

    // Alloc memory for the rows
    asmLines = malloc(lineCount * sizeof(char*));
    if (!asmLines) {
        mvwprintw(winProg, 2, 2, "Error: impossible alloc memory");
        fclose(file);
        goto cleanup;

    }

    // Read all lines
    int currentLine = 0;

    while (fgets(buffer, sizeof(buffer), file) && currentLine < lineCount) {
        const size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';

        }

        asmLines[currentLine] = malloc(strlen(buffer) + 1);
        strcpy(asmLines[currentLine], buffer);
        currentLine++;
    }

    fclose(file);

    // Calc instruction PC
    // The single instruction is a 4 byte long
    const int currentInstructionIndex = pc / 4;

    int codeLineCounter = 0;
    int highlightedLine = -1;

    for (int i = 0; i < lineCount; i++) {
        const char *trimmed = asmLines[i];

        // Skip space char
        while (*trimmed == ' ' || *trimmed == '\t') trimmed++;

        // Skip line empty, comments and directives
        if (*trimmed == '\0' || *trimmed == '#' || *trimmed == '.' || *trimmed == '_') {
            continue;
        }

        // Skip tag (line end ':')
        const char *colon = strchr(trimmed, ':');
        if (colon && *(colon + 1) == '\0') {
            continue;
        }

        // This is an instruction
        if (codeLineCounter == currentInstructionIndex) {
            highlightedLine = i;

            if (currentInstructionIndex < options.instruction_count) {
                usageInstruction = decodeInstruction(options.instructions[currentInstructionIndex].instruction);
            }

            break;
        }

        codeLineCounter++;

    }

    const int maxRows = getmaxy(winProg) - 3;
    int startLine    = 0;

    if (highlightedLine >= maxRows) {
        startLine = highlightedLine - maxRows / 2;
        if (startLine < 0) startLine = 0;

    }

    for (int i = startLine; i < lineCount && i - startLine < maxRows; i++) {
        const int row = 2 + (i - startLine);

        if (i == highlightedLine) {

            // Current line PC
            wattron(winProg, COLOR_PAIR(4) | A_BOLD);
            mvwprintw(winProg, row, 2, "-> %s", asmLines[i]);
            wattroff(winProg, COLOR_PAIR(4) | A_BOLD);

            // Add comments for debug
            wattron(winProg, COLOR_PAIR(5));
            constexpr int colComment = 50; // Pos comment

            if (currentInstructionIndex < options.instruction_count) {
                const AluOp aluOp = getInstructionEnum(usageInstruction.opcode, usageInstruction.funct3, usageInstruction.funct7Bit30);

                switch (usageInstruction.opcode) {
                    case 0x33: // R-type
                        switch (aluOp) {
                            case ALU_ADD:
                                mvwprintw(winProg, row, colComment,
                                          "// add: 0x%08X + 0x%08X = 0x%08X",
                                          input1, input2, result);
                                break;
                            case ALU_SUB:
                                mvwprintw(winProg, row, colComment,
                                          "// sub: 0x%08X - 0x%08X = 0x%08X",
                                          input1, input2, result);
                                break;

                            default:
                                mvwprintw(winProg, row, colComment, "// PC: 0x%08X", pc);
                                break;
                        }
                        break;

                    case 0x13: // I-type
                        switch (aluOp) {
                            case ALU_ADDI:
                                mvwprintw(winProg, row, colComment,
                                          "// addi: 0x%08X + %d = 0x%08X",
                                          input1, (int16_t)usageInstruction.immediate, result);
                                break;

                            default:
                                mvwprintw(winProg, row, colComment, "// PC: 0x%08X", pc);
                                break;
                        }
                        break;

                    default:
                        mvwprintw(winProg, row, colComment, "// PC: 0x%08X", pc);
                        break;
                }
            }
            wattroff(winProg, COLOR_PAIR(5));

        } else {

            wattron(winProg, COLOR_PAIR(0));
            mvwprintw(winProg, row, 2, "   %s", asmLines[i]);
            wattroff(winProg, COLOR_PAIR(0));

        }
    }

cleanup:
    // Free memory
    if (asmLines) {
        for (int i = 0; i < lineCount; i++) {
            free(asmLines[i]);
        }

        free(asmLines);

    }

    printRegisterTable(winRegs, *charCurrent, offset);
    wnoutrefresh(winProg);
    wnoutrefresh(winRegs);

    nodelay(winStatus, FALSE);

    bool quitRequested     = false;
    bool continueExecution = false;

    keypad(winStatus, TRUE);

    while (1) {
        printRegisterTable(winRegs, *charCurrent, offset);
        wnoutrefresh(winRegs);

        drawPipeline(winStatus, usageInstruction, pc, step);
        commandWindow(winCmd, *selectCurrent);

        wnoutrefresh(winStatus);
        wnoutrefresh(winCmd);
        doupdate();

        const int ch = wgetch(winStatus);

        if (ch == 'q' || ch == 'Q') {
            quitRequested = true;
            break;
        }

        if (ch == 'e' || ch == 'E') {
            *selectCurrent = PROG_WINDOW;
        }

        if (ch == 't' || ch == 'T') {
            *selectCurrent = REGS_WINDOW;
        }

        if (ch == 'r' || ch == 'R') {
            *selectCurrent = STATUS_WINDOW;
        }

        switch (*selectCurrent) {
            case PROG_WINDOW:

                if (usageInstruction.opcode != 0x73) { // Not work if ecall
                    if (ch == 'j' || ch == 'J') continueExecution = true;
                }

                break;

            case STATUS_WINDOW:
                if (ch == 'n' || ch == 'N') { step++; }
                break;

            case REGS_WINDOW:
                if (ch == KEY_UP && offset > 0) {
                    offset--;
                }

                const int availableRows = getmaxy(winRegs) - 4;
                if (ch == KEY_DOWN && offset + availableRows < 32) {
                    offset++;
                }

                if (ch == 'd' || ch == 'D') {
                    *charCurrent = 'd';
                    printRegisterTable(winRegs, *charCurrent, offset);
                    wnoutrefresh(winRegs);
                }

                if (ch == 'h' || ch == 'H') {
                    *charCurrent = 'h';
                    printRegisterTable(winRegs, *charCurrent, offset);
                    wnoutrefresh(winRegs);
                }
                break;

            default: break;
        }

        if (step > 5 || continueExecution) break;
    }

    return quitRequested;
}
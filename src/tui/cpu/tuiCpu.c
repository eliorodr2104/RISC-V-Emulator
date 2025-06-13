//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#define SPACE_COMMENT 30

#include "tuiCpu.h"

#include <stdlib.h>

#include "aluControl.h"
#include "assemblyData.h"
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
        case 2: mvwprintwWrap(winStatus, 11, 2, "3. REGISTRI leggono i valori sorgente"); break;
        case 3: mvwprintwWrap(winStatus, 11, 2, "4. ALU esegue l'operazione ADD sui valori"); break;
        case 4: mvwprintwWrap(winStatus, 11, 2, "5. Il risultato viene scritto nei REGISTRI (write back)"); break;
        case 5: mvwprintwWrap(winStatus, 11, 2, "6. Il PC viene aggiornato a PC+4"); break;
        default: break;
    }
}

bool printProgramWithCurrentInstruction(
    const WindowsManagement windowManagement,
          int*     charCurrent,
    const int32_t  input1,
    const int32_t  input2,
    const int32_t  result,
          Cpu*     cpu,
    const options_t options,
    const AssemblyData* data,
          int*     offsetProg
) {

    int step       = 0;
    int offset     = 0; // Offset for registers

    DecodedInstruction usageInstruction = {};

    int rowToLineMapping[data->lineCount];
    int visibleRows = 0;

    // Header definition
    werase(windowManagement.winProg->window);
    wbkgd(windowManagement.winProg->window, COLOR_PAIR(0));
    box(windowManagement.winProg->window, 0, 0);

    wattron(windowManagement.winProg->window,  COLOR_PAIR(2) | A_BOLD);
    mvwprintw(windowManagement.winProg->window, 0, 2, " E");
    wattroff(windowManagement.winProg->window, COLOR_PAIR(2) | A_BOLD);

    wattron(windowManagement.winProg->window, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(windowManagement.winProg->window, 0, 4, "xecution status ");
    wattroff(windowManagement.winProg->window, COLOR_PAIR(1) | A_BOLD);

    // Calc instruction PC
    const int currentInstructionIndex = cpu->pc / 4;
          int highlightedLine = -1;

    if (currentInstructionIndex < options.instruction_count) {

        highlightedLine = data->instructionToLineMap[currentInstructionIndex];
        usageInstruction = decodeInstruction(options.instructions[currentInstructionIndex].instruction);

    }

    const int maxRows = getmaxy(windowManagement.winProg->window) - 3;
    int startLine     = *offsetProg;

    if (highlightedLine >= 0) {
        if (highlightedLine < startLine) {

            *offsetProg = highlightedLine;
            startLine = *offsetProg;

        } else if (highlightedLine >= startLine + maxRows) {

            *offsetProg = highlightedLine - maxRows + 1;
            if (*offsetProg < 0) *offsetProg = 0;
            startLine = *offsetProg;

        }
    }

    int maxStartLine = data->lineCount - maxRows;
    if (maxStartLine < 0) maxStartLine = 0;
    if (startLine > maxStartLine) {
        startLine = maxStartLine;
        *offsetProg = startLine;
    }

    for (int i = startLine; i < data->lineCount && (i - startLine) < maxRows; i++) {
        const int row = 2 + (i - startLine);
        rowToLineMapping[visibleRows] = i;
        visibleRows++;

        if (i == highlightedLine) {

            // Current line PC
            wattron(windowManagement.winProg->window, COLOR_PAIR(4) | A_BOLD);
            mvwprintw(windowManagement.winProg->window, row, 2, "-> %s", data->asmLines[i]);
            wattroff(windowManagement.winProg->window, COLOR_PAIR(4) | A_BOLD);

            // Add comments for debug
            wattron(windowManagement.winProg->window, COLOR_PAIR(5));

            if (currentInstructionIndex < options.instruction_count) {
                const AluOp aluOp = getInstructionEnum(usageInstruction.opcode, usageInstruction.funct3, usageInstruction.funct7Bit30);

                // Add comment for ecall
                if (usageInstruction.opcode == 0x73) {
                    mvwprintw(windowManagement.winProg->window, row, SPACE_COMMENT, "// ecall - End program");

                } else {
                    switch (usageInstruction.opcode) {
                        case 0x33: // R-type
                            switch (aluOp) {
                                case ALU_ADD:
                                    mvwprintw(windowManagement.winProg->window, row, SPACE_COMMENT,
                                            "// add: 0x%08X + 0x%08X = 0x%08X",
                                            input1, input2, result);
                                    break;
                                case ALU_SUB:
                                    mvwprintw(windowManagement.winProg->window, row, SPACE_COMMENT,
                                            "// sub: 0x%08X - 0x%08X = 0x%08X",
                                            input1, input2, result);
                                    break;

                                default:
                                    mvwprintw(windowManagement.winProg->window, row, SPACE_COMMENT, "// PC: 0x%08X", cpu->pc);
                                    break;
                            }
                            break;

                        case 0x13: // I-type
                            switch (aluOp) {
                                case ALU_ADDI:
                                    mvwprintw(windowManagement.winProg->window, row, SPACE_COMMENT,
                                            "// addi: 0x%08X + %d = 0x%08X",
                                            input1, (int16_t)usageInstruction.immediate, result);
                                    break;

                                default:
                                    mvwprintw(windowManagement.winProg->window, row, SPACE_COMMENT, "// PC: 0x%08X", cpu->pc);
                                    break;
                            }
                            break;

                        default:
                            mvwprintw(windowManagement.winProg->window, row, SPACE_COMMENT, "// PC: 0x%08X", cpu->pc);
                            break;
                    }
                }
            }
            wattroff(windowManagement.winProg->window, COLOR_PAIR(5));

        } else {

            wattron(windowManagement.winProg->window, COLOR_PAIR(0));
            mvwprintw(windowManagement.winProg->window, row, 2, "   %s", data->asmLines[i]);
            wattroff(windowManagement.winProg->window, COLOR_PAIR(0));

        }
    }

    printRegisterTable(windowManagement.winRegs->window, *charCurrent, offset);
    if (windowManagement.winStatus->isActive) drawPipeline(windowManagement.winStatus->window, usageInstruction, cpu->pc, step);

    bool quitRequested     = false;
    bool continueExecution = false;

    bool redraw = true;

    // Input loop
    while (1) {

        if (redraw) {

            wnoutrefresh(windowManagement.winProg->window);
            wnoutrefresh(windowManagement.winRegs->window);

            if (windowManagement.winStatus->isActive) wnoutrefresh(windowManagement.winStatus->window);

            wnoutrefresh(windowManagement.winCmd->window);

            doupdate();

            redraw = false;

        }

        wtimeout(windowManagement.winProg->window, 50);

        const int ch = wgetch(windowManagement.winProg->window);

        if (ch == ERR) continue;

        if (ch == KEY_RESIZE) {

            if (!handleTerminalResize(&windowManagement)) {
                quitRequested = true;
                break;
            }

            redraw = true;
            redrawProgram(windowManagement, offsetProg, data, highlightedLine, maxRows, step, usageInstruction, charCurrent, offset, cpu);

        }

        if (ch == KEY_MOUSE) {
            MEVENT event;
            if (getmouse(&event) == OK) {

                // Control click is in intern win
                int prog_y, prog_x, prog_max_y, prog_max_x;
                getbegyx(windowManagement.winProg->window, prog_y, prog_x);
                getmaxyx(windowManagement.winProg->window, prog_max_y, prog_max_x);

                // If click in range win then set instruction
                if (event.x >= prog_x && event.x < prog_x + prog_max_x &&
                    event.y >= prog_y && event.y < prog_y + prog_max_y &&
                    (event.bstate & BUTTON1_CLICKED)) {

                    // Click win
                    if (event.bstate & BUTTON1_CLICKED) {

                        // Click the left bottom
                        const int clicked_row = (event.y - prog_y - 2); // -2 for header and border

                        if (clicked_row >= 0 && clicked_row < visibleRows) {

                            const int target_instruction = data->lineToInstructionMap[rowToLineMapping[clicked_row]];

                            if (target_instruction >= 0) {
                                cpu->resetFlag = target_instruction;

                                return false;
                            }
                        }
                    }
                }
            }
        }

        if (ch == 'q' || ch == 'Q') {
            quitRequested = true;
            break;
        }

        if (ch == 'e' || ch == 'E') {
            *windowManagement.currentWindow = PROG_WINDOW;
        }

        if (ch == 't' || ch == 'T') {
            *windowManagement.currentWindow = REGS_WINDOW;
        }

        if (ch == 'r' || ch == 'R') {
            *windowManagement.currentWindow = STATUS_WINDOW;
        }

        switch (*windowManagement.currentWindow) {
            case PROG_WINDOW:

                if (ch == KEY_UP && *offsetProg > 0) {
                    (*offsetProg)--;

                    redraw = true;
                    redrawProgram(windowManagement, offsetProg, data, highlightedLine, maxRows, step, usageInstruction, charCurrent, offset, cpu);
                }

                if (ch == KEY_DOWN) {
                    int maxOffset = data->lineCount - maxRows;
                    if (maxOffset < 0) maxOffset = 0;

                    if (*offsetProg < maxOffset) {
                        (*offsetProg)++;

                        redraw = true;
                        redrawProgram(windowManagement, offsetProg, data, highlightedLine, maxRows, step, usageInstruction, charCurrent, offset, cpu);
                    }
                }

                if (usageInstruction.opcode != 0x73) {
                    if (ch == 'j' || ch == 'J') {
                        continueExecution = true;

                    }
                }

                break;

            case STATUS_WINDOW:
                if (ch == 'n' || ch == 'N' && windowManagement.winStatus->isActive) {
                    step++;

                    redraw = true;
                    drawPipeline(windowManagement.winStatus->window, usageInstruction, cpu->pc, step);

                }
                break;

            case REGS_WINDOW:
                if (ch == KEY_UP && offset > 0) {
                    offset--;

                    redraw = true;
                    printRegisterTable(windowManagement.winRegs->window, *charCurrent, offset);

                }

                const int availableRows = getmaxy(windowManagement.winRegs->window) - 4;
                if (ch == KEY_DOWN && offset + availableRows < 32) {
                    offset++;

                    redraw = true;
                    printRegisterTable(windowManagement.winRegs->window, *charCurrent, offset);

                }

                if (ch == 'd' || ch == 'D') {
                    *charCurrent = 'd';

                    redraw = true;
                    printRegisterTable(windowManagement.winRegs->window, *charCurrent, offset);

                }

                if (ch == 'h' || ch == 'H') {
                    *charCurrent = 'h';

                    redraw = true;
                    printRegisterTable(windowManagement.winRegs->window, *charCurrent, offset);

                }
                break;

            default: break;
        }

        if (step > 5 || continueExecution) break;

    }

    return quitRequested;
}

void redrawProgram(
    const WindowsManagement windowManagement,
    int*              offsetProg,
    const AssemblyData* data,
    const int        highlightedLine,
    const int        maxRows,
    const int        step,
    const DecodedInstruction usageInstruction,
    const int*      charCurrent,
    const int       offset,
    const Cpu* cpu


) {
    printRegisterTable(windowManagement.winRegs->window, *charCurrent, offset);

    if (windowManagement.winStatus->isActive)  drawPipeline(windowManagement.winStatus->window, usageInstruction, cpu->pc, step);

    commandWindow(windowManagement.winCmd->window, *windowManagement.currentWindow);

    werase(windowManagement.winProg->window);
    wbkgd(windowManagement.winProg->window, COLOR_PAIR(0));
    box(windowManagement.winProg->window, 0, 0);

    wattron(windowManagement.winProg->window,  COLOR_PAIR(2) | A_BOLD);
    mvwprintw(windowManagement.winProg->window, 0, 2, " E");
    wattroff(windowManagement.winProg->window, COLOR_PAIR(2) | A_BOLD);

    wattron(windowManagement.winProg->window, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(windowManagement.winProg->window, 0, 4, "xecution status ");
    wattroff(windowManagement.winProg->window, COLOR_PAIR(1) | A_BOLD);

    int maxStartLine_redraw = data->lineCount - maxRows;
    if (maxStartLine_redraw < 0) maxStartLine_redraw = 0;
    if (*offsetProg > maxStartLine_redraw) *offsetProg = maxStartLine_redraw;

    for (int i = *offsetProg; i < data->lineCount && (i - *offsetProg) < maxRows; i++) {
        const int row = 2 + (i - *offsetProg);

        if (i == highlightedLine) {
            wattron(windowManagement.winProg->window, COLOR_PAIR(4) | A_BOLD);
            mvwprintw(windowManagement.winProg->window, row, 2, "-> %s", data->asmLines[i]);
            wattroff(windowManagement.winProg->window, COLOR_PAIR(4) | A_BOLD);

        } else {
            wattron(windowManagement.winProg->window, COLOR_PAIR(0));
            mvwprintw(windowManagement.winProg->window, row, 2, "   %s", data->asmLines[i]);
            wattroff(windowManagement.winProg->window, COLOR_PAIR(0));

        }
    }

    wnoutrefresh(windowManagement.winRegs->window);
    wnoutrefresh(windowManagement.winProg->window);

    if (windowManagement.winStatus->isActive) wnoutrefresh(windowManagement.winStatus->window);

    wnoutrefresh(windowManagement.winCmd->window);
}
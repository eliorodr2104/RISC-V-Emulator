//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#define SPACE_COMMENT 35

#include <stdlib.h>
#include <string.h>

#include "tui_cpu.h"
#include "alu_control.h"
#include "assembly_data.h"
#include "cpu.h"
#include "decode.h"
#include "fetch.h"
#include "tools.h"
#include "ncurses.h"
#include "tui_main.h"
#include "tui_ncurses.h"

const char* register_names[32] = {
    "zero", "ra",  "sp",  "gp",  "tp",  "t0",  "t1",  "t2",
    "s0",   "s1",  "a0",  "a1",  "a2",  "a3",  "a4",  "a5",
    "a6",   "a7",  "s2",  "s3",  "s4",  "s5",  "s6",  "s7",
    "s8",   "s9",  "s10", "s11", "t3",  "t4",  "t5",  "t6"
};

void printRegisterTable(
    Cpu       cpu,
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

    mvwaddch(winRegs, 0, 3, ACS_LRCORNER);

    wattron  (winRegs,  COLOR_PAIR(2) | A_BOLD);
    mvwprintw(winRegs, 0, 4, " R");
    wattroff (winRegs, COLOR_PAIR(2) | A_BOLD);

    wattron  (winRegs, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(winRegs, 0, 6, "egisters ");
    wattroff (winRegs, COLOR_PAIR(1) | A_BOLD);

    wattron  (winRegs,  COLOR_PAIR(2) | A_BOLD);
    mvwprintw(winRegs, 0, 16, " R");
    wattroff (winRegs, COLOR_PAIR(2) | A_BOLD);

    wattron  (winRegs, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(winRegs, 0, 18, "AM ");
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
        if (cpu->registers[i] != 0) {
            wattron(winRegs, COLOR_PAIR(2) | A_BOLD);
        }

        if (currentSetting == 'd') {
            mvwprintw(winRegs, row, column + 3, "%-6s        %d",
                  register_names[i], cpu->registers[i]);

        }

        if (currentSetting == 'h') {
            mvwprintw(winRegs, row, column + 3, "%-6s      0x%08X",
                  register_names[i], cpu->registers[i]);
        }

        wattroff(winRegs, COLOR_PAIR(2) | A_BOLD);
        mvwaddch(winRegs, row, column + 27, ACS_VLINE);

        row++;
    }

    mvwprintw(winRegs, row, column, "╰");

    for (uint8_t i = 0; i < 26; i++)
        mvwaddch(winRegs, row, column + 1 + i, ACS_HLINE);

    mvwprintw(winRegs, row, column + 27, "╯");

}


void drawPipeline(
          WINDOW* winStatus,
    const DecodedInstruction currentDecoded,
    const uint32_t pc,
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
          Cpu      cpu,
    const options_t options,
    const AssemblyData* data,
          int*     offsetProg,
          RAM      main_memory
) {

    if (!data) {
        fprintf(stderr, "ERROR: data is NULL\n");
        return true;
    }

    if (!data->asmLines) {
        fprintf(stderr, "ERROR: data->asmLines is NULL\n");
        return true;
    }

    if (!data->lineToInstructionMap) {
        fprintf(stderr, "ERROR: data->lineToInstructionMap is NULL\n");
        return true;
    }

    if (!data->instructionToLineMap) {
        fprintf(stderr, "ERROR: data->instructionToLineMap is NULL\n");
        return true;
    }

    if (data->lineCount <= 0) {
        fprintf(stderr, "ERROR: data->lineCount = %d\n", data->lineCount);
        return true;
    }

    int step       = 0;
    int offset     = 0;

    DecodedInstruction usageInstruction = { 0, 0, 0, 0, 0, 0, 0 };

    // SAFE INIT ARRAY
    int* rowToLineMapping = malloc(sizeof(int) * data->lineCount);
    if (!rowToLineMapping) {
        fprintf(stderr, "ERROR: Failed to allocate rowToLineMapping\n");
        return true;
    }

    // Init all -1
    for (int i = 0; i < data->lineCount; i++) {
        rowToLineMapping[i] = -1;
    }

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
    const uint32_t currentInstructionIndex = (cpu->pc - options.text_vaddr) / 4;
          int highlightedLine = -1;

    if (cpu->pc >= options.text_vaddr && cpu->pc < options.text_vaddr + options.text_size) {
        const auto raw_instruction = fetchInstruction(cpu, options, main_memory);

        if (currentInstructionIndex < options.text_size / 4) {
            highlightedLine = data->instructionToLineMap[currentInstructionIndex];
        }

        usageInstruction = decodeInstruction(raw_instruction);
    }

    const int maxRows          = getmaxy(windowManagement.winProg->window) - 3;
    const int max_rows_padding = maxRows / 2;
          int startLine        = *offsetProg;

    if (highlightedLine >= 0 && highlightedLine < startLine) {
        *offsetProg = highlightedLine;
        startLine = *offsetProg;

    } else if (highlightedLine >= startLine + max_rows_padding) {

        *offsetProg = highlightedLine - max_rows_padding + 1;
        if (*offsetProg < 0) *offsetProg = 0;
        startLine = *offsetProg;

    }

    int maxStartLine = data->lineCount - max_rows_padding;
    if (maxStartLine < 0) maxStartLine = 0;

    if (startLine > maxStartLine) {
        startLine = maxStartLine;
        *offsetProg = startLine;
    }

    int lineCount = data->lineCount;
    // Secure control for draw code
    for (int i = startLine; i < lineCount && i - startLine < maxRows; i++) {
        if (i < 0 || i >= data->lineCount) {
            fprintf(stderr, "ERROR: Invalid line index %d (lineCount=%d)\n", i, data->lineCount);
            continue;
        }

        if (!data->asmLines[i]) {
            fprintf(stderr, "ERROR: asmLines[%d] is NULL\n", i);
            continue;
        }

        const int row = 2 + (i - startLine);

        if (visibleRows >= data->lineCount) {
            fprintf(stderr, "ERROR: visibleRows=%d >= lineCount=%d\n", visibleRows, data->lineCount);
            break;
        }

        rowToLineMapping[visibleRows] = i;
        visibleRows++;

        draw_instruction_colored(
            windowManagement.winProg->window,
            data->asmLines[i],
            row,
            i == highlightedLine
        );

        if (i == highlightedLine) {
            // Add comments for debug
            wattron(windowManagement.winProg->window, COLOR_PAIR(5));

            if (currentInstructionIndex < options.text_size) {
                const AluEnumOperation aluOp = getInstructionEnum(usageInstruction.opcode, usageInstruction.funz3, usageInstruction.funz7_bit30);

                // Add comment for ecall
                if (usageInstruction.opcode == 0x73) {
                    mvwprintw(windowManagement.winProg->window, row, SPACE_COMMENT, "// ecall - End program");

                } else {
                    switch (aluOp) {
                        case ALU_ADD:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := %s(0x%08X) + %s(0x%08X) then %s is 0x%08X" : "-- %s := %s(%d) + %s(%d) then %s is %d",
                                register_names[usageInstruction.rd],
                                register_names[usageInstruction.rs1],
                                input1,
                                register_names[usageInstruction.rs2],
                                input2,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case ALU_SUB:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := %s(0x%08X) - %s(0x%08X) then %s is 0x%08X" : "-- %s := %s(%d) - %s(%d) then %s is %d",
                                register_names[usageInstruction.rd],
                                register_names[usageInstruction.rs1],
                                input1,
                                register_names[usageInstruction.rs2],
                                input2,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case ALU_SRA:
                        case ALU_SRL:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := %s(0x%08X) << %s(0x%08X) then %s is 0x%08X" : "-- %s := %s(%d) << %s(%d) then %s is %d",
                                register_names[usageInstruction.rd],
                                register_names[usageInstruction.rs1],
                                input1,
                                register_names[usageInstruction.rs2],
                                input2,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case ALU_SLL:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := %s(0x%08X) >> %s(0x%08X) then %s is 0x%08X" : "-- %s := %s(%d) >> %s(%d) then %s is %d",
                                register_names[usageInstruction.rd],
                                register_names[usageInstruction.rs1],
                                input1,
                                register_names[usageInstruction.rs2],
                                input2,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case ALU_AND:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := %s(0x%08X) & %s(0x%08X) then %s is 0x%08X" : "-- %s := %s(%d) & %s(%d) then %s is %d",
                                register_names[usageInstruction.rd],
                                register_names[usageInstruction.rs1],
                                input1,
                                register_names[usageInstruction.rs2],
                                input2,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case ALU_OR:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := %s(0x%08X) | %s(0x%08X) then %s is 0x%08X" : "-- %s := %s(%d) | %s(%d) then %s is %d",
                                register_names[usageInstruction.rd],
                                register_names[usageInstruction.rs1],
                                input1,
                                register_names[usageInstruction.rs2],
                                input2,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case ALU_XOR:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := %s(0x%08X) ^ %s(0x%08X) then %s is 0x%08X" : "-- %s := %s(%d) ^ %s(%d) then %s is %d",
                                register_names[usageInstruction.rd],
                                register_names[usageInstruction.rs1],
                                input1,
                                register_names[usageInstruction.rs2],
                                input2,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case ALU_ADDI:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := %s(0x%08X) + 0x%08X then %s is 0x%08X" : "-- %s := %s(%d) + %d then %s is %d",
                                register_names[usageInstruction.rd],
                                register_names[usageInstruction.rs1],
                                input1,
                                usageInstruction.immediate,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case LUI:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := 0x%08X then %s is 0x%08X" : "-- %s := %d then %s is %d",
                                register_names[usageInstruction.rd],
                                usageInstruction.immediate,
                                register_names[usageInstruction.rd],
                                usageInstruction.immediate
                            );
                            break;

                        case ALU_AUIPC:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := pc(0x%08X) + 0x%08X then %s is 0x%08X" : "-- %s := pc(%d) + %d then %s is %d",
                                register_names[usageInstruction.rd],
                                input1,
                                usageInstruction.immediate,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case ALU_LW:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := RAM[%s(0x%08X) + 0x%08X] then %s is 0x%08X" : "-- %s := RAM[%s(%d) + %d] then %s is %d",
                                register_names[usageInstruction.rd],   // dst
                                register_names[usageInstruction.rs1],  // base
                                input1,                                // rs1
                                offset,                                // immediate
                                register_names[usageInstruction.rd],   // dst (for “then”)
                                read_ram32bit(main_memory, result)     // load value
                            );

                            break;

                        default:
                            mvwprintw(windowManagement.winProg->window, row, SPACE_COMMENT, "// PC: 0x%08X", cpu->pc);
                            break;
                    }
                }
            }
            wattroff(windowManagement.winProg->window, COLOR_PAIR(5));
        }
    }

    printRegisterTable(
        cpu,
        windowManagement.winRegs->window,
        *charCurrent,
        offset
    );

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
            redrawProgram(
                windowManagement,
                offsetProg,
                data,
                highlightedLine,
                maxRows,
                step,
                usageInstruction,
                charCurrent,
                offset,
                cpu,
                input1,
                input2,
                result,
                main_memory,
                options,
                currentInstructionIndex
            );

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
                    event.bstate & BUTTON1_CLICKED
                ) {

                    // Click win
                    if (event.bstate & BUTTON1_CLICKED) {

                        // Click the left bottom
                        const int clicked_row = event.y - prog_y - 2; // -2 for header and border

                        if (clicked_row < 0) {
                            continue;
                        }

                        if (clicked_row >= visibleRows) {
                            continue;
                        }

                        if (clicked_row >= data->lineCount) {
                            continue;
                        }

                        const int mappedLine = rowToLineMapping[clicked_row];

                        if (mappedLine < 0) {
                            continue;
                        }

                        if (mappedLine >= data->lineCount) {
                            fprintf(stderr, "ERROR: mappedLine=%d >= lineCount=%d\n", mappedLine, data->lineCount);
                            continue;
                        }

                        const int target_instruction = data->lineToInstructionMap[mappedLine];

                        if (target_instruction >= 0 && target_instruction < options.text_size / 4) {
                            cpu->reset_flag = target_instruction;

                            // CLEANUP
                            free(rowToLineMapping);
                            return false;

                        }

                    }
                } else
                    fprintf(stderr, "ERROR: Click outside window bounds\n");

            } else
                fprintf(stderr, "ERROR: getmouse() failed\n");
        }

        if (ch == 'q' || ch == 'Q') {
            quitRequested = true;
            break;
        }

        if (ch == 'e' || ch == 'E') {
            *windowManagement.currentWindow = PROG_WINDOW;

            redraw = true;
            commandWindow(windowManagement.winCmd->window, *windowManagement.currentWindow);
        }

        if (ch == 't' || ch == 'T') {
            *windowManagement.currentWindow = REGS_WINDOW;

            redraw = true;
            commandWindow(windowManagement.winCmd->window, *windowManagement.currentWindow);
        }

        if (ch == 'r' || ch == 'R') {
            *windowManagement.currentWindow = STATUS_WINDOW;

            redraw = true;
            commandWindow(windowManagement.winCmd->window, *windowManagement.currentWindow);
        }

        switch (*windowManagement.currentWindow) {
            case PROG_WINDOW:

                if (ch == KEY_UP && *offsetProg > 0) {
                    (*offsetProg)--;

                    redraw = true;
                    redrawProgram(
                        windowManagement,
                        offsetProg,
                        data,
                        highlightedLine,
                        maxRows,
                        step,
                        usageInstruction,
                        charCurrent,
                        offset,
                        cpu,
                        input1,
                        input2,
                        result,
                        main_memory,
                        options,
                        currentInstructionIndex
                    );
                }

                if (ch == KEY_DOWN) {
                    int maxOffset = data->lineCount - maxRows;
                    if (maxOffset < 0) maxOffset = 0;

                    if (*offsetProg < maxOffset) {
                        (*offsetProg)++;

                        redraw = true;
                        redrawProgram(
                            windowManagement,
                            offsetProg,
                            data,
                            highlightedLine,
                            maxRows,
                            step,
                            usageInstruction,
                            charCurrent,
                            offset,
                            cpu,
                            input1,
                            input2,
                            result,
                            main_memory,
                            options,
                            currentInstructionIndex
                        );
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
                    printRegisterTable(
                         cpu,
                         windowManagement.winRegs->window,
                        *charCurrent,
                         offset
                    );

                }

                if (ch == KEY_DOWN && offset + getmaxy(windowManagement.winRegs->window) - 4 < 32) {
                    offset++;

                    redraw = true;
                    printRegisterTable(
                         cpu,
                         windowManagement.winRegs->window,
                        *charCurrent,
                         offset
                    );

                }

                if (ch == 'd' || ch == 'D') {
                    *charCurrent = 'd';

                    redraw = true;

                    redrawProgram(windowManagement,
                      offsetProg,
                      data,
                      highlightedLine,
                      maxRows,
                      step,
                      usageInstruction,
                      charCurrent,
                      offset,
                      cpu,
                      input1,
                      input2,
                      result,
                      main_memory,
                      options,
                      currentInstructionIndex
                    );

                }

                if (ch == 'h' || ch == 'H') {
                    *charCurrent = 'h';

                    redraw = true;

                    redrawProgram(
                        windowManagement,
                        offsetProg,
                        data,
                        highlightedLine,
                        maxRows,
                        step,
                        usageInstruction,
                        charCurrent,
                        offset,
                        cpu,
                        input1,
                        input2,
                        result,
                        main_memory,
                        options,
                        currentInstructionIndex
                    );

                }
                break;

            default: break;
        }

        if (step > 5 || continueExecution) break;

    }

    // CLEANUP
    free(rowToLineMapping);
    return quitRequested;
}

void redrawProgram(
    const WindowsManagement  windowManagement,
          int*               offsetProg,
    const AssemblyData*      data,
    const int                highlightedLine,
    const int                maxRows,
    const int                step,
    const DecodedInstruction usageInstruction,
    const int*               charCurrent,
    const int                offset,
          Cpu                cpu,
    const int                input1,
    const int                input2,
    const int                result,
          RAM                main_memory,
    const options_t          options,
    const uint32_t           currentInstructionIndex


) {

    printRegisterTable(
         cpu,
         windowManagement.winRegs->window,
        *charCurrent,
         offset
    );

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

        draw_instruction_colored(
            windowManagement.winProg->window,
            data->asmLines[i],
            row,
            i == highlightedLine
        );

        if (i == highlightedLine) {
            // Add comments for debug
            wattron(windowManagement.winProg->window, COLOR_PAIR(5));

            if (currentInstructionIndex < options.text_size) {
                const AluEnumOperation aluOp = getInstructionEnum(usageInstruction.opcode, usageInstruction.funz3, usageInstruction.funz7_bit30);

                // Add comment for ecall
                if (usageInstruction.opcode == 0x73) {
                    mvwprintw(windowManagement.winProg->window, row, SPACE_COMMENT, "// ecall - End program");

                } else {
                    switch (aluOp) {
                        case ALU_ADD:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := %s(0x%08X) + %s(0x%08X) then %s is 0x%08X" : "-- %s := %s(%d) + %s(%d) then %s is %d",
                                register_names[usageInstruction.rd],
                                register_names[usageInstruction.rs1],
                                input1,
                                register_names[usageInstruction.rs2],
                                input2,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case ALU_SUB:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := %s(0x%08X) - %s(0x%08X) then %s is 0x%08X" : "-- %s := %s(%d) - %s(%d) then %s is %d",
                                register_names[usageInstruction.rd],
                                register_names[usageInstruction.rs1],
                                input1,
                                register_names[usageInstruction.rs2],
                                input2,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case ALU_SRA:
                        case ALU_SRL:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := %s(0x%08X) << %s(0x%08X) then %s is 0x%08X" : "-- %s := %s(%d) << %s(%d) then %s is %d",
                                register_names[usageInstruction.rd],
                                register_names[usageInstruction.rs1],
                                input1,
                                register_names[usageInstruction.rs2],
                                input2,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case ALU_SLL:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := %s(0x%08X) >> %s(0x%08X) then %s is 0x%08X" : "-- %s := %s(%d) >> %s(%d) then %s is %d",
                                register_names[usageInstruction.rd],
                                register_names[usageInstruction.rs1],
                                input1,
                                register_names[usageInstruction.rs2],
                                input2,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case ALU_AND:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := %s(0x%08X) & %s(0x%08X) then %s is 0x%08X" : "-- %s := %s(%d) & %s(%d) then %s is %d",
                                register_names[usageInstruction.rd],
                                register_names[usageInstruction.rs1],
                                input1,
                                register_names[usageInstruction.rs2],
                                input2,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case ALU_OR:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := %s(0x%08X) | %s(0x%08X) then %s is 0x%08X" : "-- %s := %s(%d) | %s(%d) then %s is %d",
                                register_names[usageInstruction.rd],
                                register_names[usageInstruction.rs1],
                                input1,
                                register_names[usageInstruction.rs2],
                                input2,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case ALU_XOR:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := %s(0x%08X) ^ %s(0x%08X) then %s is 0x%08X" : "-- %s := %s(%d) ^ %s(%d) then %s is %d",
                                register_names[usageInstruction.rd],
                                register_names[usageInstruction.rs1],
                                input1,
                                register_names[usageInstruction.rs2],
                                input2,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case ALU_ADDI:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := %s(0x%08X) + 0x%08X then %s is 0x%08X" : "-- %s := %s(%d) + %d then %s is %d",
                                register_names[usageInstruction.rd],
                                register_names[usageInstruction.rs1],
                                input1,
                                usageInstruction.immediate,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case LUI:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := 0x%08X then %s is 0x%08X" : "-- %s := %d then %s is %d",
                                register_names[usageInstruction.rd],
                                usageInstruction.immediate,
                                register_names[usageInstruction.rd],
                                usageInstruction.immediate
                            );
                            break;

                        case ALU_AUIPC:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := pc(0x%08X) + 0x%08X then %s is 0x%08X" : "-- %s := pc(%d) + %d then %s is %d",
                                register_names[usageInstruction.rd],
                                input1,
                                usageInstruction.immediate,
                                register_names[usageInstruction.rd],
                                result
                            );
                            break;

                        case ALU_LW:
                            mvwprintw(
                                windowManagement.winProg->window,
                                row,
                                SPACE_COMMENT,
                                *charCurrent == 'h' ? "-- %s := RAM[%s(0x%08X) + 0x%08X] then %s is 0x%08X" : "-- %s := RAM[%s(%d) + %d] then %s is %d",
                                register_names[usageInstruction.rd],   // dst
                                register_names[usageInstruction.rs1],  // base
                                input1,                                // rs1
                                offset,                                // immediate
                                register_names[usageInstruction.rd],   // dst (for “then”)
                                read_ram32bit(main_memory, result)     // load value
                            );

                            break;

                        default:
                            mvwprintw(windowManagement.winProg->window, row, SPACE_COMMENT, "// PC: 0x%08X", cpu->pc);
                            break;
                    }
                }
            }
            wattroff(windowManagement.winProg->window, COLOR_PAIR(5));
        }
    }

    wnoutrefresh(windowManagement.winRegs->window);
    wnoutrefresh(windowManagement.winProg->window);

    if (windowManagement.winStatus->isActive) wnoutrefresh(windowManagement.winStatus->window);

    wnoutrefresh(windowManagement.winCmd->window);
}

void draw_instruction_colored(
    WINDOW* prog_window,
    const char* asm_current_line,
    const int row,
    const bool highlight
) {
    if (highlight) mvwprintw(prog_window, row, 2, "->");

    int col = 4;
    const char* p = asm_current_line;

    // Skip leading whitespace
    while (*p == ' ' || *p == '\t') p++;

    // Reset to beginning of line for display
    p = asm_current_line;

    // First, always print the original instruction regardless of type
        while (*p != '\0' && *p != '#') {
            if (*p == ' ' || *p == '\t') {
                mvwprintw(prog_window, row, col++, "%c", *p);
                p++;
                continue;
            }

            char token[128] = {0};
            int len = 0;
            while (*p != '\0' && *p != ' ' && *p != '\t' && *p != ',' && *p != '#') {
                token[len++] = *p++;
            }
            token[len] = '\0';

            // Skip any comma
            if (*p == ',') {
                token[len++] = *p++;
                token[len] = '\0';
            }

            // Apply appropriate color based on token type
            if (strcmp(token, "add") == 0 || strcmp(token, "and") == 0 ||
                strcmp(token, "or") == 0 || strcmp(token, "sll") == 0  ||
                strcmp(token, "srl") == 0 || strcmp(token, "la") == 0
            ) {
                    wattron(prog_window, COLOR_PAIR(7) | A_BOLD);
                    mvwprintw(prog_window, row, col, "%s", token);
                    wattroff(prog_window, COLOR_PAIR(7) | A_BOLD);

            } else if (strcmp(token, "zero") == 0 || strcmp(token, "ra") == 0 ||
                       strcmp(token, "sp") == 0 || strcmp(token, "gp") == 0   ||
                       strcmp(token, "tp") == 0
            ) {
                    wattron(prog_window, COLOR_PAIR(8) | A_BOLD);
                    mvwprintw(prog_window, row, col, "%s", token);
                    wattroff(prog_window, COLOR_PAIR(8) | A_BOLD);

            } else if (token[0] == 'a') {
                    wattron(prog_window, COLOR_PAIR(9) | A_BOLD);
                    mvwprintw(prog_window, row, col, "%s", token);
                    wattroff(prog_window, COLOR_PAIR(9) | A_BOLD);

            } else if (token[0] == 't') {
                    wattron(prog_window, COLOR_PAIR(10) | A_BOLD);
                    mvwprintw(prog_window, row, col, "%s", token);
                    wattroff(prog_window, COLOR_PAIR(10) | A_BOLD);

            } else if (token[0] == 's') {
                    wattron(prog_window, COLOR_PAIR(11) | A_BOLD);
                    mvwprintw(prog_window, row, col, "%s", token);
                    wattroff(prog_window, COLOR_PAIR(11) | A_BOLD);

            } else {
                    mvwprintw(prog_window, row, col, "%s", token);
            }

            col += len;
        }
}
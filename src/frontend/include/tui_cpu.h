//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef TUI_H
#define TUI_H

#include <string.h>

#include "assembly_data.h"
#include "cpu.h"
#include "decode.h"

extern const char* register_names[32];

void printRegisterTable(
    Cpu     cpu,
    WINDOW* winRegs,
    int     currentSetting,
    int     offset
);

bool printProgramWithCurrentInstruction(
          WindowsManagement windowManagement,
          int*              charCurrent,
          int32_t           input1,
          int32_t           input2,
          int32_t           result,
          Cpu               cpu,
          options_t         options,
    const AssemblyData*     data,
    int*                    offsetProg,
          RAM      main_memory

);

static void drawBlock(
          WINDOW* win,
    const int     y,
    const int     x,
    const char*   label,
    const bool    highlight

) {
    if (highlight) attron(COLOR_BLACK);

    mvwprintw(win, y, x, "[%s]", label);

    if (highlight) attroff(COLOR_BLACK);

}

void drawPipeline(
    WINDOW*            winStatus,
    DecodedInstruction currentDecoded,
    uint32_t           pc,
    int                step

);

void redrawProgram(
          WindowsManagement  windowManagement,
          int*               offsetProg,
    const AssemblyData*      data,
          int                highlightedLine,
          int                maxRows,
          int                step,
          DecodedInstruction usageInstruction,
    const int*               charCurrent,
          int                offset,
          Cpu                cpu,
          int                input1,
          int                input2,
          int                result,
          RAM                main_memory,
          options_t          options,
          uint32_t           currentInstructionIndex
);

void draw_instruction_colored(
          WINDOW* prog_window,
    const char*   asm_current_line,
          int     row,
          bool    highlight
);

static bool contains_pseudo_instruction(const char* line) {
    if (!line) return false;

    char token[128];
    if (sscanf(line, "%127s", token) == 1) {
        if (strcmp(token, "la") == 0) return true;
    }
    return false;
}

#endif //TUI_H

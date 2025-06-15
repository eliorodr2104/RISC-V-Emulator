//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef TUI_H
#define TUI_H

#include "assembly_data.h"
#include "cpu.h"
#include "decode.h"

extern const char* register_names[32];

void printRegisterTable(
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
          Cpu*              cpu,
          options_t         options,
    const AssemblyData*     data,
    int*                    offsetProg

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
    int32_t            pc,
    int                step

);

void redrawProgram(
    WindowsManagement windowManagement,
    int*              offsetProg,
    const AssemblyData* data,
    int        highlightedLine,
    int        maxRows,
    int        step,
    DecodedInstruction usageInstruction,
    const int*      charCurrent,
    int       offset,
    const Cpu* cpu
);

#endif //TUI_H

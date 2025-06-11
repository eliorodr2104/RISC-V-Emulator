//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef TUI_H
#define TUI_H
#include "assemblyData.h"
#include "ncurses.h"
#include "instructionMemory.h"

extern const char* register_names[32];

// ########################
// # CPU print functions  #
// ########################
void printRegisterTable(
    WINDOW* winRegs,
    int     currentSetting,
    int     offset
);

bool printProgramWithCurrentInstruction(
    WINDOW*   winProg,
    WINDOW*   winRegs,
    WINDOW*   winStatus,
    WINDOW*   winCmd,
    Windows*  selectCurrent,
    int*      charCurrent,
    int32_t   input1,
    int32_t   input2,
    int32_t   result,
    int32_t   pc,
    options_t options,
    AssemblyData* data,
    int*      offsetProg
);

static void drawBlock(
    WINDOW* win,
    const int y,
    const int x,
    const char* label,
    const bool highlight

) {

    if (highlight) attron(COLOR_BLACK);
    mvwprintw(win, y, x, "[%s]", label);
    if (highlight) attroff(COLOR_BLACK);

}

void drawPipeline(
    WINDOW* winStatus,
    DecodedInstruction currentDecoded,
    int32_t pc,
    int step

);

#endif //TUI_H

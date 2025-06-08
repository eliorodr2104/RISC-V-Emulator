//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef TUI_H
#define TUI_H
#include "ncurses.h"
#include "instructionMemory.h"

extern const char* register_names[32];

// ########################
// # CPU print functions  #
// ########################
void printRegisterTable(
    WINDOW* winRegs
);

bool printProgramWithCurrentInstruction(
    WINDOW* winProg,
    WINDOW* winRegs,
    WINDOW* winStatus,
    int32_t input1,
    int32_t input2,
    int32_t result,
    int32_t pc
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

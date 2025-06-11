//
// Created by Eliomar Alejandro Rodriguez Ferrer on 08/06/25.
//

#ifndef TUIMAIN_H
#define TUIMAIN_H

#include <ncurses.h>

#include "args_handler.h"
#include "cpu.h"
#include "windows.h"

// ########################
// # Main print functions #
// ########################
void userChoices(
    WINDOW* winProg,
    WINDOW* winRegs,
    WINDOW* winStatus,
    WINDOW* winCmd,
    Cpu* cpu,
    options_t options
);

void commandWindow(
    WINDOW* winCmd,
    Windows window

);

#endif //TUIMAIN_H

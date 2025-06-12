//
// Created by Eliomar Alejandro Rodriguez Ferrer on 08/06/25.
//

#ifndef TUINCURSES_H
#define TUINCURSES_H

#define MIN_ROWS 25
#define MIN_COLS 80

#include <curses.h>

#include "windowsManagement.h"


// ########################
// # Ncurses functions    #
// ########################
bool initNcurses(
    WINDOW** winRegs,
    WINDOW** winProg,
    WINDOW** winStatus,
    WINDOW** winCmd
);

void closeNcurses(
    WINDOW** winRegs,
    WINDOW** winProg,
    WINDOW** winStatus
);

void mvwprintwWrap(WINDOW *win, int starty, int startx, const char *str);

bool handleTerminalResize(const WindowsManagement* windowManagement);

#endif //TUINCURSES_H

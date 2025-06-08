//
// Created by Eliomar Alejandro Rodriguez Ferrer on 08/06/25.
//

#ifndef TUINCURSES_H
#define TUINCURSES_H

#include <curses.h>

// ########################
// # Ncurses functions    #
// ########################
void initNcurses(WINDOW* winRegs, WINDOW* winProg);
void closeNcurses(WINDOW* winRegs, WINDOW* winProg);

#endif //TUINCURSES_H

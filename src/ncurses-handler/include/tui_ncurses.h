//
// Created by Eliomar Alejandro Rodriguez Ferrer on 08/06/25.
//

#ifndef TUINCURSES_H
#define TUINCURSES_H

#define MIN_ROWS 25
#define MIN_COLS 80

#include <curses.h>

#include "windows_management.h"

bool initNcurses(
    WindowsManagement windowsManagement
);

void closeNcurses(
    WINDOW** winRegs,
    WINDOW** winProg,
    WINDOW** winStatus
);

void mvwprintwWrap(WINDOW *win, int starty, int startx, const char *str);

bool handleTerminalResize(WindowsManagement* windowManagement);

bool recreateWindows(const WindowsManagement* windowManagement);

/**
 * @brief Prints the header of a window with a title and color.
 *
 * @param win The window where the header will be printed.
 * @param start_y The starting y-coordinate for the header.
 * @param start_x The starting x-coordinate for the header.
 * @param title  The title to be printed in the header.
 * @param color_pair The color pair to be used for the header text.
 * @param style_text  The text style to be applied (e.g., A_BOLD).
 */
static void print_header_window(
          WINDOW* win,
    const int   start_y,
    const int   start_x,
    const char* title,
    const int   color_pair,
    const int   style_text

) {

    wattron  (win,  color_pair | style_text);
    mvwprintw(win, start_y, start_x, title);
    wattroff (win, COLOR_PAIR(2) | A_BOLD);

}

static void restart_window(WINDOW* window) {
    werase(window);
    wbkgd(window, COLOR_PAIR(0));
    box(window, 0, 0);
}

#endif //TUINCURSES_H

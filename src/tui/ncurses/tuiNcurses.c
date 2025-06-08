//
// Created by Eliomar Alejandro Rodriguez Ferrer on 08/06/25.
//

// ########################
// # Ncurses functions    #
// ########################

#include <curses.h>
#include <locale.h>

void initNcurses(WINDOW* winRegs, WINDOW* winProg) {
    setlocale(LC_ALL, "");

    // Init screen TUI
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK,  COLOR_WHITE);  // pair 1: header / title
        init_pair(2, COLOR_BLACK,  COLOR_WHITE);  // pair 2: status bar
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);  // pair 3: current instruction
        init_pair(4, COLOR_GREEN,  COLOR_BLACK);  // pair 4: text / result
        init_pair(5, COLOR_CYAN,   COLOR_BLACK);  // pair 5: all registers
        init_pair(6, COLOR_WHITE,  COLOR_BLACK);   // Righe alternate (opzionale)
    }

    // Calc dim and create windows
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    const int regsHeight = rows;
    const int regsWidth  = cols / 3;

    // Divide screen in two columns
    const int progHeight = rows;
    const int progWidth  = cols - regsWidth;

    // Left Window
    winProg = newwin(progHeight, progWidth, 0, 0);

    // Right Window
    winRegs = newwin(regsHeight, regsWidth, 0, progWidth);

    //Draw borders and title default
    wbkgd(winProg, COLOR_PAIR(0));  // background black, text white default

    box(winProg, 0, 0);
    wattron(winProg, COLOR_PAIR(1) | A_BOLD);

    mvwprintw(winProg, 0, 2, " PROGRAM EXECUTION STATUS ");

    wattroff(winProg, COLOR_PAIR(1) | A_BOLD);

    wrefresh(winProg);

    wbkgd(winRegs, COLOR_PAIR(0));
    box(winRegs, 0, 0);
    wattron(winRegs, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(winRegs, 0, 2, " REGISTER STATE ");
    wattroff(winRegs, COLOR_PAIR(1) | A_BOLD);
    wrefresh(winRegs);
}

void closeNcurses(WINDOW* winRegs, WINDOW* winProg) {
    refresh();

    // Clean
    delwin(winProg);
    delwin(winRegs);
    endwin();
}
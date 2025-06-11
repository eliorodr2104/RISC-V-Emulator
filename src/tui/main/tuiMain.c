//
// Created by Eliomar Alejandro Rodriguez Ferrer on 08/06/25.
//

#include "tuiMain.h"

#include <stdlib.h>

#include "cpu.h"

void userChoices(
    WINDOW* winProg,
    WINDOW* winRegs,
    WINDOW* winStatus,
    WINDOW* winCmd,
    Cpu* cpu,
    options_t options
) {

    Windows* currentWindow = malloc(sizeof *currentWindow);
    if (!currentWindow) { perror("malloc error"); return; }

    *currentWindow = PROG_WINDOW;

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    WINDOW* menuWin = newwin(7, 40, (rows-7)/2, (cols-40)/2);
    box(menuWin, 0, 0);
    wattron(menuWin, COLOR_PAIR(2) | A_BOLD);
    mvwprintw(menuWin, 1, 2, "Select mode:");
    wattroff(menuWin, COLOR_PAIR(2) | A_BOLD);

    constexpr int n_choices = 3;
    int highlight = 0;

    const char* choices[] = {
        "1 - Full execution",
        "2 - Step by step",
        "3 - Exit"
    };

    keypad(menuWin, TRUE);

    while (1) {
        for (int i = 0; i < n_choices; i++) {

            if (i == highlight) {
                wattron(menuWin, A_REVERSE);
                mvwprintw(menuWin, i + 3, 4, "%s", choices[i]);
                wattroff(menuWin, A_REVERSE);

            } else {
                mvwprintw(menuWin, i + 3, 4, "%s", choices[i]);

            }
        }
        wrefresh(menuWin);

        const int c = wgetch(menuWin);
        switch (c) {
            case KEY_UP:
                highlight = (highlight == 0) ? n_choices - 1 : highlight - 1;
                break;

            case KEY_DOWN:
                highlight = (highlight == n_choices - 1) ? 0 : highlight + 1;
                break;

            case 10:

                if (highlight == 0) {
                    runCpuFull(winProg, winRegs, winStatus, winCmd, currentWindow, cpu, options);


                } else if (highlight == 1) {
                    commandWindow   (winCmd, *currentWindow);
                    runCpuStepByStep(winProg, winRegs, winStatus, winCmd, currentWindow, cpu, options);


                } else {
                    endwin();
                    exit(0);

                }

                return;

            default:
                return;
        }
    }
}

void commandWindow(
          WINDOW* winCmd,
    const Windows window

) {

    // Header definition
    werase(winCmd);
    wbkgd(winCmd, COLOR_PAIR(0)); // background black, text white default
    box(winCmd, 0, 0);

    wattron(winCmd,  COLOR_PAIR(1) | A_BOLD);
    mvwprintw(winCmd, 0, 2, " Help ");
    wattroff(winCmd, COLOR_PAIR(1) | A_BOLD);


    switch (window) {

        case PROG_WINDOW:

            mvwprintw(winCmd, 1, 2, "<");
            wattron(winCmd,  COLOR_PAIR(7) | A_BOLD);
            mvwprintw(winCmd, 1, 3, "q");
            wattroff(winCmd, COLOR_PAIR(7) | A_BOLD);
            mvwprintw(winCmd, 1, 4, ":");
            mvwprintw(winCmd, 1, 5, " Quit>");

            mvwprintw(winCmd, 1, 13, "<");
            wattron(winCmd,  COLOR_PAIR(7) | A_BOLD);
            mvwprintw(winCmd, 1, 14, "j");
            wattroff(winCmd, COLOR_PAIR(7) | A_BOLD);
            mvwprintw(winCmd, 1, 15, ": ");
            mvwprintw(winCmd, 1, 17, "Jump>");

            break;


        case REGS_WINDOW:
            mvwprintw(winCmd, 1, 2, "<");
            wattron(winCmd,  COLOR_PAIR(7) | A_BOLD);
            mvwprintw(winCmd, 1, 3, "q");
            wattroff(winCmd, COLOR_PAIR(7) | A_BOLD);
            mvwprintw(winCmd, 1, 4, ":");
            mvwprintw(winCmd, 1, 5, " Quit>");

            mvwprintw(winCmd, 1, 13, "<");
            wattron(winCmd,  COLOR_PAIR(7) | A_BOLD);
            mvwprintw(winCmd, 1, 14, "d");
            wattroff(winCmd, COLOR_PAIR(7) | A_BOLD);
            mvwprintw(winCmd, 1, 15, ": ");
            mvwprintw(winCmd, 1, 17, "Dec>");

            mvwprintw(winCmd, 1, 23, "<");
            wattron(winCmd,  COLOR_PAIR(7) | A_BOLD);
            mvwprintw(winCmd, 1, 24, "h");
            wattroff(winCmd, COLOR_PAIR(7) | A_BOLD);
            mvwprintw(winCmd, 1, 25, ": ");
            mvwprintw(winCmd, 1, 28, "Hex>");

            break;


        case STATUS_WINDOW:
            mvwprintw(winCmd, 1, 2, "<");
            wattron(winCmd,  COLOR_PAIR(7) | A_BOLD);
            mvwprintw(winCmd, 1, 3, "q");
            wattroff(winCmd, COLOR_PAIR(7) | A_BOLD);
            mvwprintw(winCmd, 1, 4, ":");
            mvwprintw(winCmd, 1, 5, " Quit>");

            mvwprintw(winCmd, 1, 13, "<");
            wattron(winCmd,  COLOR_PAIR(7) | A_BOLD);
            mvwprintw(winCmd, 1, 14, "n");
            wattroff(winCmd, COLOR_PAIR(7) | A_BOLD);
            mvwprintw(winCmd, 1, 15, ": ");
            mvwprintw(winCmd, 1, 17, "Next>");

            break;

        default: break;
    }

    wrefresh(winCmd);

}

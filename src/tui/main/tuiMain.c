//
// Created by Eliomar Alejandro Rodriguez Ferrer on 08/06/25.
//

#include "tuiMain.h"

#include <stdlib.h>

#include "cpu.h"
#include "../../include/tui/cpu/tuiCpu.h"

void userChoices(
    WINDOW* winProg,
    WINDOW* winRegs,
    WINDOW* winStatus,
    WINDOW* winCmd,
    Cpu* cpu
) {

    Windows* currentWindow = malloc(sizeof *currentWindow);
    if (!currentWindow) perror("malloc error");

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

                //printProgramWithCurrentInstruction(winProg, winRegs, winStatus, 0, 0, 0, cpu->pc);

                if (highlight == 0) {
                    runCpuFull(winProg, winRegs, cpu);


                } else if (highlight == 1) {
                    commandWindow   (winCmd, *currentWindow);
                    runCpuStepByStep(winProg, winRegs, winStatus, cpu);


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
    Windows window

) {

    // Header definition
    werase(winCmd);
    wbkgd(winCmd, COLOR_PAIR(0)); // background black, text white default

    mvwprintw(winCmd, 0, 1, " Enter command: ");
    wrefresh(winCmd);

}

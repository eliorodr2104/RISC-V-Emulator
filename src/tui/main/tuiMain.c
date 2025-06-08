//
// Created by Eliomar Alejandro Rodriguez Ferrer on 08/06/25.
//

#include <stdlib.h>

#include "cpu.h"
#include "../../include/tui/cpu/tuiCpu.h"

void userChoices(
    WINDOW* winProg,
    WINDOW* winRegs,
    Cpu* cpu
) {

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

        int c = wgetch(menuWin);
        switch (c) {
            case KEY_UP:
                highlight = (highlight == 0) ? n_choices - 1 : highlight - 1;
                break;

            case KEY_DOWN:
                highlight = (highlight == n_choices - 1) ? 0 : highlight + 1;
                break;

            case 10:
                delwin(menuWin);

                clear();
                refresh();

                werase(winProg);
                werase(winRegs);
                box(winProg, 0, 0);
                box(winRegs, 0, 0);

                wattron(winProg, COLOR_PAIR(1) | A_BOLD);
                mvwprintw(winProg, 0, 2, " PROGRAM EXECUTION STATUS ");
                wattroff(winProg, COLOR_PAIR(1) | A_BOLD);

                wattron(winRegs, COLOR_PAIR(1) | A_BOLD);
                mvwprintw(winRegs, 0, 2, " REGISTER STATE ");
                wattroff(winRegs, COLOR_PAIR(1) | A_BOLD);

                printProgramWithCurrentInstruction(winProg, winRegs, 0, 0, 0, cpu->pc);

                if (highlight == 0) {
                    runCpuFull(winProg, winRegs, cpu);


                } else if (highlight == 1) {
                    runCpuStepByStep(winProg, winRegs, cpu);

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

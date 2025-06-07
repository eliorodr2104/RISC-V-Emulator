#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "ncurses.h"
#include "tui.h"

void userChoicesNcurses(
    WINDOW* winProg,
    WINDOW* winRegs,
    Cpu* cpu
);

int main(void) {
    //headerProgram();

    initscr(); // Init screen TUI
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE,  COLOR_BLUE);   // pair 1: header / title
        init_pair(2, COLOR_BLACK,  COLOR_WHITE);  // pair 2: status bar
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);  // pair 3: current instruction
        init_pair(4, COLOR_GREEN,  COLOR_BLACK);  // pair 4: text / result
        init_pair(5, COLOR_CYAN,   COLOR_BLACK);  // pair 5: all registers
    }

    // 2. Calc dim and create windows
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    // (~rows-4) for list instructions and 4 rows for registers
    const int prog_height = rows - 5;
    const int prog_width  = cols;
    constexpr int regs_height = 5;
    const int regs_width  = cols;

    WINDOW* winProg = newwin(prog_height, prog_width, 0,  0);
    WINDOW* winRegs = newwin(regs_height, regs_width, prog_height, 0);

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

    Cpu* cpu = newCpu();

    userChoicesNcurses(winProg, winRegs, cpu);

    refresh();

    // Clean
    delwin(winProg);
    delwin(winRegs);
    endwin();

    free(cpu);
    return 0;
}

void userChoicesNcurses(
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


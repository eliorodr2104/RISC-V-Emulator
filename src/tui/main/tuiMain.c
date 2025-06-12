//
// Created by Eliomar Alejandro Rodriguez Ferrer on 08/06/25.
//

#include "tuiMain.h"

#include <stdlib.h>

#include "cpu.h"
#include "tuiNcurses.h"

/**
 * @brief Function to show the mode chooser window
 *
 * @param windowManagement
 * Current window management structure
 * @param cpu
 * Cpu structure containing the CPU state and their flags
 * @param options
 * Options structure containing the execution mode and other options
 * @param rows
 * Current terminal rows
 * @param cols
 * Current terminal columns
 */
void show_mode_chooser_window(
    const WindowsManagement windowManagement,
          Cpu*              cpu,
    const options_t         options,
    const int32_t           rows,
    const int32_t           cols
) {

    // Create a new window for the mode chooser
    WINDOW* menuWin = newwin(7, 40, (rows-7)/2, (cols-40)/2);

    if (!menuWin) return;
    constexpr int n_choices = 3;
    int highlight = 0;

    const char* choices[] = {
        "1 - Full execution",
        "2 - Step by step",
        "3 - Exit"
    };

    // Set the background color and border for the menu window
    box(menuWin, 0, 0);

    // Set title for the menu window
    wattron(menuWin, COLOR_PAIR(2) | A_BOLD);
    mvwprintw(menuWin, 1, 2, "Select mode:");
    wattroff(menuWin, COLOR_PAIR(2) | A_BOLD);

    // Set the keypad for the menu window to capture arrow keys
    keypad(menuWin, TRUE);

    while (1) {

        // Show select and unselect rows
        for (int i = 0; i < n_choices; i++) {

            if (i == highlight) {
                wattron(menuWin, A_REVERSE);
                mvwprintw(menuWin, i + 3, 4, "%s", choices[i]);
                wattroff(menuWin, A_REVERSE);

            } else {
                mvwprintw(menuWin, i + 3, 4, "%s", choices[i]);

            }
        }

        // Refresh the menu window to show the changes
        wrefresh(menuWin);

        // Wait for user input and handle it
        const int c = wgetch(menuWin);

        // Handle user input for navigation and selection
        switch (c) {
            case KEY_UP:
                highlight = (highlight == 0) ? n_choices - 1 : highlight - 1;
                break;

            case KEY_DOWN:
                highlight = (highlight == n_choices - 1) ? 0 : highlight + 1;
                break;

            case 10:

                if (highlight == 0) {
                    runCpuFull(windowManagement, cpu, options);


                } else if (highlight == 1) {
                    if (windowManagement.winCmd->isActive) commandWindow(windowManagement.winCmd->window, *windowManagement.currentWindow);

                    runCpuStepByStep(windowManagement, cpu, options);


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

/**
 * @brief Function to handle user choices for execution mode
 *
 * @param windowManagement
 * Current window management structure
 * @param cpu
 * Cpu structure containing the CPU state and their flags
 * @param options
 * Current options structure containing the execution mode and other options
 */
void userChoices(
          WindowsManagement windowManagement,
          Cpu*              cpu,
    const options_t         options
) {

    // Allocate memory for the current window pointer
    windowManagement.currentWindow = malloc(sizeof windowManagement.currentWindow);

    // Check if memory allocation was successful
    if (!windowManagement.currentWindow) { perror("Alloc memory for the current windows is not successfully"); return; }

    // Set the current window to the program window (default -> PROG_WINDOW)
    *windowManagement.currentWindow= PROG_WINDOW;

    int32_t rows, cols;

    // Get the current terminal size
    getmaxyx(stdscr, rows, cols);

    // Select current window based on the execution mode
    switch (options.execution_mode) {
        case DEFAULT:
            show_mode_chooser_window(windowManagement, cpu, options, rows, cols);
            break;

        case FULL:
            runCpuFull(windowManagement, cpu, options);
            break;

        case STEP_BY_STEP:
            if (windowManagement.winCmd->isActive) commandWindow(windowManagement.winCmd->window, *windowManagement.currentWindow);

            runCpuStepByStep(windowManagement, cpu, options);
            break;
        default:
            break;
    }

    closeNcurses(&windowManagement.winRegs->window, &windowManagement.winProg->window, &windowManagement.winStatus->window);
    free(windowManagement.currentWindow);

}

/**
 * @brief Function to draw the command window with help information
 *
 * @param winCmd
 * The window where the command help will be displayed
 * @param window
 * Given window type to display specific commands
 */
void commandWindow(
          WINDOW* winCmd,
    const Windows window
) {
    // Header definition and set background color and border
    werase(winCmd);
    wbkgd(winCmd, COLOR_PAIR(0));
    box(winCmd, 0, 0);

    // Set title for the command window
    wattron(winCmd,  COLOR_PAIR(1) | A_BOLD);
    mvwprintw(winCmd, 0, 2, " Help ");
    wattroff(winCmd, COLOR_PAIR(1) | A_BOLD);

    // Command definitions and draw
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

//
// Created by Eliomar Alejandro Rodriguez Ferrer on 12/06/25.
//

#ifndef WINDOWSMANAGEMENT_H
#define WINDOWSMANAGEMENT_H

#include <ncurses.h>

/**
 * @brief Enum to manage the different windows in the TUI
 */
typedef enum {
    PROG_WINDOW,
    REGS_WINDOW,
    STATUS_WINDOW,
    CMD_WINDOW,
} Windows;

typedef struct {
    WINDOW *window;
    bool isActive;
} WindowAndStatus;

/**
 * @brief Struct to manage the windows in the TUI
 *
 * @var WindowsManagement::winProg
 * Pointer to the program window
 * @var WindowsManagement::winRegs
 * Pointer to the registers window
 * @var WindowsManagement::winStatus
 * Pointer to the status window
 * @var WindowsManagement::winCmd
 * Pointer to the command window
 * @var WindowsManagement::currentWindow
 * Pointer to the current window being used
 */
typedef struct {
    WindowAndStatus *winProg;
    WindowAndStatus *winRegs;
    WindowAndStatus *winStatus;
    WindowAndStatus *winCmd;
    Windows *currentWindow;
} WindowsManagement;


#endif //WINDOWSMANAGEMENT_H

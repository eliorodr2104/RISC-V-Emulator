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
    WindowAndStatus *window_left;
    WindowAndStatus *window_right;
    WindowAndStatus *window_bottom_right;
    WindowAndStatus *bottom_window;
    Windows         *window_on_focus;
    int8_t          current_window_character;

} WindowsManagement;

void destroy_windows_management(WindowsManagement window_management);


#endif //WINDOWSMANAGEMENT_H

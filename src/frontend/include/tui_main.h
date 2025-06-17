//
// Created by Eliomar Alejandro Rodriguez Ferrer on 08/06/25.
//

#ifndef TUIMAIN_H
#define TUIMAIN_H

#include "args_handler.h"
#include "cpu.h"
#include "windows_management.h"

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
    WindowsManagement windowManagement,
    Cpu               cpu,
    options_t         options,
    int32_t           rows,
    int32_t           cols
);

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
    Cpu               cpu,
    options_t         options
);

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
    Windows window

);

#endif //TUIMAIN_H

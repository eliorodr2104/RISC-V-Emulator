#include <stdlib.h>

#include "args_handler.h"
#include "cpu.h"
#include "tui_ncurses.h"
#include "tui_main.h"

/**
 * @brief Main function to run the RISC-V CPU simulator
 * @param argc
 * Number of command line arguments
 * @param argv
 * array of command line arguments for define options execution mode, instruction count, etc.
 * @return 0 if successful, non-zero otherwise
 */
int main(const int argc, char** argv) {

    // Initialize options' structure with null
    options_t opts = { nullptr };

    // Handle command line arguments and set options with the provided arguments
    const int result = handle_args(argc, argv, &opts);
    if (result != 0)
        return result;

    // Set the instruction count aligned to 4 bytes for RISC-V 32-bit architecture
    opts.instruction_count_aligned = opts.instruction_count * 4;

    // Initialize the CPU structure
    Cpu cpu = newCpu();

    // Initialize ncurses windows for the TUI
    WINDOW* winProg   = nullptr;
    WINDOW* winRegs   = nullptr;
    WINDOW* winStatus = nullptr;
    WINDOW* winCmd    = nullptr;

    // Create a structure to manage the windows and passed the current windows and the struct
    const WindowsManagement winManagement = {
        .winProg   = &(WindowAndStatus) { .window = winProg,   .isActive = true },
        .winRegs   = &(WindowAndStatus) { .window = winRegs,   .isActive = true },
        .winStatus = &(WindowAndStatus) { .window = winStatus, .isActive = true },
        .winCmd    = &(WindowAndStatus) { .window = winCmd,    .isActive = true }
    };

    // Initialize ncurses and create windows for program, registers, status, and command
    if (!initNcurses(winManagement)) return 1;

    // Show the mode chooser window to select the execution mode
    userChoices(winManagement, cpu, opts);

    // Close ncurses windows and clean up resources
    closeNcurses(&winRegs, &winProg, &winStatus);

    // Free the CPU structure and options
    free(cpu);

    // Free the options' structure
    free_options(&opts);

    return 0;
}


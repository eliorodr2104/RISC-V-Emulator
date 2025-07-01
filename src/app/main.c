#include <stdlib.h>

#include "args_handler.h"
#include "cpu.h"
#include "ram.h"
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

    // RAM Initialization 1MB
    RAM main_memory = new_ram(1024 * 1024);

    if (!main_memory) return 1; // Ram allocation failed

    // Initialize the CPU structure
    Cpu cpu = newCpu();

    // Load .data (variable's)
    load_binary_to_ram(main_memory, opts.data_data, opts.data_size, opts.data_vaddr);

    // Load .text (Instructions)
    load_binary_to_ram(main_memory, opts.text_data, opts.text_size, opts.text_vaddr);

    // Set PC (Program Counter) to the start of the text section
    cpu->pc = opts.entry_point;

    // Set stack pointer (SP) to the end of the RAM
    cpu->registers[2] = 0x100000;

    // Test to print the RAM state from address 0 to the stack pointer
    //print_ram_state(main_memory, opts.data_vaddr, opts.data_vaddr + opts.data_size, 32);

    //print_ram_state(main_memory, opts.text_vaddr, opts.text_vaddr + opts.text_size, 32);

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
    userChoices(winManagement, cpu, opts, main_memory);

    // Close ncurses windows and clean up resources
    closeNcurses(&winRegs, &winProg, &winStatus);

    // Free the CPU structure and options
    free(cpu);

    // Free the options' structure
    free_options(&opts);

    return 0;
}


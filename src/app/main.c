#include <stdlib.h>

#include "args_handler.h"
#include "cpu.h"
#include "ram.h"
#include "tui_ncurses.h"
#include "tui_main.h"
#include "windows_management.h"

// #define DEBUG

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
    options_t* opts = calloc(1, sizeof(options_t));

    // Handle command line arguments and set options with the provided arguments
    const int result = handle_args(argc, argv, opts);

    if (result == 1) {
        return result;
    }

    if (result != 0) {
        perror("Error handling command line");
        return result;
    }

    // RAM Initialization
    RAM main_memory = new_ram(opts->ram_size);

    if (!main_memory) {
        perror("Error init RAM");
        return -1;
    }

    // Initialize the CPU structure
    Cpu cpu = newCpu();

    if (!cpu) {
        perror("Error init CPU");
        destroy_ram(main_memory);
        free_options(opts);
        return -1;
    }

    // Load .data (variable's)
    load_binary_to_ram(main_memory, opts->data_data, opts->data_size, opts->data_vaddr);

    // Load .text (Instructions)
    load_binary_to_ram(main_memory, opts->text_data, opts->text_size, opts->text_vaddr);

    // Set PC (Program Counter) to the start of the text section
    cpu->pc = opts->entry_point;

    // Set stack pointer (SP) to the end of the RAM
    cpu->registers[2] = 0x100000;

    // Test to print the RAM state from address 0 to the stack pointer
#ifdef DEBUG
    print_ram_state(main_memory, opts->data_vaddr, opts->data_vaddr + opts->data_size, 32);

    print_ram_state(main_memory, opts->text_vaddr, opts->text_vaddr + opts->text_size, 32);
#endif

    // Initialize ncurses windows for the TUI
    WINDOW* winProg   = nullptr;
    WINDOW* winRegs   = nullptr;
    WINDOW* winStatus = nullptr;
    WINDOW* winCmd    = nullptr;

    // Create a structure to manage the windows and passed the current windows and the struct
     WindowsManagement winManagement = {
        .winProg   = &(WindowAndStatus) { .window = winProg,   .isActive = true },
        .winRegs   = &(WindowAndStatus) { .window = winRegs,   .isActive = true },
        .winStatus = &(WindowAndStatus) { .window = winStatus, .isActive = true },
        .winCmd    = &(WindowAndStatus) { .window = winCmd,    .isActive = true }
    };

    // Initialize ncurses and create windows for program, registers, status, and command
    if (!initNcurses(winManagement)) {
        perror("Error initializing Ncurses");
        destroy_ram(main_memory);
        free_options(opts);
        destroy_cpu(cpu);
        destroy_windows_management(winManagement);
        return -1;
    }

    // Show the mode chooser window to select the execution mode
#ifndef DEBUG
    userChoices(winManagement, cpu, opts, main_memory);
#endif

    // Close ncurses windows and clean up resources
    closeNcurses(&winRegs, &winProg, &winStatus);

    // Free structure's
    destroy_ram(main_memory);
    free_options(opts);
    destroy_cpu(cpu);
    destroy_windows_management(winManagement);

    return 0;
}


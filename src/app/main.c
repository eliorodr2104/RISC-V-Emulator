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
    options_t* options = calloc(1, sizeof(options_t));

    // Handle command line arguments and set options with the provided arguments
    const int result = handle_args(argc, argv, options);

    if (result == 1) {
        return result;
    }

    if (result != 0) {
        perror("Error handling command line");
        return result;
    }

    // RAM Initialization
    RAM main_memory = new_ram(options->ram_size);

    if (!main_memory) {
        perror("Error init RAM");
        return -1;
    }

    // Initialize the CPU structure
    Cpu cpu = newCpu();

    if (!cpu) {
        perror("Error init CPU");
        destroy_ram(main_memory);
        free_options(options);
        return -1;
    }

    // Load .data (variable's)
    load_binary_to_ram(main_memory, options->data_data, options->data_size, options->data_vaddr);

    // Load .text (Instructions)
    load_binary_to_ram(main_memory, options->text_data, options->text_size, options->text_vaddr);

    // Set PC (Program Counter) to the start of the text section
    cpu->pc = options->entry_point;

    // Set stack pointer (SP) to the end of the RAM
    cpu->registers[2] = 0x100000;

    // Test to print the RAM state from address 0 to the stack pointer
#ifdef DEBUG
    print_ram_state(main_memory, opts->data_vaddr, opts->data_vaddr + opts->data_size, 32);

    print_ram_state(main_memory, opts->text_vaddr, opts->text_vaddr + opts->text_size, 32);
#endif

    // Initialize ncurses windows for the TUI
    WINDOW* code_window       = nullptr;
    WINDOW* memory_window     = nullptr;
    WINDOW* cpu_status_window = nullptr;
    WINDOW* commands_window   = nullptr;

    // Create a structure to manage the windows and passed the current windows and the struct
     const WindowsManagement winManagement = {
        .window_left              = &(WindowAndStatus) { .window = code_window,   .isActive = true },
        .window_right             = &(WindowAndStatus) { .window = memory_window,   .isActive = true },
        .window_bottom_right      = &(WindowAndStatus) { .window = cpu_status_window, .isActive = true },
        .bottom_window            = &(WindowAndStatus) { .window = commands_window,    .isActive = true },
        .current_window_character = 'h'
    };

    // Initialize ncurses and create windows for program, registers, status, and command
    if (!initNcurses(winManagement)) {
        fprintf(stderr, "Failed to initialize curses\n");

        destroy_ram(main_memory);
        free_options(options);
        destroy_cpu(cpu);
        destroy_windows_management(winManagement);
        return -1;
    }

    // Show the mode chooser window to select the execution mode
#ifndef DEBUG
    AssemblyData* data = newAssemblyData(options);

    if (!data) {
        fprintf(stderr, "Error allocating assembly data\n");

        destroy_ram(main_memory);
        free_options(options);
        destroy_cpu(cpu);
        destroy_windows_management(winManagement);
        free(data);
        return -1;
    }

    userChoices(winManagement, cpu, data, options, main_memory);
#endif

    // Close ncurses windows and clean up resources
    closeNcurses(&memory_window, &code_window, &cpu_status_window);

    // Free structure's
    destroy_ram(main_memory);
    free_options(options);
    destroy_cpu(cpu);
    destroy_windows_management(winManagement);

    return 0;
}


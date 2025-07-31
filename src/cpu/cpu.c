
/**
 * @file cpu.c
 * @brief Source file for the logic functions.
 *
 * @author eliorodr2104
 * @date 02/06/25
 *
 */

#include <stdlib.h>

#include "cpu.h"

#include "assembly_data.h"
#include "execute.h"
#include "windows_management.h"

/**
 * @brief Destroy the CPU instance and free its resources.
 *
 * @param cpu Pointer to the CPU instance to be destroyed.
 *
 * This function frees the memory allocated for the CPU instance.
 */
void destroy_cpu(Cpu cpu) {
    if (!cpu) return;

    free(cpu);
}

/**
 * @brief Create a new CPU instance
 *
 * @return A pointer to the newly created CPU instance, or nullptr if memory allocation fails
 */
Cpu newCpu() {
    // Allocate memory for the CPU structure
    Cpu cpu = malloc(sizeof(struct cpu));

    // Check if memory allocation was successful
    if (!cpu) return nullptr;

    // Initialize the CPU state
    cpu->pc = 0;
    cpu->reset_flag = -1;

    for (int i = 0; i < 32; i++) {
       cpu->registers[i] = 0;
    }

    // Return CPU instance
    return cpu;
}

/**
 * @brief Run the CPU in full execution mode, executing all instructions until completion.
 *
 * @param window_management The management structure for the TUI windows
 * @param cpu The CPU instance to run
 * @param options The options for execution
 */
void runCpuFull(
    Cpu               cpu,
    options_t         options,
    WindowsManagement window_management
) {
    // int currentChar = 'h';
    //const AssemblyData* data = newAssemblyData(options);

    /*while (cpu->pc < options.instruction_count_aligned) {
        executeSingleStep(windowManagement, &currentChar, cpu, options, data, 0);

    }*/

}

/**
 * @brief Run the CPU in step-by-step execution mode, allowing for interactive debugging.
 *
 * @param window_management The management structure for the TUI windows
 * @param cpu The CPU instance to run
 * @param options The options for execution
 */
void runCpuStepByStep(
          Cpu               cpu,
          AssemblyData*     data,
    const options_t         options,
    const WindowsManagement window_management,
          RAM               main_memory
) {

    // TODO(Add "currentChar" to the WindowsManagement struct, and pass AssemblyData to the function)

    if (!data) {
        fprintf(stderr, "Error initializing AssemblyData");
        return;
    }

    // While the program counter is less than the aligned instruction count
    while (cpu->pc >= options.text_vaddr && cpu->pc < options.text_vaddr + options.text_size) {

        // Execute a single step of the CPU, if the execution fails then break the loop
        if (executeSingleStep(cpu, options, data, window_management, true, main_memory) == -1) {
            break;
        }

        // If the reset flag is set to a valid value, re-execute instructions until the target instruction
        if (cpu->reset_flag >= 0) {

            // Reset the CPU state to the initial state
            reExecuteUntilTarget(cpu, options, main_memory);

            // Reset the reset flag to -1 to indicate that the CPU is no longer in a reset state
            cpu->reset_flag = -1;
        }

    }

}

/**
 * @brief Reset the CPU state to its initial values.
 *
 * @param cpu The CPU instance to reset
 */
void resetCpuState(Cpu cpu, options_t options) {
    // Reset the program counter
    cpu->pc = options.entry_point;

    // Reset all registers to 0
    for (int i = 1; i < 32; i++) {
        writeRegister(cpu, i, 0);
    }
}

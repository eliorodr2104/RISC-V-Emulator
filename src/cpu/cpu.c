//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include <stdlib.h>

#include "include/cpu.h"
#include "assembly_data.h"
#include "execute.h"
#include "register_memory.h"
#include "windows_management.h"

/**
 * @brief Create a new CPU instance
 *
 * @return A pointer to the newly created CPU instance, or nullptr if memory allocation fails
 */
Cpu* newCpu() {
    // Allocate memory for the CPU structure
    Cpu* cpu = malloc(sizeof(Cpu));

    // Check if memory allocation was successful
    if (!cpu) return nullptr;

    // Initialize the CPU state
    cpu->pc = 0;
    cpu->resetFlag = -1;

    /*for (int i = 0; i < 32; i++) {
        writeRegister(i, 0);
    }*/

    // Return CPU instance
    return cpu;
}

/**
 * @brief Run the CPU in full execution mode, executing all instructions until completion.
 *
 * @param windowManagement The management structure for the TUI windows
 * @param cpu The CPU instance to run
 * @param options The options for execution
 */
void runCpuFull(
    const WindowsManagement windowManagement,
          Cpu*              cpu,
    const options_t         options
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
 * @param windowManagement The management structure for the TUI windows
 * @param cpu The CPU instance to run
 * @param options The options for execution
 */
void runCpuStepByStep(
    const WindowsManagement windowManagement,
          Cpu*              cpu,
    const options_t         options
) {

    // TODO(Add "currentChar" to the WindowsManagement struct, and pass AssemblyData to the function)
          int           currentChar = 'h';
    const AssemblyData* data        = newAssemblyData(options);

    // While the program counter is less than the aligned instruction count
    while (cpu->pc < options.instruction_count_aligned) {

        // Execute a single step of the CPU, if the execution fails then break the loop
        if (!executeSingleStep(windowManagement, &currentChar, cpu, options, data, true)) {
            break;
        }

        // If the reset flag is set to a valid value, re-execute instructions until the target instruction
        if (cpu->resetFlag >= 0) {

            // Reset the CPU state to the initial state
            reExecuteUntilTarget(cpu, options);

            // Reset the reset flag to -1 to indicate that the CPU is no longer in a reset state
            cpu->resetFlag = -1;
        }

    }

}

/**
 * @brief Reset the CPU state to its initial values.
 *
 * @param cpu The CPU instance to reset
 */
void resetCpuState(Cpu* cpu) {
    // Reset the program counter to 0
    cpu->pc = 0;

    // Reset all registers to 0
    for (int i = 1; i < 32; i++) {
        writeRegister(i, 0);
    }
}

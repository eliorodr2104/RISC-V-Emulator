//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef CPU_H
#define CPU_H
#include <ncurses.h>

#include "args_handler.h"
#include "assemblyData.h"
#include "windowsManagement.h"

/**
 * @brief Struct representing the CPU state
 *
 * @var CPU::pc
 * Representing the program counter, which points to the current instruction address
 * @var CPU::resetFlag
 * Flag indicating if the CPU should reset to a specific instruction address
 */
typedef struct {
    int32_t pc;
    int32_t resetFlag;

} Cpu;

/**
 * @brief Create a new CPU instance
 *
 * @return A pointer to the newly created CPU instance, or nullptr if memory allocation fails
 */
Cpu* newCpu();

/**
 * @brief Run the CPU in full execution mode, executing all instructions until completion.
 *
 * @param windowManagement The management structure for the TUI windows
 * @param cpu The CPU instance to run
 * @param options The options for execution
 */
void runCpuFull(
    WindowsManagement windowManagement,
    Cpu* cpu,
    options_t options
);

/**
 * @brief Run the CPU in step-by-step execution mode, allowing for interactive debugging.
 *
 * @param windowManagement The management structure for the TUI windows
 * @param cpu The CPU instance to run
 * @param options The options for execution
 */
void runCpuStepByStep(
    WindowsManagement windowManagement,
    Cpu* cpu,
    options_t options
);

/**
 * @brief Execute a single step of the CPU, fetching and decoding the instruction, and updating the CPU state.
 *
 * @param windowManagement The management structure for the TUI windows
 * @param currentChar Pointer to the current character input for interactive mode
 * @param cpu The CPU instance to run
 * @param options The options for execution
 * @param data The assembly data containing instructions and metadata
 * @param interactive Flag indicating if the execution is in interactive mode
 * @return 1 if the step was executed successfully, 0 if an error occurred or execution should stop
 */
int executeSingleStep(
    WindowsManagement windowManagement,
    int* currentChar,
    Cpu* cpu,
    options_t options,
    const AssemblyData* data,
    bool interactive
);

/**
 * @brief Re-execute instructions until the target instruction is reached, resetting the CPU state.
 *
 * @param cpu The CPU instance to reset and re-execute
 * @param options The options for execution
 */
void reExecuteUntilTarget(Cpu* cpu, options_t options);

/**
 * @brief Execute an instruction silently without updating the TUI or printing output.
 *
 * @param cpu The CPU instance to execute the instruction on
 * @param options The options for execution
 */
void executeInstructionSilently(Cpu* cpu, options_t options);

/**
 * @brief Reset the CPU state to its initial values.
 *
 * @param cpu The CPU instance to reset
 */
void resetCpuState(Cpu* cpu);

#endif //CPU_H

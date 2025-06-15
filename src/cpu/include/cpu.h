//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef CPU_H
#define CPU_H
#include <ncurses.h>

#include "args_handler.h"
#include "windows_management.h"

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
    int32_t registers[32];

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
 * @brief Reset the CPU state to its initial values.
 *
 * @param cpu The CPU instance to reset
 */
void resetCpuState(Cpu* cpu);

#endif //CPU_H


/**
 * @file cpu.h
 * @brief Header file for the CPU structure and functions.
 *
 * @author eliorodr2104
 * @date 02/06/25
 *
 */

#ifndef CPU_H
#define CPU_H

#include <ncurses.h>

#include "args_handler.h"
#include "windows_management.h"

/**
 * @struct Cpu
 * @brief Representing the CPU, their state and registers.
 *
 * @param pc This is a program counter register, which points to the current instruction address
 * @param resetFlag Flag indicating if the CPU should reset to a specific instruction address
 * @param registers Array of 32 registers, each represented as a 32-bit signed integer
 */
typedef struct cpu {
    int32_t pc;
    int32_t reset_flag;
    int32_t registers[32];

} *Cpu;

/**
 * @brief Create a new CPU instance
 *
 * @return A pointer to the newly created CPU instance, or nullptr if memory allocation fails
 */
Cpu newCpu();

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
);

/**
 * @brief Run the CPU in step-by-step execution mode, allowing for interactive debugging.
 *
 * @param window_management The management structure for the TUI windows
 * @param cpu The CPU instance to run
 * @param options The options for execution
 */
void runCpuStepByStep(
    Cpu               cpu,
    options_t         options,
    WindowsManagement window_management
);

/**
 * @brief Reset the CPU state to its initial values.
 *
 * @param cpu The CPU instance to reset
 */
void resetCpuState(Cpu cpu);

#endif //CPU_H

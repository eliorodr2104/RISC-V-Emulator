
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
    uint32_t pc;
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
    WindowsManagement window_management,
    RAM               main_memory
);

/**
 * @brief Reset the CPU state to its initial values.
 *
 * @param cpu The CPU instance to reset
 */
void resetCpuState(Cpu cpu);

/**
 * @brief Get the value of a specific register by its number.
 *
 * @param cpu The CPU instance
 * @param register_number The number of the register to retrieve (0-31) in RISC-V 32 bit architecture
 */
static int32_t getValueRegister(
    const Cpu      cpu,
    const uint32_t register_number
) {
    return (register_number >= 32 ? 0 : cpu->registers[register_number]);
}

static bool writeRegister(
    const Cpu       cpu,
    const uint32_t register_number,
    const int32_t  value
) {
    if (register_number == 0) return false;

    return register_number < 32 ? (cpu->registers[register_number] = value, true) : false;
}

#endif //CPU_H

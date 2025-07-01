/**
* @file execute.h
* @brief Execute complete instruction RISC-V in separate fields.
*
* This file contains the declaration of functions to execute RISC-V instructions.
*
* @author eliorodr2104
* @date 15/06/25
*
*/

#ifndef EXECUTE_H
#define EXECUTE_H

#include "cpu.h"
#include "windows_management.h"

/**
 * @brief Execute a single step of the CPU, fetching and decoding the instruction, and updating the CPU state.
 *
 * @param window_management The management structure for the TUI windows
 * @param current_char Pointer to the current character input for interactive mode
 * @param cpu The CPU instance to run
 * @param options The options for execution
 * @param data The assembly data containing instructions and metadata
 * @param interactive Flag indicating if the execution is in interactive mode
 * @return 1 if the step was executed successfully, 0 if an error occurred or execution should stop
 */
int executeSingleStep(
    Cpu               cpu,
    options_t         options,
    const AssemblyData*     data,
    WindowsManagement window_management,
    int*              current_char,
    bool              interactive,
    RAM               main_memory

);

/**
 * @brief Re-execute instructions until the target instruction is reached, resetting the CPU state.
 *
 * @param cpu The CPU instance to reset and re-execute
 * @param options The options for execution
 */
void reExecuteUntilTarget(
    Cpu       cpu,
    options_t options,
    RAM       main_memory

);

/**
 * @brief Execute an instruction silently without updating the TUI or printing output.
 *
 * @param cpu The CPU instance to execute the instruction on
 * @param options The options for execution
 */
void executeInstructionSilently(
    Cpu       cpu,
    options_t options,
    RAM       main_memory
);

#endif //EXECUTE_H

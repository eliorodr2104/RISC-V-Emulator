/**
* @file fetch.h
* @brief Source file for the fetch module in RISC-V CPU simulation.
*
* @author eliorodr2104
* @date 15/06/25
*
*/

#include <stdint.h>

#include "cpu.h"

/**
 * @brief Fetch the instruction at the current program counter from the CPU.
 *
 * This function retrieves the instruction located at the current program counter
 * of the CPU, based on the provided options.
 *
 * @param cpu The CPU instance from which to fetch the instruction.
 * @param options The options containing instruction data and metadata.
 *
 * @return The fetched instruction as a 32-bit unsigned integer, or 0 if no valid instruction is found.
 */
uint32_t fetchInstruction(
          Cpu       cpu,
    const options_t options

) {

    if (!cpu) return 0;

    const uint32_t instructionIndex = cpu->pc / 4;

    if (instructionIndex >= options.instruction_count) return 0;

    return options.instructions[instructionIndex].instruction;
}

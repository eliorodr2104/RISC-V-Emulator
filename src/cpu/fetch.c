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
 * @param main_memory The main memory instance where instructions are stored.
 *
 * @return The fetched instruction as a 32-bit unsigned integer, or 0 if no valid instruction is found.
 */
uint32_t fetchInstruction(
          Cpu       cpu,
    const options_t options,
          RAM       main_memory

) {

    if (cpu->pc < options.text_vaddr || cpu->pc >= options.text_vaddr + options.text_size) {
        perror("Invalid program counter, outside the text section");
        return -1;
    }

    if (cpu->pc % 4 != 0) {
        perror("Program counter must be aligned to 4 bytes for RISC-V instructions");
        return -1;
    }

    return read_ram32bit(main_memory, cpu->pc);
}

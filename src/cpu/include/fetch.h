/**
* @file fetch.h
* @brief Header file for the fetch module in RISC-V CPU simulation.
*
* @author eliorodr2104
* @date 15/06/25
*
*/

#ifndef FETCH_H
#define FETCH_H

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
uint32_t fetchInstruction (
    Cpu        cpu,
    options_t  options,
    RAM        main_memory
);

#endif //FETCH_H

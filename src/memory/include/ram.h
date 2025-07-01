/**
 * @file ram.h
 * @brief Header file for RAM management in a RISC-V CPU simulator.
 *
 * This file defines the RAM structure and functions to create, free, write, and read from RAM.
 */

#ifndef RAM_H
#define RAM_H
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Header file for RAM management in a RISC-V CPU simulator.
 * This file defines the RAM structure and functions to create, free, write, and read from RAM.
 *
 * @param data Pointer to the RAM data.
 * @param size Size of the RAM in bytes.
 */
typedef struct ram {
    uint8_t *data;
    size_t size;

} *RAM;

/**
 * @brief Create a new RAM instance with the specified size.
 * @param size Size of the RAM in bytes.
 *
 * @return Pointer to the newly created RAM instance, or NULL if allocation fails.
 */
RAM new_ram(size_t size);

/**
 * @brief Free the RAM instance and its data.
 * @param ram Pointer to the RAM instance to be freed.
 */
void free_ram(RAM ram);

/**
 * @brief Write a 32-bit value to the specified address in RAM.
 * @param ram Pointer to the RAM instance.
 * @param address Address in RAM where the value will be written.
 * @param value 32-bit value to write to RAM.
 */
void write_ram32bit(RAM ram, uint32_t address, uint32_t value);

/**
 * @brief Read a 32-bit value from the specified address in RAM.
 * @param ram Pointer to the RAM instance.
 * @param address Address in RAM from which the value will be read.
 *
 * @return The 32-bit value read from RAM.
 */
uint32_t read_ram32bit(RAM ram, uint32_t address);

/**
 * @brief Load instructions into RAM from a binary data array.
 * @param ram Pointer to the RAM instance where instructions will be loaded.
 *
 * This function loads instructions into RAM, starting from address 0x0.
 */
void load_binary_to_ram(RAM ram, const uint8_t *binary, size_t size, uint32_t start_addr);

void print_ram_state(RAM ram, uint32_t start_addr, uint32_t end_addr, size_t max_lines);


#endif //RAM_H

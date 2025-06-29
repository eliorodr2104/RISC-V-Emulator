/**
 * @file ram.c
 * @brief Implementation of RAM memory management functions.
 *
 * This file contains functions to create, free, write to, and read from RAM.
 */

#include <stdint.h>
#include "ram.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief Create a new RAM instance with the specified size.
 * @param size Size of the RAM in bytes.
 *
 * @return Pointer to the newly created RAM instance, or NULL if allocation fails.
 */
RAM new_ram(size_t size) {
    if (size == 0) return nullptr;

    RAM main_memory = malloc(sizeof(struct ram));

    if (!main_memory) return nullptr;

    main_memory->data = (u_int8_t *)malloc(size);
    if (!main_memory->data) {
        free(main_memory);
        return nullptr;
    }

    main_memory->size = size;

    memset(main_memory->data, 0, size);
    return main_memory;

}

/**
 * @brief Free the RAM instance and its data.
 * @param ram Pointer to the RAM instance to be freed.
 */
void free_ram(RAM ram) {
    free(ram->data);
    ram->data = nullptr;
    ram->size = 0;

}

/**
 * @brief Write a 32-bit value to the specified address in RAM.
 * @param ram Pointer to the RAM instance.
 * @param address Address in RAM where the value will be written.
 * @param value 32-bit value to write to RAM.
 */
void write_ram32bit(
          RAM      ram,
    const uint32_t address,
    const uint32_t value
) {
    if (address + 3 >= ram->size) return; // Out of bounds check

    uint8_t *p = ram->data + address;

    p[0] = value & 0xFF;
    p[1] = value >> 8 & 0xFF;
    p[2] = value >> 16 & 0xFF;
    p[3] = value >> 24 & 0xFF;

}

/**
 * @brief Read a 32-bit value from the specified address in RAM.
 * @param ram Pointer to the RAM instance.
 * @param address Address in RAM from which the value will be read.
 *
 * @return The 32-bit value read from RAM.
 */
uint32_t read_ram32bit(
          RAM ram,
    const uint32_t address
) {

    const uint8_t *p = ram->data + address;

    return (uint32_t)p[0]
         | (uint32_t)p[1] << 8
         | (uint32_t)p[2] << 16
         | (uint32_t)p[3] << 24;

}

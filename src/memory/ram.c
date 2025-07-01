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

void destroy_ram(RAM ram) {

    if (!ram) return;

    free(ram->data);
    free(ram);
}

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

    if (!ram || !ram->data) {
        fprintf(stderr, "Errore: RAM non inizializzata\n");
        return;
    }

    if (address % 4 != 0) {
        fprintf(stderr, "Errore: indirizzo 0x%08x non allineato a 4 byte\n", address);
        return;
    }

    if (address + 3 >= ram->size) {
        fprintf(stderr, "Errore: accesso fuori bounds 0x%08x + 4 > 0x%08x\n",
                address, ram->size);
        return;
    }

    uint8_t *p = ram->data + address;

    p[0] = (uint8_t)(value & 0xFF);
    p[1] = (uint8_t)(value >> 8 & 0xFF);
    p[2] = (uint8_t)(value >> 16 & 0xFF);
    p[3] = (uint8_t)(value >> 24 & 0xFF);
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
    if (!ram || !ram->data) {
        fprintf(stderr, "Errore: RAM non inizializzata\n");
        return 0;
    }

    if (address % 4 != 0) {
        fprintf(stderr, "Errore: indirizzo 0x%08x non allineato a 4 byte\n", address);
        return 0;
    }

    if (address + 3 >= ram->size) {
        fprintf(stderr, "Errore: accesso fuori bounds 0x%08x + 4 > 0x%08x\n",
                address, ram->size);
        return 0;
    }

    const uint8_t *p = ram->data + address;

    return (uint32_t)p[0]
         | (uint32_t)p[1] << 8
         | (uint32_t)p[2] << 16
         | (uint32_t)p[3] << 24;

}

void load_binary_to_ram(RAM ram, const uint8_t *binary, size_t size, uint32_t start_addr) {
    if (!ram || !binary) {
        fprintf(stderr, "RAM o binary pointer nullo\n");
        return;
    }

    // Controllo bounds più rigoroso
    if (start_addr >= ram->size) {
        fprintf(stderr, "Indirizzo di start 0x%08x fuori dai limiti della RAM (0x%08x)\n",
                start_addr, ram->size);
        return;
    }

    if (start_addr + size > ram->size) {
        fprintf(stderr, "Dimensione %zu + indirizzo 0x%08x eccede la RAM (0x%08x)\n",
                size, start_addr, ram->size);
        size = ram->size - start_addr; // Tronca alla dimensione disponibile
    }

    // Copia byte per byte
    for (size_t i = 0; i < size; i++) {
        ram->data[start_addr + i] = binary[i];
    }

}

void print_ram_state(const RAM ram, const uint32_t start_addr, const uint32_t end_addr, const size_t max_lines) {
    if (!ram || !ram->data || start_addr >= ram->size || end_addr > ram->size || start_addr > end_addr) {
        fprintf(stderr, "Indirizzi non validi per la stampa della RAM.\n");
        return;
    }

    const uint32_t total_bytes = end_addr - start_addr;
    const uint32_t lines_needed = (total_bytes + 15) / 16; // Arrotonda per eccesso
    const uint32_t actual_lines = (lines_needed > max_lines) ? max_lines : lines_needed;

    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                         📊 STACK MEMORIA RAM                 ║\n");
    printf("║                    Range: 0x%08X → 0x%08X            ║\n", start_addr, end_addr);
    printf("╠══════════════════════════════════════════════════════════════╣\n");

    // Mostra gli indirizzi più alti prima (come un vero stack)
    uint32_t current_start = start_addr + ((actual_lines - 1) * 16);
    if (current_start >= end_addr) {
        current_start = ((end_addr - 1) / 16) * 16;
    }

    for (size_t line = 0; line < actual_lines; line++) {
        uint32_t addr = current_start - (line * 16);

        // Verifica che l'indirizzo sia valido
        if (addr < start_addr || addr >= end_addr) {
            continue;
        }

        printf("║ 0x%08X │", addr);

        // Stampa i bytes in esadecimale
        for (int i = 0; i < 16; i++) {
            if (addr + i < end_addr && addr + i >= start_addr) {
                uint8_t byte_val = ram->data[addr + i];

                // Usa colori diversi per valori diversi (se il terminale li supporta)
                if (byte_val == 0x00) {
                    printf(" \033[90m%02X\033[0m", byte_val); // Grigio per zero
                } else if (byte_val == 0xFF) {
                    printf(" \033[91m%02X\033[0m", byte_val); // Rosso per 0xFF
                } else {
                    printf(" \033[94m%02X\033[0m", byte_val); // Blu per altri valori
                }
            } else {
                printf("   "); // Spazio vuoto per indirizzi fuori range
            }
        }

        printf(" ║\n");

        // Aggiungi separatori ogni 4 righe per migliorare la leggibilità
        if ((line + 1) % 4 == 0 && line < actual_lines - 1) {
            printf("║────────────┼─────────────────────────────────────────────────║\n");
        }
    }

    // Mostra informazioni di riepilogo
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║ 📈 Righe mostrate: %3zu/%-3u", actual_lines, lines_needed);
    printf("           📏 Bytes totali: %-6u ║\n", total_bytes);

    if (actual_lines < lines_needed) {
        printf("║ ⚠️  Output limitato! Usa max_lines più grande per vedere tutto ║\n");
    }

    printf("╚══════════════════════════════════════════════════════════════╝\n");

    // Legenda
    printf("\n🎨 Legenda colori:\n");
    printf("   \033[90m■\033[0m Grigio: 0x00 (zero)    ");
    printf("\033[91m■\033[0m Rosso: 0xFF (tutti bit a 1)    ");
    printf("\033[94m■\033[0m Blu: Altri valori\n");
    printf("   · = Carattere non stampabile\n\n");
}
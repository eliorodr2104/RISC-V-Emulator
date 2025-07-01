//
// Created by C4V4H.exe on 10/06/25.
//

#include "include/asm_file_parser.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "args_handler.h"
#include "elf.h"

/**
 * @brief Check if the file has a .s extension
 * @param options_pointer options_t pointer to the options structure
 * @return true if the file has a .s extension, false otherwise
 */
int parse_riscv_file(
    options_t **options_pointer

) {
    options_t* opts = *options_pointer;

    if (!opts || !opts->binary_file) {
        fprintf(stderr, "file not found\n");
        return -1;
    }

    if (is_assembly_file(opts->binary_file)) {
        char elf_path[512];
        if (compile_assembly(opts->binary_file, elf_path) != 0) return -1;
        return load_elf_sections(elf_path, opts);
    }

    return load_elf_sections(opts->binary_file, opts);
}

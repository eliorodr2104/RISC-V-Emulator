//
// Created by C4V4H.exe on 10/06/25.
//

#ifndef ASM_FILE_PARSER_H
#define ASM_FILE_PARSER_H
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "args_handler.h"

int parse_riscv_file(options_t **opts);

/**
 * @brief returns true if the file in the given path has .s extension
 * @param filepath path of file to parse
 * @return if the file has the .s extension
 */
static bool is_assembly_file(const char *filepath) {
    const char *ext = strrchr(filepath, '.');
    return ext && strcmp(ext, ".s") == 0;
}

/**
 * @brief if the file given is a .s this compiles it
 * @param filepath path if the asm file
 * @param output_elf_path path where the output elf file will be saved
 * @return integer representing if an error occurred (0 if OK)
 */
static int compile_assembly(
    const char *filepath,
    char *output_elf_path
) {

    char cmd[512];
    char temp_obj[256];

    sprintf(temp_obj, "%s.o", filepath);
    sprintf(output_elf_path, "%s.elf", filepath);

    // Compile assembly file to object file
    sprintf(cmd, "riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 \"%s\" -o \"%s\"", filepath, temp_obj);
    if (system(cmd) != 0) {
        fprintf(stderr, "Error assembling %s\n", filepath);
        return -1;
    }

    // Link new obj file to the elf
    sprintf(cmd, "riscv64-unknown-elf-ld -m elf32lriscv \"%s\" -o \"%s\"", temp_obj, output_elf_path);
    if (system(cmd) != 0) {
        fprintf(stderr, "Error linking %s\n", temp_obj);
        unlink(temp_obj);
        return -1;
    }

    unlink(temp_obj);

    return 0;
}

#endif //ASM_FILE_PARSER_H

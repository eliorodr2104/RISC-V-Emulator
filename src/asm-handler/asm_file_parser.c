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

static bool is_assembly_file(const char *filepath);
static int read_binary_file_text_section(const char *filepath, options_t *opts);
static int compile_and_parse_assembly(const char *filepath, options_t *opts);

/**
 * @brief returns true if the file in the given path has .s extension
 * @param filepath path of file to parse
 * @return if the file has the .s extension
 */
static bool is_assembly_file(const char *filepath) {
    const char *ext = strrchr(filepath, '.');
    return ext && strcmp(ext, ".s") == 0;
}

/*/**
 * @brief initializes the instructions of the opts obj
 * @param filepath path to the asm file
 * @param opts options obj
 * @return -1: error occurred, 0: OK
 #1#
static int read_binary_file_text_section(const char *filepath, options_t *opts) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        fprintf(stderr, "Error opening the file %s\n", filepath);
        return -1;
    }

    // get file size
    fseek(file, 0, SEEK_END);
    const long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // return -1 if the file is empty
    if (file_size == 0) {
        fprintf(stderr, "Bin file %s is empty\n", filepath);
        fclose(file);
        return -1;
    }

    // printf("\n found instruction count: %ld", file_size/4);
    opts->instruction_count = (int)(file_size / 4l);

    // printf("\n allocating memory...");
    opts->text_data = malloc(opts->instruction_count * sizeof(riscv_instruction_t));
    if (!opts->instructions_machine_code) return -1;

    for (int i = 0; i < opts->instruction_count; i++) {
        uint32_t instr = 0;
        uint8_t bytes[4];

        if (fread(bytes, sizeof(uint8_t), 4, file) != 4) {
            if (feof(file)) {
                opts->instruction_count = i;
                break;
            }
            fprintf(stderr, "Error loading the instruction %d\n", i);
            fclose(file);
            return -1;
        }

        instr = (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];

        opts->instructions_machine_code[i] = instr;
        // printf("\nloaded instructions: ");
        // print_binary(instr);
    }

    // printf("\n closing file...");
    fclose(file);
    return 0;
}

/**
 * @brief initializes the instructions of the opts obj
 * @param filepath path to the asm file
 * @param opts options obj
 * @return -1: error occurred, 0: OK
 #1#
static int read_binary_file_data_section(const char *filepath, options_t *opts) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        fprintf(stderr, "Error opening the file %s\n", filepath);
        return -1;
    }

    // get file size
    fseek(file, 0, SEEK_END);
    const long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // return -1 if the file is empty
    if (file_size == 0) {
        fprintf(stderr, "Bin file %s is empty\n", filepath);
        fclose(file);
        return -1;
    }

    // printf("\n found instruction count: %ld", file_size/4);
    opts->data_count = (int)(file_size / 4l);

    // printf("\n allocating memory...");
    opts->data_machine_code = malloc(opts->data_count * sizeof(riscv_instruction_t));
    if (!opts->data_machine_code) return -1;

    for (int i = 0; i < opts->data_count; i++) {
        uint32_t instr = 0;
        uint8_t bytes[4];

        if (fread(bytes, sizeof(uint8_t), 4, file) != 4) {
            if (feof(file)) {
                opts->data_count = i;
                break;
            }
            fprintf(stderr, "Error loading the instruction %d\n", i);
            fclose(file);
            return -1;
        }

        instr = (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];

        opts->data_machine_code[i] = instr;
        // printf("\nloaded instructions: ");
        // print_binary(instr);
    }

    // printf("\n closing file...");
    fclose(file);
    return 0;
}*/

/**
 * @brief if the file given is a .s this compiles it
 * @param filepath path if the asm file
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

// Funzione principale per parsare un file RISC-V
int parse_riscv_file(
    options_t **opts_ptr
) {
    options_t* opts = *opts_ptr;

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

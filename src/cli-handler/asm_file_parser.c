//
// Created by C4V4H.exe on 10/06/25.
//

#include "asm_file_parser.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "args_handler.h"

static bool is_assembly_file(const char *filepath);
static int read_binary_file(const char *filepath, options_t *opts);
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

/**
 * @brief initializes the instructions of the opts obj
 * @param filepath path to the asm file
 * @param opts options obj
 * @return -1: error occurred, 0: OK
 */
static int read_binary_file(const char *filepath, options_t *opts) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        fprintf(stderr, "Error opening the file %s\n", filepath);
        return -1;
    }

    // get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // return -1 if the file is empty
    if (file_size == 0) {
        fprintf(stderr, "Bin file %s is empty\n", filepath);
        fclose(file);
        return -1;
    }

    opts->instruction_count = file_size / 4;

    opts->instructions = malloc(opts->instruction_count * sizeof(uint32_t));
    if (!opts->instructions) return -1;

    // for simple binaries assume that the first addres is 0x00
    opts->text_start = 0;
    opts->text_size = file_size;
    opts->entry_point = 0;

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

        opts->instructions[i].address = opts->text_start + i * 4;
        opts->instructions[i].instruction = instr;
        printf("%d", instr);
    }

    fclose(file);
    return 0;
}

/**
 * @brief if the file given is a .s this compiles it
 * @param filepath path if the asm file
 * @param opts option obj
 * @return integer representing if an error occurred (0 if OK)
 */
static int compile_and_parse_assembly(const char *filepath, options_t *opts) {
    char cmd[512];
    char temp_obj[256];
    char temp_bin[256];
    int ret;

    // tmp files
    sprintf(temp_obj, "%s.o", filepath);
    sprintf(temp_bin, "%s.bin", filepath);


    sprintf(cmd, "riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 %s -o %s",
            filepath, temp_obj);
    printf("Runnning: %s\n", cmd);
    ret = system(cmd);
    if (ret != 0) {
        fprintf(stderr, "Error assembling %s\n", filepath);
        return -1;
    }

    // Link new obj file to the elf
    sprintf(cmd, "riscv64-unknown-elf-ld -m elf32lriscv %s -o %s.elf",
            temp_obj, filepath);
    printf("Running: %s\n", cmd);
    ret = system(cmd);
    if (ret != 0) {
        fprintf(stderr, "Error linking %s\n", temp_obj);
        unlink(temp_obj);
        return -1;
    }

    sprintf(cmd, "riscv64-unknown-elf-objcopy -O binary -j .text %s.elf %s",
            filepath, temp_bin);
    printf("Running: %s\n", cmd);
    ret = system(cmd);
    if (ret != 0) {
        fprintf(stderr, "Error extracting binary from %s.elf\n", filepath);
        unlink(temp_obj);
        unlink(filepath);
        return -1;
    }

    // load in instructions
    ret = read_binary_file(temp_bin, opts);

    // parse the elf file
    sprintf(cmd, "riscv64-unknown-elf-readelf -h %s.elf | grep \"Entry point\"", filepath);
    FILE *fp = popen(cmd, "r");
    if (fp) {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), fp)) {
            const char *entry_str = strstr(buffer, "0x");
            if (entry_str) {
                opts->entry_point = strtoul(entry_str, nullptr, 16);
            }
        }
        pclose(fp);
    }

    sprintf(cmd, "riscv64-unknown-elf-readelf -S %s.elf | grep \".text\"", filepath);
    fp = popen(cmd, "r");
    if (fp) {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), fp)) {
            // Estrai l'indirizzo di inizio e dimensione della sezione .text
            char *addr_str = strstr(buffer, " ");
            if (addr_str) {
                // Salta i campi non necessari per trovare l'indirizzo e la dimensione
                int field_count = 0;
                char *token = strtok(buffer, " \t");
                while (token && field_count < 5) {
                    token = strtok(nullptr, " \t");
                    field_count++;
                }
                // Il token corrente è l'indirizzo della sezione .text
                if (token) {
                    opts->text_start = strtoul(token, nullptr, 16);
                    // Il prossimo token è la dimensione
                    token = strtok(nullptr, " \t");
                    if (token) {
                        opts->text_size = strtoul(token, nullptr, 16);
                    }
                }
            }
        }
        pclose(fp);
    }

    for (int i = 0; i < opts->instruction_count; i++) {
        opts->instructions[i].address = opts->text_start + (i * 4);
    }

    // Pulisci i file temporanei
    unlink(temp_obj);
    unlink(temp_bin);
    sprintf(cmd, "%s.elf", filepath);
    unlink(cmd);

    return ret;
}

// Funzione principale per parsare un file RISC-V
int parse_riscv_file(const options_t *opts) {
    const auto mutable_opts = (options_t*)opts; // Necessario per modificare la struct

    if (!opts || !opts->binary_file) {
        fprintf(stderr, "Parametri non validi\n");
        return -1;
    }

    // // Verifica che l'array instructions sia già allocato
    // if (!opts->instructions) {
    //     fprintf(stderr, "L'array instructions non è allocato\n");
    //     return -1;
    // }

    if (is_assembly_file(opts->binary_file)) {
        // File assembly, dobbiamo prima compilarlo
        return compile_and_parse_assembly(opts->binary_file, mutable_opts);
    }
    // File binario, parsiamolo direttamente
    return read_binary_file(opts->binary_file, mutable_opts);
}

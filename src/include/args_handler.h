//
// Created by C4V4H.exe on 10/06/25.
//

#ifndef ARGS_HANDLER_H
#define ARGS_HANDLER_H

#include<stdint.h>


typedef struct {
    uint32_t address;       // address of the instruction
    uint32_t instruction;   // machine code of the instruction (32-bit)
} riscv_instruction_t;

// struct with the options for the binary
typedef struct {
    char *binary_file;      // path to asm riscv 32bit binary file
    char **args;            // args for the asm (after the -a flag)
    int args_count;         // number of args for the asm
    bool full_execution;    // bool flag for full execution         (incompatible with -s)
    bool step_mode;         // bool flag for step by step execution (incompatible with -f)
    char **breakpoints;     // breakpoints for the asm program
    int breakpoint_count;

    // data of the bin given
    riscv_instruction_t *instructions;   // Array of the instructions
    int instruction_count;               // instruction count
    uint32_t entry_point;                // address of the entry point
    uint32_t text_start;                 // address of the '.text' section
    uint32_t text_size;                  // size    of the '.text' section
} options_t;

// public functions
int handle_args(int argc, char *argv[], options_t *opts);
void print_options(const options_t *opts);
void free_options(const options_t *opts);

#endif //ARGS_HANDLER_H

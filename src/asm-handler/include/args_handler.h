//
// Created by C4V4H.exe on 10/06/25.
//

#ifndef ARGS_HANDLER_H
#define ARGS_HANDLER_H

#include<stdint.h>

#include "ram.h"

typedef enum {
    DEFAULT, FULL, STEP_BY_STEP
} execution_mode;

typedef struct {
    uint32_t address;       // address of the instruction
    uint32_t instruction;   // machine code of the instruction (32-bit)
} riscv_instruction_t;

// struct with the options for the binary
typedef struct {
    char *binary_file;                // path to asm riscv 32bit binary file
    // @TODO: implement args
    char **args;                      // args for the asm (after the -a flag)
    int args_count;                   // number of args for the asm
    execution_mode execution_mode;    // execution mode
    // @TODO: implement breakpoints
    char **breakpoints;               // breakpoints for the asm program
    int breakpoint_count;

    // Add options for load in memory the instructions
    // Instruction (.text)
    uint8_t* text_data;      // binary buffer.text
    size_t text_size;         // byte
    uint32_t text_vaddr;      // virtual address .text

    // Data (.data)
    uint8_t* data_data;      // binary buffer .data
    size_t data_size;         // byte
    uint32_t data_vaddr;      // virtual address .data

    // Entry point
    uint32_t entry_point;

} options_t;

// public functions
int  handle_args  (
    int argc,
    char *argv[],
    options_t *opts
);
void print_options(const options_t *opts);
void free_options (const options_t *opts);

#endif //ARGS_HANDLER_H

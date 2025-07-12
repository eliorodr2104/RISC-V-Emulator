//
// Created by Eliomar Alejandro Rodriguez Ferrer on 03/06/25.
//

#include <inttypes.h>
#include <stdio.h>

#include "alu_control.h"
#include "tui_cpu.h"
#include "tools.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>


void print_binary(const uint32_t bin) {
    printf("\n");
    for (int i = 31; i >= 0; --i) {
        printf("%"PRIu32, bin >> i & 1);
    }
}

const char* getInstructionName(const uint8_t opcode, const uint8_t funct3, const uint8_t funct7Bit30) {
    switch(opcode) {
        case 0x13: // Tipo I - Aritmetica immediata
            switch(funct3) {
                case 0x0: return "addi";
                case 0x1: return "slli";
                case 0x5: return funct7Bit30 ? "srai" : "srli";
                case 0x4: return "xori";
                case 0x6: return "ori ";
                case 0x7: return "andi";
                default: return "I-TYPE";
            }
        case 0x33: // Tipo R - Aritmetica registro
            switch(funct3) {
                case 0x0: return funct7Bit30 ? "sub " : "add ";
                case 0x1: return "sll ";
                case 0x5: return funct7Bit30 ? "sra " : "srl ";
                case 0x4: return "xor ";
                case 0x6: return "or  ";
                case 0x7: return "and ";
                default: return "R-TYPE";
            }

        case 0x67: return "jalr";
        case 0x6F: return "jal ";
        case 0x63: return "BRANCH";
        case 0x03: return "LOAD";
        case 0x23: return "STORE";
        case 0x37: return "LUI";
        default: return "UNKNOWN";
    }
}

AluOp getInstructionEnum(const uint8_t opcode, const uint8_t funct3, const uint8_t funct7Bit30) {
    switch(opcode) {
        case 0x13: // Tipo I - Aritmetica immediata
            switch(funct3) {
                case 0x0: return ALU_ADDI;
                case 0x1: return ALU_SLLI;
                case 0x5: return funct7Bit30 ? ALU_SRAI : ALU_SRLI;
                case 0x4: return ALU_XORI;
                case 0x6: return ALU_ORI;
                case 0x7: return ALU_ANDI;

                default:  return ALU_UNKNOWN;
            }

        case 0x03:
            switch (funct3) {
                case 0x0: return ALU_LB;
                case 0x1: return ALU_LH;
                case 0x2: return ALU_LW;
                case 0x4: return ALU_LBU;
                case 0x5: return ALU_LHU;

                default:  return ALU_UNKNOWN;
            }

        case 0x33: // Tipo R - Aritmetica registro
            switch(funct3) {
                case 0x0: return funct7Bit30 ? ALU_SUB : ALU_ADD;
                case 0x1: return ALU_SLL;
                case 0x5: return funct7Bit30 ? ALU_SRA : ALU_SRL;
                case 0x4: return ALU_XOR;
                case 0x6: return ALU_OR;
                case 0x7: return ALU_AND;
                default: return ALU_UNKNOWN;
            }

        case 0x37:
            return LUI;

        case 0x17:
            return ALU_AUIPC;

        default: return ALU_UNKNOWN;
    }
}

void formatInstruction(const DecodedInstruction decoded, char* buffer, uint32_t size) {
    const char* name = getInstructionName(decoded.opcode, decoded.funz3, decoded.funz7_bit30);

    if (decoded.opcode == 0x33) { // Tipe R
        snprintf(buffer, size, "%s %-4s, %-4s, %s",
                name, register_names[decoded.rd], register_names[decoded.rs1], register_names[decoded.rs2]);

    }
    else if (decoded.opcode == 0x67 || decoded.opcode == 0x13) { // Tipe I || JALR
        snprintf(buffer, size, "%s %-4s, %-4s, %d",
                name, register_names[decoded.rd], register_names[decoded.rs1], decoded.immediate);

    }
    else {
        snprintf(buffer, size, "%s (0x%02X)", name, decoded.opcode);
    }
}

char **tokenize(const char *src, int *n_tokens) {
    const char *p = src;
    char **tokens = nullptr;
    int count = 0;

    while (*p) {

        while (*p && isspace((unsigned char)*p)) p++;

        if (!*p) break;

        if (*p == ',') {
            tokens = realloc(tokens, sizeof(*tokens) * (count + 2));
            tokens[count++] = strdup(",");
            p++;
            continue;
        }

        const char *start = p;
        while (*p && !isspace((unsigned char)*p) && *p != ',') p++;
        size_t len = p - start;

        tokens = realloc(tokens, sizeof(*tokens) * (count + 2));
        tokens[count] = malloc(len + 1);
        memcpy(tokens[count], start, len);
        tokens[count][len] = '\0';
        count++;
    }

    // Termina con NULL per comodità
    tokens = realloc(tokens, sizeof(*tokens) * (count + 1));
    tokens[count] = NULL;
    *n_tokens = count;
    return tokens;
}

void free_tokens(char** tokens) {
    if (!tokens) return;

    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }

    free(tokens);
}
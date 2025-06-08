//
// Created by Eliomar Alejandro Rodriguez Ferrer on 03/06/25.
//

#include <stdio.h>

#include "aluControl.h"
#include "instructionMemory.h"

const char* getInstructionName(const uint8_t opcode, const uint8_t funct3, const uint8_t funct7Bit30) {
    switch(opcode) {
        case 0x13: // Tipo I - Aritmetica immediata
            switch(funct3) {
                case 0x0: return "ADDI";
                case 0x1: return "SLLI";
                case 0x5: return funct7Bit30 ? "SRAI" : "SRLI";
                case 0x4: return "XORI";
                case 0x6: return "ORI";
                case 0x7: return "ANDI";
                default: return "I-TYPE";
            }
        case 0x33: // Tipo R - Aritmetica registro
            switch(funct3) {
                case 0x0: return funct7Bit30 ? "SUB" : "ADD";
                case 0x1: return "SLL";
                case 0x5: return funct7Bit30 ? "SRA" : "SRL";
                case 0x4: return "XOR";
                case 0x6: return "OR";
                case 0x7: return "AND";
                default: return "R-TYPE";
            }

        case 0x67: return "JALR";
        case 0x6F: return "JAL";
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
            default: return ALU_UNKNOWN;
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

        default: return ALU_UNKNOWN;
    }
}

void formatInstruction(const DecodedInstruction decoded, char* buffer, size_t size) {
    const char* name = getInstructionName(decoded.opcode, decoded.funct3, decoded.funct7Bit30);

    if (decoded.opcode == 0x13) { // Tipo I
        snprintf(buffer, size, "%s x%d, x%d, %d",
                name, decoded.rd, decoded.rs1, decoded.immediate);
    }
    else if (decoded.opcode == 0x33) { // Tipo R
        snprintf(buffer, size, "%s x%d, x%d, x%d",
                name, decoded.rd, decoded.rs1, decoded.rs2);
    }
    else if (decoded.opcode == 0x67) { // JALR
        snprintf(buffer, size, "%s x%d, x%d, %d",
                name, decoded.rd, decoded.rs1, decoded.immediate);
    }
    else {
        snprintf(buffer, size, "%s (0x%02X)", name, decoded.opcode);
    }
}
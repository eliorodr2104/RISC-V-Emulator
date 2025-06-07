//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include "aluControl.h"
#include <stdint.h>

AluOp getAluControl(const uint8_t aluOp, const uint8_t funct3, const uint8_t funct7Bit30) {

    switch (aluOp) {

        // Tipe-R
        case 0x33:

            switch (funct3) {
                case 0x0: return funct7Bit30 ? ALU_SUB : ALU_ADD;
                case 0x1: return ALU_SLL;
                case 0x2: return ALU_SLT;
                case 0x4: return ALU_XOR;
                case 0x5: return funct7Bit30 ? ALU_SRA : ALU_SRL;
                case 0x6: return ALU_OR;
                case 0x7: return ALU_AND;

                default:  return ALU_UNKNOWN;
            }

        // Tipe-I
        // Type-I -> jalr
        case 0x67:
        case 0x13:

            switch (funct3) {
                case 0x0: return ALU_ADD;                          // addi or jalr
                case 0x1: return ALU_SLL;                          // slli
                case 0x4: return ALU_XOR;                          // xori
                case 0x5: return funct7Bit30 ? ALU_SRA : ALU_SRL;  // srai or srli
                case 0x6: return ALU_OR;                           // ori
                case 0x7: return ALU_AND;                          // andi

                default: return ALU_UNKNOWN;

            }

        default:
            return ALU_UNKNOWN;
    }
}

void getAluOperationBits(
    const AluOp aluOpEnum,
    uint8_t* operation
) {

    switch (aluOpEnum) {

        case ALU_ADD:
            *operation = 0b00010;
        break;

        case ALU_SUB:
            *operation = 0b01010;
        break;

        case ALU_AND:
            *operation = 0b00000;
        break;

        case ALU_OR:
            *operation = 0b00001;
        break;

        case ALU_SLT:
            *operation = 0b01011;
        break;

        case ALU_SLL:
            *operation = 0b00100;
        break;

        case ALU_SRL:
            *operation = 0b00101;
        break;

        case ALU_SRA:
            *operation = 0b00110;
        break;

        case ALU_XOR:
            *operation = 0b00111;
        break;

        default:
            *operation = 0b00000;
        break;
    }
}

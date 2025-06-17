/**
 * @file decode.c
 * @brief Decode complete instruction RISC-V in separate fields.
 *
 * @author eliorodr2104
 * @date 15/06/25
 *
 */

#include <stdint.h>

#include "tools.h"
#include "decode.h"

/**
 * @brief Decode a RISC-V instruction from a 32-bit integer.
 *
 * This function takes a 32-bit instruction and decodes it into its components,
 * including opcode, registers, immediate value, funct3, and funct7 bit 30.
 *
 * @param instruction The 32-bit instruction to decode.
 *
 * @return DecodedInstruction A structure containing the decoded instruction components.
 */
DecodedInstruction decodeInstruction(const uint32_t instruction) {
    DecodedInstruction decoded = { 0 };

    decoded.opcode      = extractBits(instruction, 0, 6);    // Set opcode to [6-0] bit
    decoded.rd          = extractBits(instruction, 7, 11);   // Set register destination to [11-7] bit
    decoded.funz3       = extractBits(instruction, 12, 14);  // Set funz3 to [14-12] bit
    decoded.rs1         = extractBits(instruction, 15, 19);  // Set register source one to [19-15] bit
    decoded.rs2         = extractBits(instruction, 20, 24);  // Set register source two [24-20] bit
    decoded.funz7_bit30 = extractBits(instruction, 30, 30);  // Set funz7 to [30] bit

    // Decode instruction based on the opcode (instruction type) and set the immediate value accordingly
    switch (decoded.opcode) {

        // Tipe-I (example: ADDI, LW, JALR)
        // Set the immediate value for I-type instructions
        case 0x67:
        case 0x13:
        case 0x03:
            decoded.immediate = signExtend(extractBits(instruction, 20, 31), 12);

            break;

        // Tipe-S (example: SW, SB, SH)
        // Set the immediate value for S-type instructions, first unify the bits.
        case 0x23:
            const uint32_t imm_11_5 = extractBits(instruction, 25, 31);
            const uint32_t imm_4_0  = extractBits(instruction, 7, 11);
            const uint32_t imm_s    = (imm_11_5 << 5) | imm_4_0;
            decoded.immediate       = signExtend(imm_s, 12);

            break;

        // Tipe-U
        // Set the immediate value for U-type instructions (LUI, AUIPC); this instruction has an immediate segment in 16 bits.
        case 0x37:
            decoded.immediate = (int32_t)(instruction & 0xFFFFF000);
            break;

        // Tipe-J
        // Set the immediate value for J-type instructions (JAL), this instruction has an immediate segment.
        case 0x6F:
            {
                const uint32_t imm_20    = extractBits(instruction, 31, 31);
                const uint32_t imm_19_12 = extractBits(instruction, 12, 19);
                const uint32_t imm_11    = extractBits(instruction, 20, 20);
                const uint32_t imm_10_1  = extractBits(instruction, 21, 30);
                const uint32_t imm_j     = (imm_20 << 20) | (imm_19_12 << 12) | (imm_11 << 11) | (imm_10_1 << 1);
                      decoded.immediate  = signExtend(imm_j, 21);
            }
            break;

        // Tipe-R
        // For R-type instructions, the immediate value is not used, so set it to 0.
        case 0x33:
        default:
            decoded.immediate = 0;
            break;
    }

    return decoded;
}

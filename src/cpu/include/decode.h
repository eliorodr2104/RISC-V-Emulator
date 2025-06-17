/**
 * @file decode.h
 * @brief Decode complete instruction RISC-V in separate fields.
 *
 * @author eliorodr2104
 * @date 15/06/25
 *
 */

#ifndef DECODE_H
#define DECODE_H

#include <stdint.h>

/**
 * @struct DecodedInstruction
 *
 * @brief Structure representing a decoded RISC-V instruction.
 *
 * @param opcode      - This is operation code, determinate the type of instruction (6-0)
 * @param rs1         - Register source one, this represents the number register (19-15)
 * @param rs2         - Register source two, this represents the number register (24-20)
 * @param rd          - Register destination, use to write the result operation (11-7)
 * @param immediate   - Immediate number, for operation when not use rs2 (31-0)
 * @param funct3      - Segment to determinate the operation ALU  (14-12)
 * @param funct7Bit30 - Specific ALU operation (30)
 *
 */
typedef struct {
    uint8_t  opcode;
    uint8_t  rs1;
    uint8_t  rs2;
    uint8_t  rd;
    int32_t  immediate;
    uint8_t  funz3;
    uint8_t  funz7_bit30;
} DecodedInstruction;

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
DecodedInstruction decodeInstruction(uint32_t instruction);

#endif //DECODE_H

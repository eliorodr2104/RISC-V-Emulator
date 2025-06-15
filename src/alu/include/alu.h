/**
 * @file alu.h
 * @brief Header file for the ALU operations
 *
 * @author eliorodr2104
 * @date 02/06/25
 *
 * This file contains the definitions and function prototypes for performing
 * arithmetic and logical operations in a 1-bit and 32-bit ALU.
 */

#ifndef ALU_H
#define ALU_H
#include <stdint.h>

/**
 * @brief Struct to hold the result of a 1-bit ALU operation
 *
 * @var AluResult::result
 * The result of the ALU operation
 * @var AluResult::carry_out
 * The carry-out flag
 */
typedef struct {
    bool result;
    bool carry_out;

} AluResult;

/**
 * @brief Struct to hold the result of a 32-bit ALU operation
 *
 * @var Alu32BitResult::result
 * The result of the ALU operation
 * @var Alu32BitResult::zero
 * The zero flag, set if the result is zero
 * @var Alu32BitResult::overflow
 * The overflow flag, set if there was an overflow in the operation
 */
typedef struct {
    int32_t result;
    bool zero;
    bool overflow;

} Alu32BitResult;

/**
 * @brief Perform a 1-bit ALU operation
 *
 * @param a The first bit
 * @param b The second bit
 * @param less The less than bit (used for set less than operation), this is 0 for all bits except the last one
 * @param carry_in The carry in bit for addition/subtraction
 * @param invert_control Control bits to determine if inputs should be inverted
 * @param alu_operation The ALU operation to perform
 * @return AluResult containing the result and all flags
 */
AluResult alu_1_bit (
    bool    a,
    bool    b,
    bool    less,
    bool    carry_in,
    uint8_t invert_control,
    uint8_t alu_operation
);

/**
 * @brief Perform a 32-bit ALU operation
 *
 * @param a The first 32-bit integer
 * @param b The second 32-bit integer
 * @param less The less than bit (used for set less than operation), this is 0 for all bits except the last one
 * @param operation The ALU operation to perform
 * @return Alu32BitResult containing the result, zero flag, and overflow flag
 */
Alu32BitResult alu_32_bit(
    int32_t a,
    int32_t b,
    bool less,
    uint8_t operation
);

/**
 * @brief Get the number of bits to shift based on the input value
 *
 * @param b The input value (should be in the range 0-31)
 * @return The number of bits to shift (0-31)
 */
static uint32_t bits_to_shift_amount(const int32_t b) {
    return b & 0x1F;
}

#endif

/**
 * @file alu.h
 * @brief Source file for the ALU operations
 *
 * @author eliorodr2104
 * @date 02/06/25
 *
 * This file contains the function's arithmetic and logical operations in a 1-bit and 32-bit ALU.
 */

#include "alu.h"
#include "tools.h"

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
AluResult alu_1_bit(
          bool    a,
          bool    b,
    const bool    less,
    const bool    carry_in,
    const uint8_t invert_control,
    const uint8_t alu_operation
) {
    // Get the bits, invert if necessary
    const bool b_invert = getBit(invert_control, 3);
    const bool a_invert = getBit(invert_control, 4);

    // Declared struct for result
    AluResult result = {};

    // Control the ALU operation
    switch (alu_operation) {

        // Operation
        // 00000 -> AND
        // 11000 -> NOR
        case 0:

            if (a_invert) a = a ^ 1;
            if (b_invert) b = b ^ 1;

            result.result   = a & b;
            result.carry_out = false;

            break;

        // Operation
        // 00001 -> OR
        case 1:

            result.result   = a | b;
            result.carry_out = false;

            break;

        // Operation
        // 00010 -> ADD
        // 01010 -> SUB
        case 2:

            if (a_invert) a = a ^ 1;
            if (b_invert) b = b ^ 1;

            result.result   = a ^ b ^ carry_in;
            result.carry_out = (a & b) | (carry_in & (a ^ b));

            break;
        // Operation
        // 00011 -> Set less than
        case 3:
            result.result   = less;
            result.carry_out = false;

            break;

        // Operation
        // 00100 -> XOR
        case 7:

            result.result   = a ^ b;
            result.carry_out = false;

            break;

        // Default case for unrecognized operations
        default:
            result.result = false;
            result.carry_out = false;

            break;

    }

    return result;
}

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
    const int32_t a,
    const int32_t b,
    const bool less,
    const uint8_t operation
) {

    // Initialize the result structure
    Alu32BitResult alu32_bit_result = { 0, 0, 0};

    // If the operation is 0xE (Unrecognize operation), return the result immediately
    if (operation == 0xE) return alu32_bit_result;

    // Extract the carry in bit and the ALU operation from the operation code
    bool carryIn = getBit(operation, 3);

    // Extract the ALU operation bits (bits 2, 3, and 4)
    const uint32_t alu_operation = operation & 0b111;

    // Shift Left Arithmetic
    if (alu_operation == 4) {
        alu32_bit_result.result = a << bits_to_shift_amount(b);

        // Jump to CONTROL_ZERO to set zero and overflow flags
        goto CONTROL_ZERO;
    }

    // Shift Right Logical
    if (alu_operation == 5) {
        alu32_bit_result.result = a >> bits_to_shift_amount(b);

        // Jump to CONTROL_ZERO to set zero and overflow flags
        goto CONTROL_ZERO;
    }

    // Shift Right Arithmetic
    if (alu_operation == 6) {
        const uint32_t shiftAmount = bits_to_shift_amount(b);

        alu32_bit_result.result = a >> shiftAmount;

        // Jump to CONTROL_ZERO to set zero and overflow flags
        goto CONTROL_ZERO;
    }

    // Declared variables for the result bits, carry in and carry out
    int32_t result_bits = 0;
    bool carry_in_overflow_control  = 0;
    bool carry_out_overflow_control = 0;

    // For loop through each bit of the 32-bit integers and pass them to the 1-bit ALU operation
    for (int i = 0; i < 32; i++) {

        // Get the bits from the integers a and b, and the less than bit if it's the last iteration
        const bool bit_a      = getBit(a, i);
        const bool bit_b      = getBit(b, i);
        const bool less_input = i == 31 ? less : false;

        // Get the ALU operation for this bit
        const AluResult res  = alu_1_bit(bit_a, bit_b, less_input, carryIn, operation, alu_operation);

        // Set the result bit in the result_bits variable
        setBit(&result_bits, i, res.result);

        // Update the carry in for the next iteration
        carryIn = res.carry_out;

        // Set the carry in and carry out overflow control bits
        if (i == 1) carry_in_overflow_control  = carryIn;
        if (i == 0) carry_out_overflow_control = res.carry_out;
    }

    // Set the result in the alu32_bit_result structure
    alu32_bit_result.result = result_bits;

// This label is used to set the zero and overflow flags
CONTROL_ZERO:

    // Set the zero flag if the result is zero
    alu32_bit_result.zero     = alu32_bit_result.result == 0;

    // Set the overflow flag based on the ALU operation
    alu32_bit_result.overflow = (alu_operation >= 4 && alu_operation <= 6 ? false : carry_in_overflow_control ^ carry_out_overflow_control);

    return alu32_bit_result;
}
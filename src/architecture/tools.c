//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include <stdlib.h>

int32_t bitsToInt(const uint8_t size, const bool bits[size]) {
    int32_t result = 0;
    for (int i = 0; i < size; i++) {
        result <<= 1;
        if (bits[i]) {
            result |= 1;
        }
    }
    return result;
}

void intToBits(int32_t value, bool bits[32]) {
    for (int i = 31; i >= 0; i--) {
        bits[i] = (value & 1);
        value >>= 1;
    }
}

bool getBit(const uint32_t x, const int i) {
    return (x >> i) & 1u;
}

void setBit(uint32_t* x, const int i, const bool val) {
    if (val)
        *x |= (1u << i);
    else
        *x &= ~(1u << i);
}
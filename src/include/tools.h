//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef TOOLS_H
#define TOOLS_H

static bool getBit(const uint32_t x, const int i) {
    return (x >> i) & 1u;
}

static void setBit(uint32_t* x, const int i, const bool val) {
    *x = (val ? *x | (1u << i) : *x & ~(1u << i)) ;
}

static uint32_t extractBits(const uint32_t instruction, const int start, const int end) {
    return instruction >> start & ((1U << (end - start + 1)) - 1);
}

// Estensione del segno per immediati
static int32_t signExtend(const uint32_t value, const int bits) {
    const int shift = 32 - bits;
    return (int32_t)(value << shift) >> shift;
}

#endif //TOOLS_H

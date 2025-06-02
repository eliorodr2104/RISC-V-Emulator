//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef TOOLS_H
#define TOOLS_H

int32_t bitsToInt(const uint8_t size, const bool bits[32]);
void intToBits(int32_t value, bool bits[32]);

bool getBit(uint32_t x, int i);
void setBit(uint32_t* x, int i, bool val);

#endif //TOOLS_H

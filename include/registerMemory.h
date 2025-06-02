//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef REGISTERMEMORY_H
#define REGISTERMEMORY_H
#include <stdint.h>

extern int32_t registers[32];

bool* getValueRegister(uint32_t registerNumber);
bool writeRegister(uint32_t registerNumber, uint32_t value);

#endif //REGISTERMEMORY_H

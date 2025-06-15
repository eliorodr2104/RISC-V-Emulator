//
// Created by Eliomar Alejandro Rodriguez Ferrer on 15/06/25.
//

#ifndef FETCH_H
#define FETCH_H

#include <stdint.h>

#include "cpu.h"

uint32_t fetchInstruction (
    Cpu*       cpu,
    options_t  options
);

#endif //FETCH_H

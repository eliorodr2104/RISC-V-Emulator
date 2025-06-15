//
// Created by Eliomar Alejandro Rodriguez Ferrer on 15/06/25.
//

#include <stdint.h>

#include "cpu.h"

uint32_t fetchInstruction(
          Cpu* cpu,
    const options_t options

) {

    if (!cpu) return 0;

    const uint32_t instructionIndex = cpu->pc / 4;

    if (instructionIndex >= options.instruction_count) return 0;

    return options.instructions[instructionIndex].instruction;
}

//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include "cpu.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alu.h"
#include "aluControl.h"
#include "controlUnit.h"
#include "instructionMemory.h"
#include "registerMemory.h"
#include "../include/tui/cpu/tuiCpu.h"

Cpu* newCpu() {
    Cpu* cpu = malloc(sizeof(Cpu));

    if (!cpu) return nullptr;
    cpu->pc = 0;

    return cpu;
}

void runCpuFull(
    WINDOW* winProg,
    WINDOW* winRegs,
    Cpu* cpu
) {

    while (cpu->pc < SIZE_INSTRUCTIONS) {
        executeSingleStep(winProg, winRegs, winRegs, cpu, 0);
    }

}

void runCpuStepByStep(
    WINDOW* winProg,
    WINDOW* winRegs,
    WINDOW* winStatus,
    Cpu* cpu
) {
    while (cpu->pc < SIZE_INSTRUCTIONS) {
        if (!executeSingleStep(winProg, winRegs, winStatus, cpu, 1)) break;
    }

}

int executeSingleStep(
    WINDOW* winProg,
    WINDOW* winRegs,
    WINDOW* winStatus,
    Cpu* cpu,
    const bool interactive
) {
    int32_t nextPc    = cpu->pc + 4;
    uint8_t operation = 0;

    const DecodedInstruction decodedInstruction = decodeInstruction(fetchInstruction(cpu));
    const ControlSignals unitControlRet         = getControlSignals(decodedInstruction.opcode);
    const AluOp aluOpEnum                       = getAluControl(
                                                        unitControlRet.operation,
                                                        decodedInstruction.funct3,
                                                        decodedInstruction.funct7Bit30
                                                    );

    getAluOperationBits(aluOpEnum, &operation);

    const int32_t firstRegisterValue = getValueRegister(decodedInstruction.rs1);

    const int32_t secondOperand = unitControlRet.aluSrc ? decodedInstruction.immediate : getValueRegister(decodedInstruction.rs2);

    const Alu32BitResult result = alu32bit(firstRegisterValue, secondOperand, 0, operation);

    if (printProgramWithCurrentInstruction(winProg, winRegs, winStatus, firstRegisterValue, secondOperand, result.result, cpu->pc)) {
        return 0;
    }

    // Esecuzione per JALR
    if (decodedInstruction.opcode == 0x67 && decodedInstruction.funct3 == 0x0) {
        if (unitControlRet.regWrite) {
            writeRegister(decodedInstruction.rd, cpu->pc + 4);
        }

        const int32_t base = getValueRegister(decodedInstruction.rs1);
        const int32_t offset = decodedInstruction.immediate;

        nextPc = (base + offset) & ~1;

    } else if (unitControlRet.regWrite) {
        writeRegister(decodedInstruction.rd, result.result);

    }

    cpu->pc = nextPc;

    //if (interactive) {
    //    const int ch = getch();
    //    if (ch == 'q' || ch == 'Q') return 0;

    //}

    return 1;
}

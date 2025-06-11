//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include "cpu.h"

#include <stdlib.h>

#include "alu.h"
#include "aluControl.h"
#include "assemblyData.h"
#include "controlUnit.h"
#include "instructionMemory.h"
#include "registerMemory.h"
#include "../include/tui/cpu/tuiCpu.h"
#include "windows.h"

Cpu* newCpu() {
    Cpu* cpu = malloc(sizeof(Cpu));

    if (!cpu) return nullptr;
    cpu->pc = 0;
    cpu->resetFlag = -1;

    return cpu;
}

void runCpuFull(
    WINDOW* winProg,
    WINDOW* winRegs,
    WINDOW*  winStatus,
    WINDOW*  winCmd,
    Windows* window,
    Cpu* cpu,
    const options_t options
) {
    int currentChar = 'h';
    const AssemblyData* data = newAssemblyData(options);

    while (cpu->pc < options.instruction_count_aligned) {
        executeSingleStep(winProg, winRegs, winStatus, winCmd, window, &currentChar, cpu, options, data, 0);

    }

}

void runCpuStepByStep(
          WINDOW *  winProg,
          WINDOW *  winRegs,
          WINDOW *  winStatus,
          WINDOW *  winCmd,
          Windows*  window,
          Cpu    *  cpu,
    const options_t options
) {
    int currentChar = 'h';
    AssemblyData* data = newAssemblyData(options);

    while (cpu->pc < options.instruction_count_aligned) {
        const int result = executeSingleStep(
            winProg, winRegs, winStatus, winCmd,
            window, &currentChar, cpu, options, data, true
        );

        if (result == 0) {
            break;
        }


        if (cpu->resetFlag >= 0) {

            reExecuteUntilTarget(cpu, options);
            cpu->resetFlag = -1;
        }

    }

}

int executeSingleStep(
          WINDOW      * winProg,
          WINDOW      * winRegs,
          WINDOW      * winStatus,
          WINDOW      * winCmd,
          Windows     * window,
          int         * currentChar,
          Cpu         * cpu,
    const options_t     options,
          const AssemblyData* data,
    const bool          interactive

) {

    int32_t nextPc      = cpu->pc + 4;
    uint8_t operation   = 0;
    int     offsetProg  = 1;

    const DecodedInstruction decodedInstruction = decodeInstruction(fetchInstruction(cpu, options));

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

    if (printProgramWithCurrentInstruction(
        winProg,
        winRegs,
        winStatus,
        winCmd,
        window,
        currentChar,
        firstRegisterValue,
        secondOperand,
        result.result,
        cpu,
        options,
        data,
        &offsetProg

    )) {
        return 0;

    }

    // Execution jalr
    if (decodedInstruction.opcode == 0x67 && decodedInstruction.funct3 == 0x0) {
        if (unitControlRet.regWrite) {
            writeRegister(decodedInstruction.rd, cpu->pc + 4);
        }

        const int32_t base = getValueRegister(decodedInstruction.rs1);
        const int32_t offset = decodedInstruction.immediate;

        nextPc = (base + offset) & ~1;

    } else if (decodedInstruction.opcode == 0x6F) { // Execution jal

        if (unitControlRet.regWrite) {

            writeRegister(decodedInstruction.rd, cpu->pc + 4);

        }

        const int32_t offset = decodedInstruction.immediate;

        nextPc = (cpu->pc + offset) & ~1;

    } else if (unitControlRet.regWrite) {
        writeRegister(decodedInstruction.rd, result.result);

    }

    cpu->pc = nextPc;

    return 1;
}

void reExecuteUntilTarget(Cpu* cpu, const options_t options) {
    resetCpuState(cpu);

    int currentInstruction = 0;

    while (currentInstruction < cpu->resetFlag && cpu->pc < options.instruction_count_aligned) {

        executeInstructionSilently(cpu, options);
        currentInstruction++;
    }
}

void executeInstructionSilently(Cpu* cpu, const options_t options) {
    const DecodedInstruction decodedInstruction = decodeInstruction(fetchInstruction(cpu, options));
    const ControlSignals unitControlRet = getControlSignals(decodedInstruction.opcode);
    const AluOp aluOpEnum = getAluControl(
        unitControlRet.operation,
        decodedInstruction.funct3,
        decodedInstruction.funct7Bit30
    );

    uint8_t operation = 0;
    getAluOperationBits(aluOpEnum, &operation);

    const int32_t firstRegisterValue = getValueRegister(decodedInstruction.rs1);
    const int32_t secondOperand = unitControlRet.aluSrc ?
        decodedInstruction.immediate : getValueRegister(decodedInstruction.rs2);

    const Alu32BitResult result = alu32bit(firstRegisterValue, secondOperand, 0, operation);

    int32_t nextPc = cpu->pc + 4;

    if (decodedInstruction.opcode == 0x67 && decodedInstruction.funct3 == 0x0) { // jalr
        if (unitControlRet.regWrite) {
            writeRegister(decodedInstruction.rd, cpu->pc + 4);
        }

        const int32_t base = getValueRegister(decodedInstruction.rs1);
        const int32_t offset = decodedInstruction.immediate;
        nextPc = (base + offset) & ~1;

    } else if (decodedInstruction.opcode == 0x6F) { // jal
        if (unitControlRet.regWrite) {
            writeRegister(decodedInstruction.rd, cpu->pc + 4);
        }

        const int32_t offset = decodedInstruction.immediate;
        nextPc = (cpu->pc + offset) & ~1;

    } else if (unitControlRet.regWrite) {
        writeRegister(decodedInstruction.rd, result.result);

    }

    cpu->pc = nextPc;
}

void resetCpuState(Cpu* cpu) {
    cpu->pc = 0;

    for (int i = 1; i < 32; i++) {
        writeRegister(i, 0);
    }
}

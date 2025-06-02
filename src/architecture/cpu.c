//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include <stdlib.h>

#include "cpu.h"

#include <stdio.h>

#include "alu.h"
#include "aluControl.h"
#include "controlUnit.h"
#include "instructionMemory.h"
#include "registerMemory.h"
#include "tui.h"

Cpu* newCpu() {
    Cpu* cpu = malloc(sizeof(Cpu));

    if (!cpu) return nullptr;
    cpu->pc = 0;

    return cpu;
}

void runCpu(Cpu* cpu) {

    while (cpu->pc < MAX_INSTRUCTIONS) {
        Alu32BitResult result; // Result operation ALU

        const uint32_t instruction                  = fetchInstruction(cpu);                        // Fetch instruction

        const DecodedInstruction decodedInstruction = decodeInstruction(instruction);               // Decode the current instruction

        const ControlSignals unitControlRet         = getControlSignals(decodedInstruction.opcode); // Operation selected

        const AluOp aluOpEnum = getAluControl(unitControlRet.operation, decodedInstruction.funct3, decodedInstruction.funct7Bit30);

        bool operation[5];
        getAluOperationBits(aluOpEnum, operation);

        // Use rs2, Type-R
        if (!unitControlRet.aluSrc) {

            const int32_t firstRegisterValue  = getValueRegister(decodedInstruction.rs1);
            const int32_t secondRegisterValue = getValueRegister(decodedInstruction.rs2);

            result = alu32bit(
                firstRegisterValue,
                secondRegisterValue,
                0,
                operation
            );

        } else {

            const int32_t firstRegisterValue = getValueRegister(decodedInstruction.rs1);
            const int32_t immediateOperator  = decodedInstruction.immediate;

            result = alu32bit(
                        firstRegisterValue,
                       immediateOperator,
                      0,
                          operation
                     );

        }

        if (unitControlRet.regWrite) {
            writeRegister(decodedInstruction.rd, result.result);
        }

        printRegisterTable();
        cpu->pc += 1;
    }

}

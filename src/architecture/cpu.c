//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include "cpu.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

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

void runCpuFull(Cpu* cpu) {
    while (cpu->pc < SIZE_INSTRUCTIONS) {
        executeSingleStep(cpu, 0);
    }

    // Print the final register table
    printFinalRegisterTable();
}

void runCpuStepByStep(Cpu* cpu) {
    while (cpu->pc < SIZE_INSTRUCTIONS) {
        if (!executeSingleStep(cpu, 1)) break;
    }

    // Print the final register table
    printFinalRegisterTable();
}

int executeSingleStep(Cpu* cpu, const bool interactive) {
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

    //printInstructionStatus(decodedInstruction, firstRegisterValue, secondOperand, result.result, cpu->pc);

    printProgramWithCurrentInstruction(firstRegisterValue, secondOperand, result.result, cpu->pc);
    //const DecodedInstruction currentDecoded,
    //                                    const int32_t input1, const int32_t input2, const int32_t result, const int32_t pc) {


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

    if (interactive) {
        commandList();

        char input[10]; // aumentiamo un po' la dimensione
        while (1) {
            printf("Enter command: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                break;
            }

            input[strcspn(input, "\n")] = '\0';

            const char cmd = tolower(input[0]);

            if (cmd == 'q') return 0;

            if (cmd == 'r') {
                printf("\n");
                printRegisterTable();
                printf("\nPress Enter to continue...\n");

                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }

            break;
        }

    }

    return 1;
}

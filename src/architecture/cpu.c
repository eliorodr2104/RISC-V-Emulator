//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#include <stdlib.h>

#include "cpu.h"
#include "alu.h"
#include "aluControl.h"
#include "assemblyData.h"
#include "controlUnit.h"
#include "instructionMemory.h"
#include "registerMemory.h"
#include "tuiCpu.h"
#include "windowsManagement.h"

/**
 * @brief Create a new CPU instance
 *
 * @return A pointer to the newly created CPU instance, or nullptr if memory allocation fails
 */
Cpu* newCpu() {
    // Allocate memory for the CPU structure
    Cpu* cpu = malloc(sizeof(Cpu));

    // Check if memory allocation was successful
    if (!cpu) return nullptr;

    // Initialize the CPU state
    cpu->pc = 0;
    cpu->resetFlag = -1;

    /*for (int i = 0; i < 32; i++) {
        writeRegister(i, 0);
    }*/

    // Return CPU instance
    return cpu;
}

/**
 * @brief Run the CPU in full execution mode, executing all instructions until completion.
 *
 * @param windowManagement The management structure for the TUI windows
 * @param cpu The CPU instance to run
 * @param options The options for execution
 */
void runCpuFull(
    const WindowsManagement windowManagement,
          Cpu*              cpu,
    const options_t         options
) {
    // int currentChar = 'h';
    //const AssemblyData* data = newAssemblyData(options);

    /*while (cpu->pc < options.instruction_count_aligned) {
        executeSingleStep(windowManagement, &currentChar, cpu, options, data, 0);

    }*/

}

/**
 * @brief Run the CPU in step-by-step execution mode, allowing for interactive debugging.
 *
 * @param windowManagement The management structure for the TUI windows
 * @param cpu The CPU instance to run
 * @param options The options for execution
 */
void runCpuStepByStep(
    const WindowsManagement windowManagement,
          Cpu*              cpu,
    const options_t         options
) {

    // TODO(Add "currentChar" to the WindowsManagement struct, and pass AssemblyData to the function)
          int           currentChar = 'h';
    const AssemblyData* data        = newAssemblyData(options);

    // While the program counter is less than the aligned instruction count
    while (cpu->pc < options.instruction_count_aligned) {

        // Execute a single step of the CPU, if the execution fails then break the loop
        if (!executeSingleStep(windowManagement, &currentChar, cpu, options, data, true)) {
            break;
        }

        // If the reset flag is set to a valid value, re-execute instructions until the target instruction
        if (cpu->resetFlag >= 0) {

            // Reset the CPU state to the initial state
            reExecuteUntilTarget(cpu, options);

            // Reset the reset flag to -1 to indicate that the CPU is no longer in a reset state
            cpu->resetFlag = -1;
        }

    }

}

/**
 * @brief Execute a single step of the CPU, fetching and decoding the instruction, and updating the CPU state.
 *
 * @param windowManagement The management structure for the TUI windows
 * @param currentChar Pointer to the current character input for interactive mode
 * @param cpu The CPU instance to run
 * @param options The options for execution
 * @param data The assembly data containing instructions and metadata
 * @param interactive Flag indicating if the execution is in interactive mode
 * @return 1 if the step was executed successfully, 0 if an error occurred or execution should stop
 */
int executeSingleStep(
    const WindowsManagement windowManagement,
          int         * currentChar,
          Cpu         * cpu,
    const options_t     options,
          const AssemblyData* data,
    const bool          interactive

) {

    // Calculate the next program counter value, this value is temp, because, the PC can change if execute a jalr or jal instruction
    int32_t nextPc      = cpu->pc + 4;

    // Get the ALU operation value
    uint8_t operation   = 0;

    // Offset for the program window, used to scroll through the instructions
    int     offsetProg  = 1;

    // Fetch and decode the instruction at the current program counter
    const DecodedInstruction decodedInstruction = decodeInstruction(fetchInstruction(cpu, options));

    // Get the control signals for the instruction based on its opcode
    const ControlSignals unitControlRet         = getControlSignals(decodedInstruction.opcode);

    // Determine the ALU operation based on the operation code, funz3 and funz7Bit30
    const AluOp aluOpEnum                       = getAluControl(
                                                        unitControlRet.operation,
                                                        decodedInstruction.funct3,
                                                        decodedInstruction.funct7Bit30
                                                    );

    // TODO(Union the getAluOperationBits and getAluControl functions)
    // Get the ALU operation bits for the ALU operation enum
    getAluOperationBits(aluOpEnum, &operation);

    // Invalid operation, return 0 to indicate an error
    if (operation == 0xF) return 0;

    // Get the first register value based on the decoded instruction's rs1 field
    const int32_t firstRegisterValue = getValueRegister(decodedInstruction.rs1);

    // Determine the second operand based on whether the ALU source is immediate or a register
    // if aluSrc is true, then use the immediate value, otherwise use the value from rs2 register
    const int32_t secondOperand = unitControlRet.aluSrc ? decodedInstruction.immediate : getValueRegister(decodedInstruction.rs2);

    // Perform the ALU operation with the first register value, second operand, and operation bits
    const Alu32BitResult result = alu32bit(firstRegisterValue, secondOperand, 0, operation);

    // Show all instructions and highlight the current instruction in a program window
    // If the function returns true, then it means that the program should stop executing
    if (printProgramWithCurrentInstruction(
        windowManagement,
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

    // Control if the current instruction is a
    if ((decodedInstruction.opcode == 0x67 && decodedInstruction.funct3 == 0x0) || decodedInstruction.opcode == 0x6F) {

        // If the unit control signals indicate that rd register should be written next instruction
        if (unitControlRet.regWrite) {
            writeRegister(decodedInstruction.rd, cpu->pc + 4);
        }

        // Calculate the next program counter based on the jalr or jal instruction
        nextPc = (decodedInstruction.opcode == 0x6F ? (cpu->pc + secondOperand) & ~1 : (firstRegisterValue + secondOperand) & ~1);

        // If the instruction is not jalr or jal, then write the result to the rd register if regWrite is true
    } else if (unitControlRet.regWrite) {
        writeRegister(decodedInstruction.rd, result.result);

    }

    // Update the program counter to the next instruction address
    cpu->pc = nextPc;

    // If the current character is not 'q' or 'Q', return 1 to indicate successful execution
    return 1;
}

/**
 * @brief Re-execute instructions until the target instruction is reached, resetting the CPU state.
 *
 * @param cpu The CPU instance to reset and re-execute
 * @param options The options for execution
 */
void reExecuteUntilTarget(Cpu* cpu, const options_t options) {

    // Reset the CPU state to the initial state
    resetCpuState(cpu);

    // Count the number of instructions executed
    uint32_t currentInstruction = 0;

    while (currentInstruction < cpu->resetFlag && cpu->pc < options.instruction_count_aligned) {

        executeInstructionSilently(cpu, options);
        currentInstruction++;
    }
}

/**
 * @brief Execute an instruction silently without updating the TUI or printing output.
 *
 * @param cpu The CPU instance to execute the instruction on
 * @param options The options for execution
 */
void executeInstructionSilently(Cpu* cpu, const options_t options) {

    // Fetch and decode the instruction at the current program counter
    const DecodedInstruction decodedInstruction = decodeInstruction(fetchInstruction(cpu, options));

    // Get the control signals for the instruction based on its opcode
    const ControlSignals unitControlRet = getControlSignals(decodedInstruction.opcode);

    // Determine the ALU operation based on the operation code, funct3, and funct7Bit30
    const AluOp aluOpEnum = getAluControl(
        unitControlRet.operation,
        decodedInstruction.funct3,
        decodedInstruction.funct7Bit30
    );

    // Get the ALU operation bits for the ALU operation enum
    uint8_t operation = 0;

    // TODO(Union the getAluOperationBits and getAluControl functions)
    // Get the ALU operation bits for the ALU operation enum
    getAluOperationBits(aluOpEnum, &operation);

    // Invalid operation, return early
    if (operation == 0xFF) return;

    // Get the first register value based on the decoded instruction's rs1 field
    const int32_t firstRegisterValue = getValueRegister(decodedInstruction.rs1);

    // Determine the second operand based on whether the ALU source is immediate or a register
    const int32_t secondOperand      = unitControlRet.aluSrc ? decodedInstruction.immediate : getValueRegister(decodedInstruction.rs2);

    // Perform the ALU operation with the first register value, second operand, and operation bits
    const Alu32BitResult result = alu32bit(firstRegisterValue, secondOperand, 0, operation);

    // Calculate the next program counter value, this value is temp, because, the PC can change if execute a jalr or jal instruction
    int32_t nextPc = cpu->pc + 4;

    // Control if the current instruction is a
    if ((decodedInstruction.opcode == 0x67 && decodedInstruction.funct3 == 0x0) || decodedInstruction.opcode == 0x6F) {

        // If the unit control signals indicate that rd register should be written next instruction
        if (unitControlRet.regWrite) {
            writeRegister(decodedInstruction.rd, cpu->pc + 4);
        }

        // Calculate the next program counter based on the jalr or jal instruction
        nextPc = (decodedInstruction.opcode == 0x6F ? (cpu->pc + secondOperand) & ~1 : (firstRegisterValue + secondOperand) & ~1);

        // If the instruction is not jalr or jal, then write the result to the rd register if regWrite is true
    } else if (unitControlRet.regWrite) {
        writeRegister(decodedInstruction.rd, result.result);

    }

    // Update the program counter to the next instruction address
    cpu->pc = nextPc;
}

/**
 * @brief Reset the CPU state to its initial values.
 *
 * @param cpu The CPU instance to reset
 */
void resetCpuState(Cpu* cpu) {
    // Reset the program counter to 0
    cpu->pc = 0;

    // Reset all registers to 0
    for (int i = 1; i < 32; i++) {
        writeRegister(i, 0);
    }
}

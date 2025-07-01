/**
* @file execute.h
* @brief Execute complete instruction RISC-V in separate fields.
*
**
* @author eliorodr2104
* @date 15/06/25
*
*/

#include "alu.h"
#include "alu_control.h"
#include "control_unit.h"
#include "cpu.h"
#include "decode.h"
#include "windows_management.h"
#include "fetch.h"
#include "tui_cpu.h"
#include "execute.h"

/**
 * @brief Execute a single step of the CPU, fetching and decoding the instruction, and updating the CPU state.
 *
 * @param window_management The management structure for the TUI windows
 * @param current_char Pointer to the current character input for interactive mode
 * @param cpu The CPU instance to run
 * @param options The options for execution
 * @param data The assembly data containing instructions and metadata
 * @param interactive Flag indicating if the execution is in interactive mode
 * @param main_memory The main memory instance where instructions are stored
 * @return 1 if the step was executed successfully, 0 if an error occurred or execution should stop
 */
int executeSingleStep(
          Cpu               cpu,
    const options_t         options,
    const AssemblyData*     data,
    const WindowsManagement window_management,
          int*              current_char,
          bool              interactive,
          RAM               main_memory

) {

    if (!cpu || !main_memory) return 0;

    // Calculate the next program counter value, this value is temp, because the PC can change if execute a jalr or jal instruction
    uint32_t nextPc      = cpu->pc + 4;

    // Get the ALU operation value
    uint8_t operation   = 0;

    // Offset for the program window, used to scroll through the instructions
    int     offsetProg  = 1;

    const uint32_t raw_instruction = fetchInstruction(cpu, options, main_memory);
    if (raw_instruction == -1) {
        perror("Error fetching instruction");
        return -1;
    }

    // Fetch and decode the instruction at the current program counter
    const DecodedInstruction decodedInstruction = decodeInstruction(raw_instruction);

    // Get the control signals for the instruction based on its opcode
    const ControlSignals unitControlRet         = getControlSignals(decodedInstruction.opcode);

    // Determine the ALU operation based on the operation code, funz3 and funz7Bit30
    const AluOp aluOpEnum                       = get_alu_control(
                                                        unitControlRet.operation,
                                                        decodedInstruction.funz3,
                                                        decodedInstruction.funz7_bit30
                                                    );

    // TODO(Union the getAluOperationBits and getAluControl functions)
    // Get the ALU operation bits for the ALU operation enum
    get_alu_operation_bits(aluOpEnum, &operation);

    // Invalid operation, return 0 to indicate an error
    if (operation == 0xF) {
        perror("Invalid operation");
        return -1;
    }

    // Get the first register value based on the decoded instruction's rs1 field
    const int32_t firstRegisterValue = getValueRegister(cpu, decodedInstruction.rs1);

    // Determine the second operand based on whether the ALU source is immediate or a register
    // if aluSrc is true, then use the immediate value, otherwise use the value from rs2 register
    const int32_t secondOperand = unitControlRet.alu_src ? decodedInstruction.immediate : getValueRegister(cpu, decodedInstruction.rs2);

    // Perform the ALU operation with the first register value, second operand, and operation bits
    const Alu32BitResult result = alu_32_bit(firstRegisterValue, secondOperand, 0, operation);

    // Show all instructions and highlight the current instruction in a program window
    // If the function returns true, then it means that the program should stop executing
    if (printProgramWithCurrentInstruction(
        window_management,
        current_char,
        firstRegisterValue,
        secondOperand,
        result.result,
        cpu,
        options,
        data,
        &offsetProg,
        main_memory

    )) { return -1; }

    // Control if the current instruction is a jalr or jal instruction
    if ((decodedInstruction.opcode == 0x67 && decodedInstruction.funz3 == 0x0) || decodedInstruction.opcode == 0x6F) {

        // If the unit control signals indicate that rd register should be written next instruction
        if (unitControlRet.reg_write) {
            writeRegister(cpu, decodedInstruction.rd, (int32_t)(cpu->pc + 4));
        }

        // Calculate the next program counter based on the jalr or jal instruction
        nextPc = decodedInstruction.opcode == 0x6F ? cpu->pc + secondOperand & ~1 : firstRegisterValue + secondOperand & ~1;

        // If the instruction is not jalr or jal, then write the result to the rd register if regWrite is true
    } else if (unitControlRet.reg_write) {
        writeRegister(cpu, decodedInstruction.rd, result.result);

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
void reExecuteUntilTarget(
          Cpu cpu,
    const options_t options,
          RAM       main_memory

) {

    // Reset the CPU state to the initial state
    resetCpuState(cpu, options);

    // Count the number of instructions executed
    uint32_t currentInstruction = 0;

    while (currentInstruction < cpu->reset_flag && cpu->pc < options.text_vaddr + options.text_size) {
        executeInstructionSilently(cpu, options, main_memory);
        currentInstruction++;

    }
}

/**
 * @brief Execute an instruction silently without updating the TUI or printing output.
 *
 * @param cpu The CPU instance to execute the instruction on
 * @param options The options for execution
 */
void executeInstructionSilently(
          Cpu cpu,
    const options_t options,
          RAM       main_memory
) {

    const uint32_t raw_instruction = fetchInstruction(cpu, options, main_memory);
    if (raw_instruction == -1) {
        perror("Error fetching instruction");
        return;
    }

    // Fetch and decode the instruction at the current program counter
    const DecodedInstruction decodedInstruction = decodeInstruction(raw_instruction);

    // Get the control signals for the instruction based on its opcode
    const ControlSignals unitControlRet = getControlSignals(decodedInstruction.opcode);

    // Determine the ALU operation based on the operation code, funct3, and funct7Bit30
    const AluOp aluOpEnum = get_alu_control(
        unitControlRet.operation,
        decodedInstruction.funz3,
        decodedInstruction.funz7_bit30
    );

    // Get the ALU operation bits for the ALU operation enum
    uint8_t operation = 0;

    // TODO(Union the getAluOperationBits and getAluControl functions)
    // Get the ALU operation bits for the ALU operation enum
    get_alu_operation_bits(aluOpEnum, &operation);

    // Invalid operation, return early
    if (operation == 0xFF) return;

    // Get the first register value based on the decoded instruction's rs1 field
    const int32_t firstRegisterValue = getValueRegister(cpu, decodedInstruction.rs1);

    // Determine the second operand based on whether the ALU source is immediate or a register
    const int32_t secondOperand      = unitControlRet.alu_src ? decodedInstruction.immediate : getValueRegister(cpu, decodedInstruction.rs2);

    // Perform the ALU operation with the first register value, second operand, and operation bits
    const Alu32BitResult result = alu_32_bit(firstRegisterValue, secondOperand, 0, operation);

    // Calculate the next program counter value, this value is temp, because, the PC can change if execute a jalr or jal instruction
    uint32_t nextPc = cpu->pc + 4;

    // Control if the current instruction is a
    if ((decodedInstruction.opcode == 0x67 && decodedInstruction.funz3 == 0x0) || decodedInstruction.opcode == 0x6F) {

        // If the unit control signals indicate that rd register should be written next instruction
        if (unitControlRet.reg_write) {
            writeRegister(cpu, decodedInstruction.rd, (int32_t)cpu->pc + 4);
        }

        // Calculate the next program counter based on the jalr or jal instruction
        nextPc = decodedInstruction.opcode == 0x6F ? cpu->pc + secondOperand & ~1 : firstRegisterValue + secondOperand & ~1;

        // If the instruction is not jalr or jal, then write the result to the rd register if regWrite is true
    } else if (unitControlRet.reg_write) {
        writeRegister(cpu, decodedInstruction.rd, result.result);

    }

    // Update the program counter to the next instruction address
    cpu->pc = nextPc;
}
cmake_minimum_required(VERSION 3.30)
project(RISC_V C)

set(CMAKE_C_STANDARD 23)

add_executable(RISC_V src/main.c
        src/include/alu.h
        src/architecture/alu/alu.c
        src/include/instructionMemory.h
        src/include/cpu.h
        src/architecture/memory/instructionMemory.c
        src/include/tui.h
        src/terminalInterface/tui.c
        src/architecture/cpu.c
        src/include/controlUnit.h
        src/architecture/controlUnit/controlUnit.c
        src/include/registerMemory.h
        src/architecture/memory/registerMemory.c
        src/include/tools.h
        src/architecture/alu/aluControl.c
        src/include/aluControl.h
        src/architecture/tools.c)

target_include_directories(RISC_V PRIVATE src/include)

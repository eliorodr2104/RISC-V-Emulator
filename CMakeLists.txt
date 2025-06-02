cmake_minimum_required(VERSION 3.30)
project(RISC_V C)

set(CMAKE_C_STANDARD 23)

add_executable(RISC_V main.c
        include/alu.h
        architecture/alu/alu.c
        include/instructionMemory.h
        include/cpu.h
        architecture/memory/instructionMemory.c
        include/tui.h
        architecture/tui.c
        architecture/cpu.c
        include/controlUnit.h
        architecture/controlUnit/controlUnit.c
        include/registerMemory.h
        architecture/memory/registerMemory.c
        include/tools.h
        architecture/tools.c
        architecture/alu/aluControl.c
        include/aluControl.h)

target_include_directories(RISC_V PRIVATE include)

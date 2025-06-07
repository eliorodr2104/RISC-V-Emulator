cmake_minimum_required(VERSION 3.30)
project(RISC_V C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS_RELEASE "-O3 -std=c2x")

set(CMAKE_PREFIX_PATH "/opt/homebrew")
find_package(Curses REQUIRED)

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

if (CURSES_FOUND)
    target_include_directories(RISC_V PRIVATE ${CURSES_INCLUDE_DIR})

else()
    message(FATAL_ERROR "ncurses non trovato: verifica CMake_PREFIX_PATH o installazione Homebrew.")

endif()

target_link_libraries(RISC_V PRIVATE ${CURSES_LIBRARIES})

target_include_directories(RISC_V PRIVATE src/include)
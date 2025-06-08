cmake_minimum_required(VERSION 3.30)
project(RISC_V C)

# Set C standard and compiler flags
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set optimization flags for Release build
set(CMAKE_C_FLAGS_RELEASE "-O3 -std=c2x")

# Platform-specific configurations
if(APPLE)

# macOS-specific paths
set(CMAKE_PREFIX_PATH "/opt/homebrew" ${CMAKE_PREFIX_PATH})

# Windows-specific paths if needed
elseif(WIN32)

# Add PDCurses paths if using PDCurses

# Linux-specific paths if needed
elseif(UNIX)
# Linux-specific paths if needed

endif()

# Find ncurses package
find_package(Curses REQUIRED)
if(NOT CURSES_FOUND)
message(FATAL_ERROR "ncurses library not found. Please install ncurses:\n"
"- Linux: sudo apt-get install libncurses-dev\n"
"- macOS: brew install ncurses\n"
"- Windows: Install PDCurses or ncurses port")
endif()

# Define source files
set(SOURCE_FILES
src/main.c
src/architecture/alu/alu.c
src/architecture/memory/instructionMemory.c
        src/tui/cpu/tuiCpu.c
src/architecture/cpu.c
src/architecture/controlUnit/controlUnit.c
src/architecture/memory/registerMemory.c
src/architecture/alu/aluControl.c
src/architecture/tools.c
)

# Define header files
set(HEADER_FILES
src/include/alu.h
src/include/instructionMemory.h
src/include/cpu.h
        src/include/tui/cpu/tuiCpu.h
src/include/controlUnit.h
src/include/registerMemory.h
src/include/tools.h
src/include/aluControl.h
        src/include/tui/main/tuiMain.h
        src/tui/main/tuiMain.c
        src/tui/ncurses/tuiNcurses.c
        src/include/tui/ncurses/tuiNcurses.h
)

# Create executable
add_executable(RISC_V ${SOURCE_FILES} ${HEADER_FILES})

# Include directories
target_include_directories(RISC_V PRIVATE
        src/include
        src/include/tui
        src/include/tui/main
        src/include/tui/ncurses
        src/include/tui/cpu
${CURSES_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(RISC_V PRIVATE ${CURSES_LIBRARIES})
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
        # Alu source files
        src/alu/alu.c
        src/alu/alu_control.c

        # Main source files
        src/app/main.c

        # Assembler handler source files
        src/asm-handler/args_handler.c
        src/asm-handler/asm_file_parser.c
        src/asm-handler/assembly_data.c

        # Control unit source files
        src/control-unit/control_unit.c

        # CPU source files
        src/cpu/cpu.c
        src/cpu/fetch.c
        src/cpu/decode.c
        src/cpu/execute.c

        # Frontend source files
        src/frontend/cpu-interface/tui_cpu.c
        src/frontend/main-interface/tui_main.c

        # Memory management source files
        src/memory/register_memory.c

        # Ncurses handler source files
        src/ncurses-handler/tui_ncurses.c

        # Utility source files
        src/utils/tools.c
)

# Define header files
set(HEADER_FILES
        # Alu headers
        src/alu/include/alu.h
        src/alu/include/alu_control.h

        # Assembler handler headers files
        src/asm-handler/include/args_handler.h
        src/asm-handler/include/asm_file_parser.h
        src/asm-handler/include/assembly_data.h

        # Control unit headers files
        src/control-unit/include/control_unit.h

        # CPU headers files
        src/cpu/include/cpu.h
        src/cpu/include/fetch.h
        src/cpu/include/decode.h
        src/cpu/include/execute.h

        # Frontend headers files
        src/frontend/include/tui_cpu.h
        src/frontend/include/tui_main.h
        src/frontend/include/windows_management.h

        # Memory management headers files
        src/memory/include/register_memory.h

        # Ncurses handler headers files
        src/ncurses-handler/include/tui_ncurses.h

        # Utility headers files
        src/utils/include/tools.h
)

# Create executable
add_executable(RISC_V ${SOURCE_FILES} ${HEADER_FILES})

# Include directories
target_include_directories(RISC_V PRIVATE
        src/alu
        src/alu/include
        src/app
        src/asm-handler
        src/asm-handler/include
        src/control-unit
        src/control-unit/include
        src/cpu
        src/cpu/include
        src/frontend
        src/frontend/include
        src/memory
        src/memory/include
        src/ncurses-handler
        src/ncurses-handler/include
        src/utils
        src/utils/include
${CURSES_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(RISC_V PRIVATE ${CURSES_LIBRARIES})
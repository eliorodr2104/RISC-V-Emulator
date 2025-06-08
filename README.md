# RISC-V Core Emulator

A low-level RISC-V CPU emulator that executes machine language instructions in step-by-step mode for educational purposes. This project provides a simulation environment to visually understand and work with the RISC-V architecture at the instruction level.

## Overview

This emulator is designed to:
- Read RISC-V binary instructions
- Execute instructions incrementally or instantaneously
- Simulate the functionality of a RISC-V CPU
- Provide information about CPU operations and their execution

## Features

### Implemented
- [x]  Execution of step-by-step instructions
- [x]  Interpretation of machine language instructions
- [x]  Partial support of the RV32I ISA
- [x]  Real-time display of register status

### In Development
- [ ]  Support for Type-I "load" instructions
- [ ]  Support for Type-S "save" instructions
- [ ]  Full support of the ISA RV32I
- [ ]  Support for memory management

## Getting Started

### Prerequisites

- CMake (version 3.30 or higher)
- C Compiler with C23 support
- ncurses library:
  - **Linux**: `libncurses-dev`
  - **macOS**: Available through Homebrew
  - **Windows**: PDCurses or Windows port of ncurses

### Installation

#### 1. Clone the repository
```bash
# Clone the repository
git clone https://github.com/eliorodr2104/RISC_V_Simulator.git

# Navigate to the project directory
cd RISC_V_Simulator
```

#### 2. Install dependencies

**Linux (Ubuntu/Debian)**:
```bash
sudo apt-get update
sudo apt-get install libncurses-dev
```

**macOS**:
```bash
brew install ncurses
```

**Windows**:
- Install PDCurses or Windows port of ncurses
- Add the installation path to CMAKE_PREFIX_PATH

#### 3. Build the project

```bash
# Create build directory
mkdir build
cd build

# Configure CMake
cmake ..

# Build
cmake --build . --config Release
```

## Usage

After building the project, you can run the emulator:

```bash
./RISC_V
```

## Project Status

This project is currently under development (when I have time lol). Currently the emulator is capable of executing machine language instructions in step-by-step mode, and other functions are planned for future implementation.

This project started as an experiment while studying for a university exam lol.

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests.

<h1 align="center">RISC-V Emulator</h1>

<div align="center">

![GitHub last commit](https://img.shields.io/github/last-commit/eliorodr2104/RISC-V-Emulator?style=for-the-badge&labelColor=101418&color=9ccbfb)
![GitHub Repo stars](https://img.shields.io/github/stars/eliorodr2104/RISC-V-Emulator?style=for-the-badge&labelColor=101418&color=b9c8da)
![GitHub repo size](https://img.shields.io/github/repo-size/eliorodr2104/RISC-V-Emulator?style=for-the-badge&labelColor=101418&color=d3bfe6)
![GitHub issues](https://img.shields.io/github/issues/eliorodr2104/RISC-V-Emulator?style=for-the-badge&labelColor=101418&color=ffb4a2)

</div>


> [!WARNING]
> The project is still under development, but all the help is welcome!
---

A low-level RISC-V CPU emulator that executes machine language instructions in step-by-step mode for educational purposes. This project provides a simulation environment to visually understand and work with the RISC-V architecture at the instruction level.

This project is heavily inspired by the [Helix Editor](https://github.com/helix-editor/helix).

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
- [x]  Cli interface for parsing a .s or binary file 

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
- RISC-V GCC Toolchain:
  - **macOS**: `riscv-software-src/riscv`

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
# @TODO add command for riscv gcc toolchain
```

**macOS**:
```bash
brew install ncurses

brew tap riscv-software-src/riscv
brew install riscv-tools

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


## Stats

<a href="https://www.star-history.com/#eliorodr2104/RISC-V-Emulator&Date">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/svg?repos=eliorodr2104/RISC-V-Emulator&type=Date&theme=dark" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/svg?repos=eliorodr2104/RISC-V-Emulator&type=Date" />
   <img alt="Star History Chart" src="https://api.star-history.com/svg?repos=eliorodr2104/RISC-V-Emulator&type=Date" />
 </picture>
</a>




#include <stdlib.h>
#include <unistd.h>

#include "args_handler.h"
#include "cpu.h"
#include "tuiMain.h"
#include "tuiNcurses.h"

int main(const int argc, char** argv) {
    // options_t opts = {nullptr}; // init options
    //
    // // Gestisce il parsing degli argomenti
    // const int result = handle_args(argc, argv, &opts);
    // if (result != 0)
    //     return result;
    //
    // // === Configuration Summary ===
    // // Binary file:     opts.binary_file
    // // Execution mode:  default/full/step by step
    // // Entry point:     opts.entry_point
    // // Instructions     opts.instruction_count
    //
    // print_options(&opts);
    //
    // return 0;

    Cpu* cpu = newCpu();

    WINDOW* winProg   = nullptr;
    WINDOW* winRegs   = nullptr;
    WINDOW* winStatus = nullptr;
    WINDOW* winCmd    = nullptr;

    initNcurses(&winRegs, &winProg, &winStatus, &winCmd);

    userChoices(winProg, winRegs, winStatus, winCmd, cpu);

    closeNcurses(&winRegs, &winProg, &winStatus);

    free(cpu);
    return 0;
}


#include <stdlib.h>

#include "args_handler.h"
#include "cpu.h"
#include "tuiMain.h"
#include "tuiNcurses.h"


int main(const int argc, char** argv) {
    options_t opts = { nullptr };

    const int result = handle_args(argc, argv, &opts);
    if (result != 0)
        return result;

    //print_options(&opts);

    Cpu* cpu = newCpu();

    WINDOW* winProg   = nullptr;
    WINDOW* winRegs   = nullptr;
    WINDOW* winStatus = nullptr;
    WINDOW* winCmd    = nullptr;

    initNcurses(&winRegs, &winProg, &winStatus, &winCmd);

    userChoices(winProg, winRegs, winStatus, winCmd, cpu, opts);

    closeNcurses(&winRegs, &winProg, &winStatus);

    free(cpu);
    free_options(&opts);

    return 0;
}


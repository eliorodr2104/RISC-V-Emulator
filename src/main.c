#include <stdlib.h>

#include "cpu.h"
#include "tuiMain.h"
#include "tuiNcurses.h"

int main(void) {
    Cpu* cpu = newCpu();

    WINDOW* winProg = nullptr;
    WINDOW* winRegs = nullptr;

    initNcurses(winRegs, winProg);

    userChoices(winProg, winRegs, cpu);

    closeNcurses(winRegs, winProg);

    free(cpu);
    return 0;
}


#include <stdlib.h>

#include "cpu.h"
#include "tuiMain.h"
#include "tuiNcurses.h"

int main(void) {
    Cpu* cpu = newCpu();

    WINDOW* winProg = nullptr;
    WINDOW* winRegs = nullptr;
    WINDOW* winStatus = nullptr;

    initNcurses(&winRegs, &winProg, &winStatus);

    userChoices(winProg, winRegs, winStatus, cpu);

    closeNcurses(&winRegs, &winProg, &winStatus);

    free(cpu);
    return 0;
}


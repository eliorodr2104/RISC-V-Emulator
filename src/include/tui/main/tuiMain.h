//
// Created by Eliomar Alejandro Rodriguez Ferrer on 08/06/25.
//

#ifndef TUIMAIN_H
#define TUIMAIN_H
#include "cpu.h"

typedef enum {

    PROG_WINDOW,
    REGS_WINDOW,
    STATUS_WINDOW,
    CMD_WINDOW,

} Windows;

// ########################
// # Main print functions #
// ########################
void userChoices(
    WINDOW* winProg,
    WINDOW* winRegs,
    WINDOW* winStatus,
    WINDOW* winCmd,
    Cpu* cpu
);

void commandWindow(
    WINDOW* winCmd,
    Windows window

);

#endif //TUIMAIN_H

//
// Created by Eliomar Alejandro Rodriguez Ferrer on 01/07/25.
//

#include "windows_management.h"

#include <stdlib.h>

void destroy_windows_management(const WindowsManagement window_management) {

    if (window_management.winProg) {
        delwin(window_management.winProg->window);
    }

    if (window_management.winRegs) {
        delwin(window_management.winRegs->window);
    }

    if (window_management.winStatus) {
        delwin(window_management.winStatus->window);
    }

    if (window_management.winCmd) {
        delwin(window_management.winCmd->window);
    }

    if (window_management.currentWindow)
        free(window_management.currentWindow);

}

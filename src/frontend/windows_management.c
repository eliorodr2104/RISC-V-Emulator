//
// Created by Eliomar Alejandro Rodriguez Ferrer on 01/07/25.
//

#include "windows_management.h"

#include <stdlib.h>

void destroy_windows_management(const WindowsManagement window_management) {

    if (window_management.window_left) {
        delwin(window_management.window_left->window);
    }

    if (window_management.window_right) {
        delwin(window_management.window_right->window);
    }

    if (window_management.window_bottom_right) {
        delwin(window_management.window_bottom_right->window);
    }

    if (window_management.bottom_window) {
        delwin(window_management.bottom_window->window);
    }

    if (window_management.window_on_focus)
        free(window_management.window_on_focus);

}

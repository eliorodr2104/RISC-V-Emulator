//
// Created by Eliomar Alejandro Rodriguez Ferrer on 08/06/25.
//

// ########################
// # Ncurses functions    #
// ########################

#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

void initNcurses(
    WINDOW** winRegs,
    WINDOW** winProg,
    WINDOW** winStatus,
    WINDOW** winCmd
) {
    setlocale(LC_ALL, "");

    // Init screen TUI
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    init_color(236, 300, 300, 300);

    if (has_colors()) {
        start_color();

        // (numberColors, colorText,    colorBg)
        init_pair(1,            COLOR_WHITE,  COLOR_BLACK);  // pair 1: title window
        init_pair(2,            COLOR_RED,    COLOR_BLACK);  // pair 2: init letter for command
        init_pair(3,            COLOR_BLACK,  COLOR_WHITE);  // pair 3: status bar

        init_pair(4,            COLOR_YELLOW, COLOR_BLACK);  // pair 4: current instruction
        init_pair(5,            COLOR_GREEN,  COLOR_BLACK);  // pair 5: text / result
        init_pair(6,            COLOR_CYAN,   COLOR_BLACK);  // pair 6: all registers

        init_pair(7,            COLOR_YELLOW, COLOR_BLACK);  // pair 7: Mark letter command
    }

    // Calc dim and create windows
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    const int regsHeight = rows / 2;
    const int regsWidth  = cols / 3;

    // Divide screen in two columns
    const int progHeight = rows;
    const int progWidth  = cols - regsWidth;

    // Left Window
    *winProg = newwin(progHeight - 3, progWidth, 0, 0);

    // Right Window
    *winRegs = newwin(regsHeight, regsWidth, 0, progWidth);

    // Bottom Right Window
    *winStatus = newwin(regsHeight - 2, regsWidth, regsHeight, progWidth);

    *winCmd = newwin(3, cols, rows - 3, 0);
}

void closeNcurses(
    WINDOW** winRegs,
    WINDOW** winProg,
    WINDOW** winStatus

) {
    refresh();

    // Clean
    delwin(*winProg);
    delwin(*winRegs);
    delwin(*winStatus);
    endwin();
}

void mvwprintwWrap(WINDOW *win, const int starty, const int startx, const char *str) {
    const int maxx = getmaxx(win);
    const int width = maxx - startx;
    int y = starty;
    const char *p = str;

    while (*p) {
        // se la rimanenza è più corta di width, stampo tutto
        const int len = strlen(p);
        if (len <= width) {
            mvwprintw(win, y++, startx, "%s", p);
            break;
        }

        // altrimenti cerco l’ultima spazio entro width
        int cut = width;
        while (cut > 0 && p[cut] != ' ')
            --cut;
        if (cut == 0) {
            // nessuno spazio: taglio duro
            cut = width;
        }

        // copio il pezzo in un buffer temporaneo
        char *buf = strndup(p, cut);
        mvwprintw(win, y++, startx, "%s", buf);
        free(buf);

        // salto eventuali spazi multipli
        p += cut;
        while (*p == ' ')
            ++p;
    }
}
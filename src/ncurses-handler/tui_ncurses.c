//
// Created by Eliomar Alejandro Rodriguez Ferrer on 08/06/25.
//

// ########################
// # Ncurses functions    #
// ########################

#define COLOR_GRAY  8
#define COLOR_GREEN 9
#define COLOR_AQUA  10
#define COLOR_BLUE  11

#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#include "tui_ncurses.h"

bool initNcurses(
    const WindowsManagement windowsManagement
) {
    setlocale(LC_ALL, "");

    // Init screen TUI
    initscr();
    if (!has_colors() || !can_change_color()) {
        endwin();
        fprintf(stderr, "The terminal not support colors.\n");
        exit(1);
    }

    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    init_color(COLOR_GRAY,  578, 510, 454);
    init_color(COLOR_GREEN, 602, 591, 102);
    init_color(COLOR_AQUA,  412, 615, 416);
    init_color(COLOR_BLUE,  270, 533, 537);


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


        init_pair(8,            COLOR_GRAY,   COLOR_BLACK);  // pair 8: zero register
        init_pair(9,            COLOR_GREEN,   COLOR_BLACK); // pair 9: zero register
        init_pair(10,            COLOR_AQUA,   COLOR_BLACK); // pair 10: zero register
        init_pair(11,            COLOR_BLUE,   COLOR_BLACK); // pair 11: zero register
    }

    // Calc dim and create windows
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    const int regsHeight = rows / 2;
    const int regsWidth  = cols / 3;

    // Divide screen in two columns
    const int progHeight = rows;
    const int progWidth  = cols - regsWidth;

    windowsManagement.winStatus->isActive = rows > 44 && cols > 154;

    // Left Window
    windowsManagement.winProg->window = newwin(progHeight - 3, progWidth, 0, 0);

    // Right Window
    if (windowsManagement.winRegs->isActive && windowsManagement.winStatus->isActive) {
        windowsManagement.winRegs->window = newwin(regsHeight + 7, regsWidth, 1, progWidth);

    } else if (!windowsManagement.winStatus->isActive) {
        windowsManagement.winRegs->window = newwin(progHeight - 4, regsWidth, 1, progWidth);

    }

    // Bottom Right Window
    windowsManagement.winStatus->window = windowsManagement.winStatus->isActive ? newwin(regsHeight - 10, regsWidth, regsHeight + 8, progWidth) : newwin(0, 0, 0, 0);

    windowsManagement.winCmd->window = newwin(3, cols, rows - 3, 0);

    // Check if windows are created correctly, if not, end ncurses
    if (!windowsManagement.winProg->window   ||
        !windowsManagement.winRegs->window   ||
        !windowsManagement.winStatus->window ||
        !windowsManagement.winCmd->window

    ) {
        endwin();
        return false;
    }

    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);

    keypad(windowsManagement.winStatus->window, TRUE);
    keypad(windowsManagement.winCmd->window, TRUE);
    keypad(windowsManagement.winProg->window, TRUE);
    keypad(windowsManagement.winRegs->window, TRUE);

    nodelay(windowsManagement.winProg->window, TRUE);
    nodelay(windowsManagement.winRegs->window, TRUE);

    return true;
}

void closeNcurses(
    WINDOW** winRegs,
    WINDOW** winProg,
    WINDOW** winStatus

) {
    refresh();

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

/**
 * @brief Verifica se le dimensioni del terminale sono sufficienti
 * @param rows Numero di righe correnti
 * @param cols Numero di colonne correnti
 * @return true se le dimensioni sono sufficienti, false altrimenti
 */
bool checkTerminalSize(const int rows, const int cols) {
    return (rows >= MIN_ROWS && cols >= MIN_COLS);
}

/**
 * @brief Mostra un messaggio di errore per terminale troppo piccolo
 * @param rows Numero di righe correnti
 * @param cols Numero di colonne correnti
 */
void showTerminalTooSmallMessage(int rows, int cols) {
    clear();

    // Calcola la posizione centrale
    int start_row = rows / 2 - 3;
    int start_col = cols / 2 - 25;

    if (start_row < 0) start_row = 0;
    if (start_col < 0) start_col = 0;

    // Mostra il messaggio di errore
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(start_row, start_col, "TERMINAL TOO SMALL!");
    attroff(COLOR_PAIR(2) | A_BOLD);

    mvprintw(start_row + 1, start_col - 5, "Minimum size required: %dx%d", MIN_COLS, MIN_ROWS);
    mvprintw(start_row + 2, start_col - 3, "Current size: %dx%d", cols, rows);
    mvprintw(start_row + 4, start_col - 8, "Please resize your terminal...");
    mvprintw(start_row + 5, start_col - 3, "Press 'q' to quit");

    refresh();

    // Loop per gestire input con timeout
    timeout(100); // Timeout di 100ms per non bloccare completamente
    int ch;
    while ((ch = getch()) != ERR) {
        if (ch == 'q' || ch == 'Q') {
            // Uscita forzata
            endwin();
            exit(0);
        }
        // Controlla se le dimensioni sono cambiate
        getmaxyx(stdscr, rows, cols);
        if (checkTerminalSize(rows, cols)) {
            break; // Esci dal loop se le dimensioni sono OK
        }
        // Aggiorna il messaggio con le nuove dimensioni
        clear();
        mvprintw(start_row + 2, start_col - 3, "Current size: %dx%d", cols, rows);
        refresh();
    }
    timeout(-1); // Ripristina il comportamento normale (blocking)
}

/**
 * @brief Ricrea tutte le finestre con le nuove dimensioni
 * @param windowManagement Struttura di gestione delle finestre
 * @return true se la ricreazione è avvenuta con successo, false altrimenti
 */
bool recreateWindows(const WindowsManagement* windowManagement) {

    // Ottieni le nuove dimensioni del terminale
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    // Verifica se le dimensioni sono sufficienti
    if (!checkTerminalSize(rows, cols)) {
        showTerminalTooSmallMessage(rows, cols);
        return false;
    }

    // Calcola le nuove dimensioni delle finestre
    const int regsHeight = rows / 2;
    const int regsWidth  = cols / 3;
    const int progHeight = rows;
    const int progWidth  = cols - regsWidth;

    windowManagement->winStatus->isActive = rows > 44 && cols > 154;

    // Ridimensiona e riposiziona la finestra principale (sinistra)
    if (windowManagement->winProg->window && windowManagement->winProg->isActive) {
        wresize(windowManagement->winProg->window, progHeight - 3, progWidth);
        mvwin(windowManagement->winProg->window, 0, 0);

    } else {
        wresize(windowManagement->winProg->window, 0, 0);
        mvwin(windowManagement->winProg->window, 0, 0);
    }

    // Ridimensiona e riposiziona la finestra registri (destra in alto)
    if (windowManagement->winRegs->window &&
        windowManagement->winRegs->isActive &&
        windowManagement->winStatus->isActive
    ) {

        windowManagement->winRegs->window = newwin(regsHeight + 8, regsWidth, 0, progWidth);
        mvwin(windowManagement->winRegs->window, 0, progWidth);

    } else if (!windowManagement->winStatus->isActive) {
        wresize(windowManagement->winRegs->window, progHeight - 3, regsWidth);
        mvwin(windowManagement->winRegs->window, 0, progWidth);

    }

    // Ridimensiona e riposiziona la finestra status (destra in basso)
    if (windowManagement->winStatus->window && windowManagement->winStatus->isActive) {
        wresize(windowManagement->winStatus->window, regsHeight - 10, regsWidth);
        mvwin(windowManagement->winStatus->window, regsHeight + 8, progWidth);

    } else {
        wresize(windowManagement->winStatus->window, 0, 0);
        mvwin(windowManagement->winStatus->window, regsHeight + 8, progWidth);

    }

    // Ridimensiona e riposiziona la finestra comandi (in basso)
    if (windowManagement->winCmd->window && windowManagement->winCmd->isActive) {
        wresize(windowManagement->winCmd->window, 3, cols);
        mvwin(windowManagement->winCmd->window, rows - 3, 0);

    } else {
        wresize(windowManagement->winCmd->window, 0, 0);
        mvwin(windowManagement->winCmd->window, rows - 3, 0);

    }

    // Verifica che tutte le finestre esistano ancora
    if (!windowManagement->winProg->window || !windowManagement->winRegs->window ||
        !windowManagement->winStatus->window || !windowManagement->winCmd->window) {
        return false;
    }

    // Pulisci e ridisegna tutte le finestre
    wclear(windowManagement->winProg->window);
    wclear(windowManagement->winRegs->window);
    wclear(windowManagement->winStatus->window);
    wclear(windowManagement->winCmd->window);

    // Aggiorna il display
    wrefresh(windowManagement->winProg->window);
    wrefresh(windowManagement->winRegs->window);
    wrefresh(windowManagement->winStatus->window);
    wrefresh(windowManagement->winCmd->window);

    // Riabilita le funzionalità necessarie
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);

    keypad(windowManagement->winStatus->window, TRUE);
    keypad(windowManagement->winCmd->window, TRUE);
    keypad(windowManagement->winProg->window, TRUE);
    keypad(windowManagement->winRegs->window, TRUE);

    nodelay(windowManagement->winProg->window, TRUE);
    nodelay(windowManagement->winRegs->window, TRUE);

    return true;
}

/**
 * @brief Gestisce il resize del terminale
 * @param windowManagement Struttura di gestione delle finestre
 * @return true se il resize è stato gestito correttamente, false altrimenti
 */
bool handleTerminalResize(const WindowsManagement* windowManagement) {

    // Notifica ncurses del resize
    endwin();
    refresh();
    clear();

    // Ottieni le nuove dimensioni
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    // Loop finché il terminale non ha dimensioni sufficienti
    if (!checkTerminalSize(rows, cols)) {
        showTerminalTooSmallMessage(rows, cols);


        getmaxyx(stdscr, rows, cols);
        if (!checkTerminalSize(rows, cols)) {
            return false; // Se ancora troppo piccolo, fallisce
        }
    }

    // Ricrea le finestre
    return recreateWindows(windowManagement);
}
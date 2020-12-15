#include <ncurses.h>
#include <spdlog/spdlog.h>

int main () {
    initscr();
    noecho();
    keypad(stdscr, true);

    box(stdscr, 0, 0);

    MEVENT mouse_event;
    WINDOW *main;
    int ch;

    mousemask(ALL_MOUSE_EVENTS, nullptr);
    main = subwin(stdscr, LINES - 2, COLS - 2, 1, 1);

    if (!main) {
        endwin();

    }

    do {
        ch = getch();

        if (ch == KEY_MOUSE) {
            getmouse(&mouse_event);
            mvaddch(mouse_event.y, mouse_event.x, '*');
            refresh();
        }
    } while (ch != '\n');


    endwin();
    return 0;
}

//
// Created by Roberts Kalnins on 16/12/2020.
//
#include <ncurses.h>

#include "intro.h"


namespace sc {


IntroWindow::IntroWindow () {
    logger_ = spdlog::basic_logger_mt("intro",
                                      "logs/space-colonist-log.log");
    logger_->set_level(spdlog::level::debug);

    box(stdscr, 0, 0);

    std::string welcome = "Welcome! You're a Space Colonist colonizing space!";
    std::string l1      = "Choose your crew, your supplies, and destination. Good luck!";
    std::string l2      = "Hit [ENTER] to continue.";
    mvaddstr(10, ( COLS - welcome.length()) / 2, welcome.c_str());
    mvaddstr(12, ( COLS - l1.length()) / 2, l1.c_str());
    mvaddstr(16, ( COLS - l2.length()) / 2, l2.c_str());


    logger_->debug("Setup intro window");
}

void IntroWindow::HideOnInput () {
    logger_->debug("Waiting to hide intro window");
    int ch;
    do {
        ch = getch();
    } while ( ch != 10 );
    logger_->debug("Hiding intro window", ch);

    refresh();
    erase();
    border(' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
}

}
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
    std::string l1      = "Choose your ship, destination, crew, and supplies. Good luck!";
    std::string l2      = "Hit [ENTER] to continue.";

    int intro_y = Config::GetInstance().GetValue< int >("window.intro-y",
                                                        10);

    mvaddstr(intro_y, ( COLS - welcome.length()) / 2, welcome.c_str());
    intro_y += 2;
    mvaddstr(intro_y, ( COLS - l1.length()) / 2, l1.c_str());
    intro_y += 2;
    mvaddstr(intro_y, ( COLS - l2.length()) / 2, l2.c_str());


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
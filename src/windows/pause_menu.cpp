//
// Created by Roberts Kalnins on 15/12/2020.
//

#include "pause_menu.h"


namespace sc {

PauseMenu::PauseMenu () {
    logger_ = spdlog::basic_logger_mt("pmenu",
                                      "logs/space-colonist-log.log");
    logger_->set_level(spdlog::level::debug);

    pause_menu_items_ = (ITEM **) calloc(choices_text_.size() + 1,
                                         sizeof(ITEM *));

    for ( size_t i = 0; i < choices_text_.size(); ++i ) {
        pause_menu_items_[i] = new_item(choices_text_[i].c_str(), "");
    }

    pause_menu_items_[choices_text_.size()] = nullptr;
    pause_      = new_menu((ITEM **) pause_menu_items_);
    pause_menu_ = newwin(k_menu_h_, k_menu_w, k_menu_y_, k_menu_x_);

    keypad(pause_menu_, true);

    set_menu_win(pause_, pause_menu_);
    set_menu_sub(pause_, derwin(pause_menu_, k_submenu_h_, k_submenu_w,
                                k_submenu_y_, k_submenu_x_));
    set_menu_mark(pause_, menu_mark_.c_str());

    init_pair(1, COLOR_RED, COLOR_BLACK);
}

void PauseMenu::print_in_middle ( int startx, int width, std::string &name,
                                  chtype color ) {
    int    length, x, y;
    double temp;

    getyx(pause_menu_, y, x);

    length = name.length();

    temp = ( width - length ) / 2.0;
    x    = startx + (int) temp;

    logger_->debug("{} {}", y, x);

    wattron(pause_menu_, color);
    mvwprintw(pause_menu_, 0, x, "%s", name.c_str());
    wattroff(pause_menu_, color);
}

void PauseMenu::Navigate ( int dir ) {
    menu_driver(pause_, dir);
}

int PauseMenu::Select () {
    int current = current_item(pause_)->index;
    logger_->debug("Option {} selected", current);
    return current;
}

PauseMenu::~PauseMenu () {
    Hide();
    for ( size_t i = 0; i < choices_text_.size(); ++i ) {
        free_item(pause_menu_items_[i]);
    }

    free_menu(pause_);
    wrefresh(pause_menu_);
    delwin(pause_menu_);
}

void PauseMenu::Show () {
    box(pause_menu_, 0, 0);
    print_in_middle(0, k_menu_w, name_, COLOR_PAIR(1));
    post_menu(pause_);
}

void PauseMenu::Hide () {
    unpost_menu(pause_);
    wborder(pause_menu_, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    werase(pause_menu_);
}

}
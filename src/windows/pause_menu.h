//
// Created by Roberts Kalnins on 15/12/2020.
//

#pragma once

#include <vector>
#include <string>
#include <menu.h>
#include <ncurses.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include "../logging.h"


namespace sc {


class PauseMenu {
  public:
    PauseMenu ();

    virtual ~PauseMenu ();

    void Navigate ( int dir );

    int Select ();

    void Show ();

    void Hide ();

    void Refresh () { wrefresh(pause_menu_); };

  private:
    static void
    print_in_middle ( WINDOW *win, int startx, int width,
                      std::string &name, chtype color );

  private:
    WINDOW *pause_menu_;
    ITEM   **pause_menu_items_;
    MENU   *pause_;

    logger_t logger_;

    std::string                      name_         = "Pause";
    const std::vector< std::string > choices_text_ = {
            "Resume",
            "Save",
            "Help",
            "Exit",
    };

    const int k_menu_h_ { 9 };
    const int k_menu_w { 25 };
    const int k_menu_y_ { 4 };
    const int k_menu_x_ { 4 };

    const int k_submenu_h_ { 6 };
    const int k_submenu_w { 20 };
    const int k_submenu_y_ { 2 };
    const int k_submenu_x_ { 1 };

    const std::string menu_mark_ { " * " };


};


}


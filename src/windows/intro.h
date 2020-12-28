//
// Created by Roberts Kalnins on 16/12/2020.
//

#pragma once

#include <ncurses.h>

#include "../logging/logging.h"
#include "../config/config.h"


namespace sc {

class IntroWindow {
  public:
    IntroWindow ();

    void HideOnInput ();

  private:
    logger_t logger_;
};


}


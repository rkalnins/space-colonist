//
// Created by Roberts Kalnins on 22/12/2020.
//

#pragma once

#include <ncurses.h>

#include "../spaceship.h"
#include "../nav_control_manager.h"
#include "../situation/situation_type.h"
#include "../menu_options.h"


namespace sc::play {

class PauseMenu {

  public:

    PauseMenu ( shared_spaceship_t spaceship,
                shared_nav_manager_t nav_manager,
                WINDOW *main );

    void ShowChangeRationsOptions ();

    void ShowVelocityChangeOptions ();

    void
    ShowPauseOptions ( SituationType situation, int ignored_failures );

    void PushNotification ( const std::string &notification );

    bool HasNotifications ();

    void ClearLastNotification ();

    void OnLoop ( MenuOptions menu_option, SituationType situation_type,
                  int ignored_failures );

  private:

    std::queue< std::string > notifications_;

    logger_t logger_;

    shared_spaceship_t   spaceship_ { nullptr };
    shared_nav_manager_t nav_manager_ { nullptr };

    WINDOW *main_;

    int pause_y_;
    int pause_x_;


};

}





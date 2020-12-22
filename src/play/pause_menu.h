//
// Created by Roberts Kalnins on 22/12/2020.
//

#pragma once

#include <ncurses.h>

#include "spaceship.h"
#include "nav_control_manager.h"
#include "situation_type.h"


namespace sc::play {

class PauseMenu {

  public:

    PauseMenu ( std::shared_ptr< Spaceship > spaceship,
                std::shared_ptr< NavigationControlManager > nav_manager,
                WINDOW *main );

    void ShowChangeRationsOptions ();

    void ShowVelocityChangeOptions ();

    void
    ShowPauseOptions ( SituationType situation, int ignored_failures );

    void PushNotification ( const std::string &notification );

    bool HasNotifications ();

    void ClearLastNotification ();

  private:

    std::queue< std::string > notifications_;

    std::shared_ptr< Spaceship >                spaceship_ { nullptr };
    std::shared_ptr< NavigationControlManager > nav_manager_ { nullptr };

    WINDOW *main_;

    const int pause_y_ { 32 };
    const int pause_x_ { 30 };


};

}





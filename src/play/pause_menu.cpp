//
// Created by Roberts Kalnins on 22/12/2020.
//

#include "pause_menu.h"

#include <sstream>
#include <utility>


namespace sc::play {

PauseMenu::PauseMenu ( std::shared_ptr< Spaceship > spaceship,
                       std::shared_ptr< NavigationControlManager > nav_manager,
                       WINDOW *main ) : spaceship_(std::move(spaceship)),
                                        nav_manager_(
                                                std::move(nav_manager)),
                                        main_(main) {}

void PauseMenu::ShowChangeRationsOptions () {

    int y = pause_y_;
    int x = pause_x_;

    mvwaddstr(main_, y++, x - 3, "Paused");
    mvwaddstr(main_, y++, x - 23,
              "---------------------------------------------");
    mvwaddstr(main_, y++, x - 10, "1. Half");
    mvwaddstr(main_, y++, x - 10, "2. Normal");
    mvwaddstr(main_, y++, x - 10, "3. Filling");
}

void PauseMenu::ShowVelocityChangeOptions () {
    int y = pause_y_;
    int x = pause_x_;

    std::stringstream disp;
    disp.precision(3);

    disp << spaceship_->GetFuel();
    std::string available = "Available: " + disp.str();
    disp.str("");

    disp << nav_manager_->GetStopFuel();
    std::string stop = "1. Stop (" + disp.str() + " fuel)";
    disp.str("");

    disp << nav_manager_->GetSlowFuel();
    std::string slow = "2. Slow (" + disp.str() + " fuel)";
    disp.str("");

    disp << nav_manager_->GetModerateFuel();
    std::string moderate = "3. Moderate (" + disp.str() + " fuel)";
    disp.str("");

    disp << nav_manager_->GetFastFuel();
    std::string fast = "4. Fast (" + disp.str() + " fuel)";
    disp.str("");

    disp << nav_manager_->GetDangerousFuel();
    std::string dangerous = "5. Dangerous (" + disp.str() + " fuel)";
    disp.str("");

    mvwaddstr(main_, y++, x - 3, "Paused");
    mvwaddstr(main_, y++, x - 23,
              "---------------------------------------------");
    mvwaddstr(main_, y++, x - 26,
              "Any velocity change requires fuel. Save enough fuel to stop.");
    mvwaddstr(main_, y++, x - 12, "Would be unfortunate...");
    mvwaddstr(main_, y++, x - 10, available.c_str());
    ++y;
    mvwaddstr(main_, y++, x - 10, stop.c_str());
    mvwaddstr(main_, y++, x - 10, slow.c_str());
    mvwaddstr(main_, y++, x - 10, moderate.c_str());
    mvwaddstr(main_, y++, x - 10, fast.c_str());
    mvwaddstr(main_, y++, x - 10, dangerous.c_str());
}

void PauseMenu::ShowPauseOptions ( SituationType situation,
                                   int ignored_failures ) {
    int y = pause_y_;
    int x = pause_x_;

    mvwaddstr(main_, y++, x - 3, "Paused");
    mvwaddstr(main_, y++, x - 23,
              "---------------------------------------------");

    if ( !notifications_.empty()) {
        mvwaddstr(main_, y++, x - notifications_.front().length() / 2,
                  notifications_.front().c_str());
        mvwaddstr(main_, y++, x - 23,
                  "---------------------------------------------");
    }
    std::string vel = ( situation != SituationType::ENGINE_FAILURE
                        ? "1. Change velocity"
                        : "1. <Unable to change velocity>" );
    mvwaddstr(main_, y++, x - 12, vel.c_str());
    mvwaddstr(main_, y++, x - 12, "2. Change rations");
    std::string       en     = ( nav_manager_->LowPowerEnabled()
                                 ? "enabled"
                                 : "disabled" );
    const std::string &power = "3. Toggle low power mode. Currently " + en;
    mvwaddstr(main_, y++, x - 12, power.c_str());

    if ( ignored_failures > 0 ) {
        std::stringstream out;
        out << "4. Attempt to fix one of (" << ignored_failures
            << ") minor issues";
        mvwaddstr(main_, y++, x - 12, out.str().c_str());
    }
    ++y;
    mvwaddstr(main_, y++, x - 12, "Hit [Space] to continue.");
}

bool PauseMenu::HasNotifications () {
    return !notifications_.empty();
}

void PauseMenu::PushNotification ( const std::string &notification ) {
    notifications_.push(notification);
}

void PauseMenu::ClearLastNotification () {
    if ( !notifications_.empty()) {
        notifications_.pop();
    }
}


}
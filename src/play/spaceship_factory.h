//
// Created by Roberts Kalnins on 19/12/2020.
//

#pragma once

#include <ncurses.h>

#include "../logging/logging.h"
#include "spaceship.h"
#include "../config/config.h"


namespace sc::play {

class SpaceshipFactory {

  public:

    SpaceshipFactory ();

    std::unique_ptr< Spaceship > CreateSpaceship ();

    bool PrintStation ( WINDOW *window, int cycle );

    bool PrintSpaceship ( WINDOW *window, int y, int x,
                          bool start_flip, bool can_flip,
                          bool start_burn );

    [[nodiscard]] int GetInitialMoney () const;

  private:

    std::unique_ptr< std::vector< const std::vector< const std::string>> > ship_;

    std::unique_ptr< std::vector< const std::string >> station_dep_;

    logger_t logger_;

    Range max_weight_;
    Range max_fuel_;
    Range max_hull_;
    Range max_crew_;

    int initial_money_ { 3000 };

    int station_init_x_ { 20 };
    int station_init_y_ { 0 };

    int    animation_index_ { 0 };
    bool   flipping_;
    double flip_speed_;
    int    direction_ { 1 };
    int    flip_counter_ { 0 };

    bool burning_ { false };
    int  burn_counter { 0 };
    int  max_burn_time_;
};


using shared_spaceship_factory_t = std::shared_ptr< SpaceshipFactory >;

}





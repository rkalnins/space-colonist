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

    void PrintSpaceship ( WINDOW *window, int y, int x,
                          const std::string &code );

    [[nodiscard]] int GetInitialMoney () const;

  private:

    const std::vector< const std::string > ss_a_ = {
            { R"(/\~~~~~~~~^\__/^\)" },
            { R"(|| <%%> [  ]__  ###)" },
            { R"(\/~~~~~~~~~/  \v/)" },
    };


    const std::vector< const std::string > ss_b_ = {
            { R"(/\@@@\--->)" },
            { R"(> [..].`.`< &&&&)" },
            { R"(\/@@@/--->)" },
    };

    const std::vector< const std::string > ss_c_ = {
            { R"(  (%%%\)" },
            { R"((### ::::<<<<)" },
            { R"(  (%%%/)" },
    };

    std::vector< bool > ss_used_ { false, false, false };

    logger_t logger_;

    Range max_weight_;
    Range max_fuel_;
    Range max_hull_;
    Range max_crew_;

    int initial_money_ { 3000 };
};


using shared_spaceship_factory_t = std::shared_ptr< SpaceshipFactory >;

}





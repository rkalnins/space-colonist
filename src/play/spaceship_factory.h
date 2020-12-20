//
// Created by Roberts Kalnins on 19/12/2020.
//

#pragma once

#include <ncurses.h>

#include "../logging/logging.h"
#include "objects/spaceship.h"


namespace sc::play {

class SpaceshipFactory {

  public:

    SpaceshipFactory ();

    std::shared_ptr< Spaceship > CreateSpaceship ();

    void PrintSpaceship ( WINDOW *window, int y, int x,
                                 const std::string &code );

    static int GetInitialMoney ();

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

    static const int min_max_weight_ { 50 };
    static const int max_max_weight_ { 100 };

    static const int min_max_fuel_ { 50 };
    static const int max_max_fuel_ { 100 };

    static const int min_max_hull_ { 50 };
    static const int max_max_hull_ { 100 };

    static const int min_max_crew_ { 2 };
    static const int max_max_crew_ { 3 };

    static const int initial_money_ { 1000 };
};

}





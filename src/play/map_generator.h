//
// Created by Roberts Kalnins on 19/12/2020.
//

#pragma once

#include <ncurses.h>
#include <vector>
#include <sstream>
#include <effolkronium/random.hpp>


#include "../logging/logging.h"


namespace sc::play {


class MapGenerator {

  public:
    MapGenerator ( WINDOW *main, int map_init_y, int map_init_x );

    void SeedMap ();

    void PrintMap ();

  private:
    std::vector< std::vector< bool>> planets_;
    std::vector< std::string >       planets_str_;

    logger_t logger_;

    WINDOW *main_;

    const int    map_init_y_;
    const int    map_init_x_;
    const int    map_height_ { 20 };
    const int    map_width_ { 100 };
    const double planet_probability_ { 0.01 };
};

}





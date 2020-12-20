//
// Created by Roberts Kalnins on 19/12/2020.
//

#pragma once

#include <ncurses.h>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <stack>
#include <effolkronium/random.hpp>


#include "../logging/logging.h"


namespace sc::play {

using PlanetsMap = std::vector< std::vector< bool>>;


class MapGenerator {

  public:
    MapGenerator ( WINDOW *main, int map_init_y, int map_init_x );

    void SeedMap ();

    void PrintMap ();

    void ToggleSelection ( int y, int x );

  private:
    struct LocationSelected {
        LocationSelected ( int y, int x ) : y(y), x(x) {}

        int y;
        int x;
    };


    PlanetsMap planets_;

    std::stack< LocationSelected > route_;

    std::vector< std::string > planets_str_;

    logger_t logger_;

    WINDOW *main_;

    int          max_route_length_    = 5;
    const double max_travel_distance_ = 30;
    const int    map_init_y_;
    const int    map_init_x_;
    const int    map_height_ { 20 };
    const int    map_width_ { 100 };
    const double planet_probability_ { 0.12 };
    double cost_ { 0 };

    bool toggle_end_ { false };

};

}





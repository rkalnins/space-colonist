//
// Created by Roberts Kalnins on 19/12/2020.
//

#include "map_generator.h"

#include <ncurses.h>
#include <vector>
#include <sstream>
#include <effolkronium/random.hpp>


using Random = effolkronium::random_static;

namespace sc::play {

MapGenerator::MapGenerator ( WINDOW *main, int map_init_y, int map_init_x )
        : main_(main), map_init_y_(map_init_y), map_init_x_(map_init_x) {
    logger_ = spdlog::basic_logger_mt("map_gen",
                                      "logs/space-colonist-log.log");
    logger_->set_level(spdlog::level::debug);

    planets_.resize(map_height_, std::vector< bool >(map_width_, false));
    planets_str_.reserve(map_height_);
}

void MapGenerator::SeedMap () {
    for ( auto &row : planets_ ) {
        for ( auto &&i : row ) {
            i = Random::get< bool >(planet_probability_);
        }
    }

    std::stringstream line;

    for ( auto &row : planets_ ) {
        for ( auto &&i : row ) {
            line << ( i ? "*" : " " );
        }
        planets_str_.push_back(line.str());
        line.str("");
    }
}

void MapGenerator::PrintMap () {

    int y = map_init_y_;

    for ( auto &row : planets_str_ ) {
        mvwaddstr(main_, y++, map_init_x_, row.c_str());
    }

}


}
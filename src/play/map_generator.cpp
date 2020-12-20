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
    double r = map_height_ / 2.0;
    double c = -map_width_ / 2.0;

    for ( auto &row : planets_ ) {
        for ( auto &&i : row ) {

//            double z = ( std::sin(
//                    0.8 / map_height_ * (( r ) * ( r ) + ( c / 3.0 ) * ( c / 3.0))) +
//                         1.0 ) / 2.0;

            double z = ((( r * 3.5 ) * ( r * 3.5 ) + ( c ) * ( c )) /
                        3.0 ) /
                       (( map_width_ ) * ( map_height_ ));


            i = Random::get< bool >(z * planet_probability_);

            c += 1.0;
        }
        r -= 1.0;
        c = -map_width_ / 2.0;
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

    mvwaddstr(main_, y - 2, map_init_x_ + map_width_ / 2 - 7,
              "Start: (*)");

    for ( auto &row : planets_str_ ) {
        mvwaddstr(main_, y++, map_init_x_, row.c_str());
    }

    mvwaddstr(main_, map_init_y_ + map_height_ + 2,
              map_init_x_ + map_width_ / 2 - 5, "End: (*)");


}

void MapGenerator::ToggleSelection ( int y, int x ) {

    if ( route_.empty()) {
        logger_->debug("{} {} {}", planets_str_[y][x],
                       route_.size(),
                       std::hypot(map_init_y_ - 2 - y,
                                  map_init_x_ + map_width_ / 2 - x));
    } else {
        logger_->debug("{} {} {}", planets_str_[y][x],
                       route_.size(),
                       std::hypot(route_.top().x - x, route_.top().y));
    }

    if ( !route_.empty() && route_.top().x == x && route_.top().y == y ) {
        planets_str_[y][x - 1] = ' ';
        planets_str_[y][x]     = '*';
        planets_str_[y][x + 1] = ' ';
        route_.pop();

        logger_->debug("Deselected selection at {} {}", y, x);
    } else if ( planets_str_[y][x] == '*' &&
                route_.size() < max_route_length_ ) {

        if ( route_.empty()) {
            if ( std::hypot(map_init_y_ - 2 - y,
                            map_init_x_ + map_width_ / 2 - x) >
                 max_travel_distance_ ) {
                return;
            }
        } else {
            if ( std::hypot(route_.top().x - x, route_.top().y - y) >
                 max_travel_distance_ ) {
                return;
            }
        }

        planets_str_[y][x - 1] = '(';
        planets_str_[y][x]     = '0' + (char) route_.size();
        planets_str_[y][x + 1] = ')';
        route_.emplace(y, x);

        logger_->debug("Selected selection at {} {}", y, x);

    }
}

void MapGenerator::AddRouteLeg () {
    ++max_route_length_;
}


}
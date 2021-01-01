//
// Created by Roberts Kalnins on 19/12/2020.
//

#include "spaceship_factory.h"
#include <effolkronium/random.hpp>

#include <ncurses.h>

#include "../asset-source/single_asset.h"


using Random = effolkronium::random_static;

namespace sc::play {

SpaceshipFactory::SpaceshipFactory () : logger_(
        CreateLogger("ss_factory")) {

    Config &config = Config::GetInstance();

    max_weight_ = config.GetRange("play.spaceship-factory.max-weight");
    max_fuel_   = config.GetRange("play.spaceship-factory.max-fuel");
    max_hull_   = config.GetRange("play.spaceship-factory.max-hull");
    max_crew_   = config.GetRange("play.spaceship-factory.max-crew");

    initial_money_ = config.GetValue("play.spaceship-factory.init-money",
                                     0);

    max_burn_time_ = config.GetValue(
            "play.spaceship-factory.max-burn-time", 0);


    station_dep_     = sc::GetAsset(Asset::DEPARTURE_STATION);
    station_arrival_ = sc::GetAsset(Asset::ARRIVAL_STATION);
    ship_            = sc::GetFrames(Asset::SHIP_A);
}


std::unique_ptr< Spaceship > SpaceshipFactory::CreateSpaceship () {

    std::string code = " ";

    code[0] = 'a';

    logger_->debug("Spaceship appearance code: {}", code);

    std::unique_ptr< Spaceship > s = std::make_unique< Spaceship >(code);

    double weight = Random::get(max_weight_.min, max_weight_.max);
    int    fuel   = Random::get(max_fuel_.min, max_fuel_.max);
    int    hull   = Random::get(max_hull_.min, max_hull_.max);
    int    crew   = Random::get(max_crew_.min, max_crew_.max);
    int    cost   = ( crew * 300 ) + (int) weight + (int) fuel + hull;

    s->SpaceshipInitConfig(weight, fuel, hull, crew, initial_money_ - cost,
                           cost);
    logger_->debug("Initial money {}", s->GetMoney());

    return s;
}

bool SpaceshipFactory::PrintSpaceship ( WINDOW *window, int y, int x,
                                        bool start_flip, bool can_flip,
                                        bool start_burn ) {

    if ( start_flip ) {
        logger_->debug("Starting flip");
        flipping_   = true;
        flip_speed_ = 1.0 / ( ship_->size() - 1.0 );
        if ( animation_index_ > 0 ) {
            direction_    = -1;
            flip_counter_ = static_cast<int>(( ship_->size() - 1.0 ) /
                                             flip_speed_);
            logger_->debug("Going to forward burn from {}", flip_counter_);
        } else {
            flip_counter_ = 0;
            direction_    = 1;
            logger_->debug("Going to reverse burn from {}", flip_counter_);
        }
    }

    if ( can_flip && flipping_ ) {
        if ( direction_ == 1 ) {
            animation_index_ = std::min(
                    static_cast<int>(( flip_counter_++ ) *
                                     flip_speed_),
                    static_cast<int>(ship_->size() - 1));
            if ( animation_index_ >= ship_->size() - 1 ) {
                animation_index_ = static_cast<int>(ship_->size() - 1);
                flipping_        = false;
                logger_->debug("Done flipping");
            }
        } else {
            animation_index_ = std::max(
                    static_cast<int>(( --flip_counter_ ) *
                                     flip_speed_), 0);

            if ( animation_index_ <= 0 ) {
                animation_index_ = 0;
                flipping_        = false;
                logger_->debug("Done flipping");
            }
        }

    }

    for ( auto &row : ( *ship_ )[animation_index_] ) {
        mvwaddstr(window, y++, x, row.c_str());
    }

    if ( start_burn ) {
        burning_     = true;
        burn_counter = 0;
    }

    if ( can_flip && !flipping_ && burning_ ) {
        burn_counter++;
        y -= 2;
        if ( animation_index_ == 0 ) {
            mvwaddstr(window, y,
                      x + ( *ship_ )[animation_index_][1].length() + 1,
                      "###");
        } else {
            mvwaddstr(window, y, x - 3, "###");
        }

        if ( burn_counter > max_burn_time_ ) {
            burning_ = false;
        }
    }

    return flipping_;
}

int SpaceshipFactory::GetInitialMoney () const {
    return initial_money_;
}

bool
SpaceshipFactory::PrintStationDeparture ( WINDOW *window, int cycle ) {
    int y = station_init_y_;

    for ( auto &s : *station_dep_ ) {

        mvwaddnstr(window, y++, station_init_x_ + cycle, s.c_str(),
                   s.length() - cycle);
    }
    return station_init_x_ + cycle ==
           getmaxx(window) - 8; // weird offset to make things work
}

bool SpaceshipFactory::PrintStationArrival ( WINDOW *window, int cycle ) {
    int y = station_init_y_;


    for ( auto &s : *station_arrival_ ) {
        if (s.length() < cycle) { return true; }
        mvwaddstr(window, y++, 0,
                  s.substr(s.length() - cycle, s.length()).c_str());
    }
    return cycle == 100;
}

}
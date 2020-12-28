//
// Created by Roberts Kalnins on 19/12/2020.
//

#include "spaceship_factory.h"
#include <effolkronium/random.hpp>


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
}

std::unique_ptr< Spaceship > SpaceshipFactory::CreateSpaceship () {

    std::string code = " ";
    char        index;
    do {
        index = effolkronium::random_static::get('a', 'c');
    } while ( ss_used_[index - 'a'] );
    ss_used_[index - 'a'] = true;

    code[0] = index;

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

void SpaceshipFactory::PrintSpaceship ( WINDOW *window, int y, int x,
                                        const std::string &code ) {

    switch ( code[0] ) {
        case 'a':
            for ( auto &row : ss_a_ ) {
                mvwaddstr(window, y++, x, row.c_str());
            }
            break;
        case 'b':
            for ( auto &row : ss_b_ ) {
                mvwaddstr(window, y++, x, row.c_str());
            }
            break;
        case 'c':
            for ( auto &row : ss_c_ ) {
                mvwaddstr(window, y++, x, row.c_str());
            }
            break;
    }
}

int SpaceshipFactory::GetInitialMoney () const {
    return initial_money_;
}

}
//
// Created by Roberts Kalnins on 19/12/2020.
//

#include "spaceship_factory.h"
#include <effolkronium/random.hpp>


using Random = effolkronium::random_static;

namespace sc::play {

SpaceshipFactory::SpaceshipFactory () : logger_(
        CreateLogger("ss_factory")) {
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

    double weight = Random::get(min_max_weight_, max_max_weight_);
    int    fuel   = Random::get(min_max_fuel_, max_max_fuel_);
    int    hull   = Random::get(min_max_hull_, max_max_hull_);
    int    crew   = Random::get(min_max_crew_, max_max_crew_);
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

int SpaceshipFactory::GetInitialMoney () {
    return initial_money_;
}

}
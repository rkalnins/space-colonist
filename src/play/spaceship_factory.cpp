//
// Created by Roberts Kalnins on 19/12/2020.
//

#include "spaceship_factory.h"
#include <effolkronium/random.hpp>


using Random = effolkronium::random_static;

namespace sc::play {

SpaceshipFactory::SpaceshipFactory () {
    logger_ = spdlog::basic_logger_mt("ss_factory",
                                      "logs/space-colonist-log.log");
    logger_->set_level(spdlog::level::debug);
}

std::shared_ptr< Spaceship > SpaceshipFactory::CreateSpaceship () {

    std::string code = " ";
    char        index;
    do {
        index = effolkronium::random_static::get('a', 'c');
    } while ( ss_used_[index - 'a'] );
    ss_used_[index - 'a'] = true;

    code[0] = index;

    logger_->debug("Spaceship appearance code: {}", code);

    std::shared_ptr< Spaceship > s = std::make_shared< Spaceship >(code);

    s->SetMaxWeight(Random::get(min_max_weight_, max_max_weight_));
    s->SetFullFuel(Random::get(min_max_fuel_, max_max_fuel_));
    s->SetFullHull(Random::get(min_max_hull_, max_max_hull_));
    s->SetMaxCrew(Random::get(min_max_crew_, max_max_crew_));
    s->SetCost(( s->GetMaxCrew() * 300 ) + (int) s->GetMaxWeight() +
               s->GetFullFuel() + s->GetFullHull());
    s->SetMoney(initial_money_);

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
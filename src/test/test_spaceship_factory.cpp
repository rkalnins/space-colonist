//
// Created by Roberts Kalnins on 25/12/2020.
//

#include <catch2/catch.hpp>

#include "../play/spaceship.h"
#include "../play/spaceship_factory.h"


using sc::play::Spaceship;
using sc::play::SpaceshipFactory;


TEST_CASE("5.1 Spaceship Factory", "[multi-file:5]") {
    SpaceshipFactory spaceship_factory;

    const int initial_money = 3000;

    const int min_max_weight { 1500 };
    const int max_max_weight { 1500 };

    const int min_max_fuel { 5 };
    const int max_max_fuel { 10 };

    const int min_max_hull { 150 };
    const int max_max_hull { 150 };

    const int min_max_crew { 2 };
    const int max_max_crew { 3 };

    SECTION("5.1.1 Getters") {
        REQUIRE(spaceship_factory.GetInitialMoney() == initial_money);
        REQUIRE_FALSE(spaceship_factory.GetInitialMoney() == 2999);
    }

    SECTION("5.1.2 Spaceship creation") {
        std::unique_ptr< Spaceship > s = spaceship_factory.CreateSpaceship();

        REQUIRE(s->GetMoney() ==
                spaceship_factory.GetInitialMoney() - s->GetCost());
        REQUIRE(( min_max_weight <= s->GetMaxWeight() &&
                  s->GetMaxWeight() <= max_max_weight ));
        REQUIRE(( min_max_crew <= s->GetMaxCrew() &&
                  s->GetMaxCrew() <= max_max_crew ));
        REQUIRE(( min_max_fuel <= s->GetFullFuel() &&
                  s->GetFullFuel() <= max_max_fuel ));
        REQUIRE(( min_max_hull <= s->GetFullHull() &&
                  s->GetFullHull() <= max_max_hull ));
    }


}
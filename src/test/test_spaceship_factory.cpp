//
// Created by Roberts Kalnins on 25/12/2020.
//

#include <catch2/catch.hpp>

#include "../play/spaceship.h"
#include "../play/spaceship_factory.h"


using sc::play::Spaceship;
using sc::play::SpaceshipFactory;
using sc::Range;


TEST_CASE("5.1 Spaceship Factory", "[multi-file:5]") {
    SpaceshipFactory spaceship_factory;

    const int initial_money = 3000;

    const Range max_weight { 1500, 1500 };
    const Range max_fuel { 5, 10 };
    const Range max_hull { 150, 150 };
    const Range max_crew { 2, 3 };


    SECTION("5.1.1 Getters") {
        REQUIRE(spaceship_factory.GetInitialMoney() == initial_money);
        REQUIRE_FALSE(spaceship_factory.GetInitialMoney() == 2999);
    }

    SECTION("5.1.2 Spaceship creation") {
        std::unique_ptr< Spaceship > s = spaceship_factory.CreateSpaceship();

        REQUIRE(s->GetMoney() ==
                spaceship_factory.GetInitialMoney() - s->GetCost());
        REQUIRE(( max_weight.min <= s->GetMaxWeight() &&
                  s->GetMaxWeight() <= max_weight.max ));
        REQUIRE(( max_crew.min <= s->GetMaxCrew() &&
                  s->GetMaxCrew() <= max_crew.max ));
        REQUIRE(( max_fuel.min <= s->GetFullFuel() &&
                  s->GetFullFuel() <= max_fuel.max ));
        REQUIRE(( max_hull.min <= s->GetFullHull() &&
                  s->GetFullHull() <= max_hull.max ));
    }


}
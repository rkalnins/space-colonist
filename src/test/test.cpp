//
// Created by Roberts Kalnins on 17/12/2020.
//


#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "../play/spaceship_handler.h"


unsigned int Factorial ( unsigned int number ) {
    return number <= 1 ? number : Factorial(number - 1) * number;
}

TEST_CASE("Basic Spaceship", "[ss]") {
    using namespace sc::play;

    Spaceship spaceship((std::string &) "a");

    REQUIRE(spaceship.GetItems().empty());
}

TEST_CASE("Use all fuel", "[ss]") {
    using namespace sc::play;

    Spaceship spaceship((std::string &) "a");
    spaceship.UseFuel(spaceship.GetFuel());

    REQUIRE(spaceship.GetFuel() == 0);
}

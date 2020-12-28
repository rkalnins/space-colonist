//
// Created by Roberts Kalnins on 24/12/2020.
//

#include <catch2/catch.hpp>

#include "../play/flying_debris.h"


using sc::play::FlyingDebris;

TEST_CASE("3.1 Flying Debris basic tests ", "[multi-file:3]") {
    const int    end_pt = 10;
    const double speed  = 0.5;

    FlyingDebris flying_debris(speed, end_pt);

    // end one short
    for ( int i = 0; i < end_pt * 2 - 1; ++i ) {
        flying_debris.Move();
    }

    REQUIRE_FALSE(flying_debris.IsDone());

    flying_debris.Move();
    REQUIRE(flying_debris.IsDone());

    flying_debris.Move();
    REQUIRE(flying_debris.IsDone());
}

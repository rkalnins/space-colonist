//
// Created by Roberts Kalnins on 22/12/2020.
//

#include <catch2/catch.hpp>
#include "flying_debris.h"


namespace sc::play {


FlyingDebris::FlyingDebris ( WINDOW *main, double travel_speed,
                             int end_pt )
        : main_(main), travel_speed_(travel_speed), end_pt_(end_pt) {
    std::vector< char > appearances = config_.GetList< char >(
            "play.debris.style");

    static const Range y_bounds = config_.GetRange("play.debris.y-range");

    appearance = *Random::get(appearances);

    x_ = 0;
    y_ = Random::get< int >(y_bounds.min, y_bounds.max);
}

FlyingDebris::FlyingDebris ( double travel_speed, int end_pt )
        : FlyingDebris(nullptr, travel_speed, end_pt) {}

FlyingDebris::FlyingDebris ( WINDOW *main )
        : FlyingDebris(main, 0, 0) {

    static const std::vector< double > choices = config_.GetList< double >(
            "play.debris.speed-options");

    static const Range end_pts = config_.GetRange(
            "play.debris.end-range");

    travel_speed_ = *Random::get(choices);
    end_pt_       = Random::get(end_pts.min, end_pts.max);


}

bool FlyingDebris::IsDone () const {
    return ( x_ >= end_pt_ );
}

void FlyingDebris::Move () {
    x_ = static_cast<int>(++( counter_ ) * travel_speed_);
}

void FlyingDebris::Render () {
    mvwaddch(main_, y_, x_, appearance);
}


}

//
// Created by Roberts Kalnins on 22/12/2020.
//

#include "flying_debris.h"


namespace sc::play {

FlyingDebris::FlyingDebris ( WINDOW *main, double travel_speed,
                             int end_pt )
        : main_(main), travel_speed_(travel_speed), end_pt_(end_pt) {

    appearance = Random::get({ '*', '.', '`', 'o', 'x' });
    x_         = 0;
    y_         = Random::get< int >(min_y_, max_y_);
}

FlyingDebris::FlyingDebris ( WINDOW *main ) : main_(main) {
    appearance    = Random::get({ '*', '.', '`', 'o', 'x' });
    x_            = 0;
    y_            = Random::get< int >(min_y_, max_y_);
    travel_speed_ = Random::get({ 0.5, 0.25, 0.17 });
    end_pt_       = Random::get< int >(90, 100);
}

bool FlyingDebris::IsDone () const {
    return ( x_ >= end_pt_ );
}

FlyingDebris::FlyingDebris () : main_(nullptr), x_(0), y_(0),
                                travel_speed_(0),
                                appearance(' '),
                                end_pt_(0) {}

void FlyingDebris::Move () {
    x_ = static_cast<int>(++( counter_ ) * travel_speed_);
}

void FlyingDebris::Render () {
    mvwaddch(main_, y_, x_, appearance);
}


}

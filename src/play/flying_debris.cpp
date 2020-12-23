//
// Created by Roberts Kalnins on 22/12/2020.
//

#include "flying_debris.h"


namespace sc::play {

FlyingDebris::FlyingDebris ( WINDOW *main ) : main_(main) {
    appearance   = Random::get({ '*', '.', '`', 'o', 'x' });
    x            = 0;
    y            = Random::get< int >(min_y_, max_y_);
    travel_speed = Random::get({ 2, 4, 6 });
    end_pt       = Random::get< int >(90, 100);
}

bool FlyingDebris::IsDone () const {
    return ( x > end_pt );
}

FlyingDebris::FlyingDebris () : main_(nullptr), x(0), y(0),
                                travel_speed(0),
                                appearance(' '),
                                end_pt(0) {}

void FlyingDebris::Move () {
    x = ++( counter ) / travel_speed;
    mvwaddch(main_, y, x, appearance);
}


}

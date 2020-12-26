//
// Created by Roberts Kalnins on 22/12/2020.
//

#pragma once

#include <effolkronium/random.hpp>
#include <ncurses.h>


namespace sc::play {

using Random = effolkronium::random_static;


class FlyingDebris {
  public:
    explicit FlyingDebris ( WINDOW *main );

    FlyingDebris ();

    FlyingDebris ( WINDOW *main, double travel_speed, int end_pt );

    void Move ();

    [[nodiscard]] bool IsDone () const;

    void Render ();

  private:
    WINDOW *main_;

    const int max_y_ { 40 };
    const int min_y_ { 25 };


    int    x_ {};
    int    y_ {};
    double travel_speed_;
    int    end_pt_ {};
    int    counter_ { 0 };
    char   appearance {};
};

}





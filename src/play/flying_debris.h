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

    void Move ();

    [[nodiscard]] bool IsDone () const;

  private:
    WINDOW *main_;

    const int max_y_ { 40 };
    const int min_y_ { 25 };


    int  x;
    int  y {};
    int  travel_speed {};
    int  end_pt {};
    int  counter { 0 };
    char appearance {};
};

}





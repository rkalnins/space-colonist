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
    FlyingDebris ( WINDOW *main, int min_y, int max_y );

    FlyingDebris ();

    void Move ();

    [[nodiscard]] bool IsDone () const;

  private:
    WINDOW *main_;

    int  x;
    int  y {};
    int  travel_speed {};
    int  end_pt {};
    int  counter { 0 };
    char appearance {};
};

}





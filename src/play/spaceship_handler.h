//
// Created by Roberts Kalnins on 17/12/2020.
//

#pragma once

#include <queue>

#include "spaceship.h"
#include "../input_listener.h"
#include "../logging/logging.h"
#include "spaceship_factory.h"


namespace sc::play {

struct Checkbox {
    int  y { 0 };
    int  left_x { 0 };
    int  right_x { 0 };
    bool checked { false };
};


class SpaceshipHandler {
  public:


    explicit SpaceshipHandler (
            shared_spaceship_factory_t spaceship_factory );

  public:
    void
    SetSpaceship ( const shared_spaceship_t &spaceship );

    [[nodiscard]] const shared_spaceship_t &
    GetSpaceship () const;

    void SetCrew ( const std::vector< CrewMember > &crew_choices,
                   std::vector< Checkbox > &selected );

    void PrintSpaceship ( WINDOW *main );

    void MoveSpaceship ();

    void BoundSpaceshipPosition ();

  private:

    logger_t logger_;

    shared_spaceship_t         spaceship_ { nullptr };
    shared_spaceship_factory_t spaceship_factory_ { nullptr };

    double ss_mvmt_x_prob_ { 0.75 };
    double ss_mvmt_y_prob_ { 0.75 };

    const int ss_max_y_ { 37 };
    const int ss_min_y_ { 24 };
    const int ss_max_x_ { 55 };
    const int ss_min_x_ { 45 };

    int ss_pos_y_ { 25 };
    int ss_pos_x_ { 50 };
};


using shared_spaceship_handler_t = std::shared_ptr< SpaceshipHandler >;

}





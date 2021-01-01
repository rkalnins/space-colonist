//
// Created by Roberts Kalnins on 17/12/2020.
//

#pragma once

#include <queue>

#include "spaceship.h"
#include "../input_listener.h"
#include "../logging/logging.h"
#include "spaceship_factory.h"
#include "../config/config.h"


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

    bool PrintDeparture ( WINDOW *main );

    bool PrintArrival ( WINDOW *main );

    void StartFlip ();

    void Burn ();

  private:

    logger_t logger_;

    shared_spaceship_t         spaceship_ { nullptr };
    shared_spaceship_factory_t spaceship_factory_ { nullptr };

    int    ss_pos_y_;
    int    ss_pos_x_;
    int    departure_cycle_ { 0 };
    double departure_speed_ = 1.0 / 10.0;
    int    arrival_cycle_ { 0 };
    double arrival_speed_ = 1.0 / 10.0;

    bool start_flip_ { false };

    bool burn_ { false };
};


using shared_spaceship_handler_t = std::shared_ptr< SpaceshipHandler >;

}





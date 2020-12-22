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
    int  y;
    int  left_x;
    int  right_x;
    bool checked;
};


class SpaceshipHandler {
  public:
    explicit SpaceshipHandler (std::shared_ptr<SpaceshipFactory> spaceship_factory);

  public:
    void
    SetSpaceship ( const std::shared_ptr< play::Spaceship > &spaceship );

    [[nodiscard]] const std::shared_ptr< play::Spaceship > &
    GetSpaceship () const;

    void SetCrew ( const std::vector< CrewMember > &crew_choices,
                   std::vector< Checkbox > &selected );

    void PrintSpaceship ( WINDOW *main, int y, int x );

  private:

    logger_t logger_;

    std::shared_ptr< Spaceship > spaceship_ { nullptr };
    std::shared_ptr< SpaceshipFactory > spaceship_factory_ { nullptr };
};

}





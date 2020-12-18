//
// Created by Roberts Kalnins on 17/12/2020.
//

#pragma once

#include "../loop_control/task.h"
#include "objects/spaceship.h"
#include "../input_listener.h"
#include "../logging/logging.h"


namespace sc::play {

class SpaceshipHandler : public Task {
  public:
    SpaceshipHandler ( const std::string &name, TaskType taskType,
                       std::shared_ptr< InputListener > listener,
                       WINDOW *main );

  public:
    void Init () override;

    GameState OnLoop () override;

    bool IsFinished () override;

  private:
    play::Spaceship spaceship_ {};

    std::shared_ptr< InputListener > listener_ { nullptr };

    WINDOW *main_;
    MEVENT mouse_event_ {};

    logger_t logger_;

    const int spaceship_display_x_ = 10;
    const int spaceship_display_y_ = 4;

    double fuel_use_ { 0.01 };

};

}





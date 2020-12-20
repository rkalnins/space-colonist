//
// Created by Roberts Kalnins on 20/12/2020.
//

#pragma once

#include "../loop_control/task.h"
#include "../logging/logging.h"
#include "spaceship_handler.h"


namespace sc::play {

class RunningUI : public Task {

  public:

    RunningUI ( const std::string &name, TaskType taskType,
              std::shared_ptr< SpaceshipHandler > spaceship_handler,
              std::shared_ptr< InputListener > listener,
              WINDOW *main );

    void Init () override;

    GameState OnLoop ( GameState state ) override;

  private:

    std::shared_ptr< SpaceshipHandler > spaceship_handler_;

    std::shared_ptr< InputListener > listener_;

    WINDOW *main_;

};

}





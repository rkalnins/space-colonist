//
// Created by Roberts Kalnins on 18/12/2020.
//

#pragma once

#include <ncurses.h>

#include "items/item.h"
#include "objects/crew_member.h"
#include "objects/spaceship.h"
#include "../input_listener.h"
#include "../logging/logging.h"
#include "../loop_control/task.h"


namespace sc::play {

class GameUI : public Task {


  public:

    GameUI ( const std::string &name, TaskType taskType,
             std::shared_ptr< Spaceship > spaceship,
             std::shared_ptr< InputListener > listener,
             WINDOW *main );

    void Init () override;

    GameState OnLoop () override;


  private:
    logger_t logger_;

    std::shared_ptr< Spaceship > spaceship_ { nullptr };

    std::shared_ptr< InputListener > listener_ { nullptr };

    WINDOW *main_;
    MEVENT mouse_event_ {};

    const int ui_init_y_ = 20;
    const int ui_init_x_ = 4;
};

}



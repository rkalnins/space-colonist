//
// Created by Roberts Kalnins on 16/12/2020.
//

#pragma once

#include <map>
#include "loop_control/task.h"
#include "tasks/running_input.h"
#include "tasks/pause_input.h"
#include "play/spaceship_handler.h"


namespace sc {

class GameTasks {
  public:
    GameTasks ( WINDOW *main,
                std::shared_ptr< InputListener > &listener ) : main_(
            main) {
        running_input_     = std::make_shared< RunningInput >(
                running_input_name_, TaskType::RUNNING, listener, main);
        pause_input_       = std::make_shared< PauseInput >(
                pause_input_name_,
                TaskType::PAUSE,
                listener);
        spaceship_handler_ = std::make_shared< play::SpaceshipHandler >(spaceship_handler_name_, TaskType::RUNNING, listener, main);
    }

  public:

    std::shared_ptr< RunningInput >           running_input_;
    std::shared_ptr< PauseInput >             pause_input_;
    std::shared_ptr< play::SpaceshipHandler > spaceship_handler_;


    [[nodiscard]] WINDOW *GetMain () const {
        return main_;
    }

  private:
    WINDOW *main_;

  private:

    const std::string running_input_name_ = "process_running_input";
    const std::string pause_input_name_   = "process_pause_input";
    const std::string spaceship_handler_name_   = "ss_handler";
};

}





//
// Created by Roberts Kalnins on 16/12/2020.
//

#pragma once

#include <map>

#include "loop_control/task.h"
#include "play/spaceship_handler.h"
#include "play/setup_ui.h"


namespace sc {

class GameTasks {
  public:
    GameTasks ( WINDOW *main,
                std::shared_ptr< InputListener > &listener )
            : main_(main) {


        spaceship_handler_ = std::make_shared< play::SpaceshipHandler >(
                spaceship_handler_name_, TaskType::RUNNING, listener,
                main);

        setup_ui_ = std::make_shared< play::SetupUI >(ui_name_, TaskType::RUNNING,
                                                      spaceship_handler_, listener, main);
    }

  public:

    std::shared_ptr< play::SpaceshipHandler > spaceship_handler_ {
            nullptr
    };

    std::shared_ptr< play::SetupUI > setup_ui_ { nullptr };


    [[nodiscard]] WINDOW *GetMain () const {
        return main_;
    }

  private:
    WINDOW *main_;

  private:

    const std::string running_input_name_     = "process_running_input";
    const std::string pause_input_name_       = "process_pause_input";
    const std::string spaceship_handler_name_ = "ss_handler";
    const std::string ui_name_                = "ui";
};

}





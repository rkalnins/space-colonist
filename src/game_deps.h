//
// Created by Roberts Kalnins on 16/12/2020.
//

#pragma once

#include <map>

#include "loop_control/task.h"
#include "play/spaceship_handler.h"
#include "play/spaceship_factory.h"
#include "play/ui/setup_ui.h"
#include "play/ui/running_ui.h"
#include "play/ui/hud_display.h"
#include "play/nav_control_manager.h"


namespace sc {

class GameDependencies {
  public:

    GameDependencies ( WINDOW *main,
                       std::shared_ptr< InputListener > &listener )
            : main_(main) {

        spaceship_factory_ = std::make_shared< play::SpaceshipFactory >();

        spaceship_handler_ = std::make_shared< play::SpaceshipHandler >(
                spaceship_factory_);

        nav_manager_ = std::make_shared< play::NavigationControlManager >(
                nav_manager_name_, TaskType::RUNNING,
                spaceship_handler_);

        hud_display_ = std::make_shared< play::HUDDisplay >(hud_ui_name_,
                                                            TaskType::RUNNING,
                                                            spaceship_handler_,
                                                            nav_manager_,
                                                            listener,
                                                            main);


        setup_ui_ = std::make_shared< play::SetupUI >(setup_ui_name_,
                                                      TaskType::RUNNING,
                                                      spaceship_handler_,
                                                      nav_manager_,
                                                      listener,
                                                      spaceship_factory_,
                                                      main);

        running_ui_ = std::make_shared< play::RunningUI >(
                running_ui_name_,
                TaskType::RUNNING,
                spaceship_handler_,
                nav_manager_,
                listener,
                main);

    }

  public:

    play::shared_spaceship_handler_t spaceship_handler_ { nullptr };
    play::shared_nav_manager_t       nav_manager_ { nullptr };

    play::shared_spaceship_factory_t spaceship_factory_ {
            nullptr
    };

    std::shared_ptr< play::SetupUI >    setup_ui_ {
            nullptr
    };
    std::shared_ptr< play::RunningUI >  running_ui_ {
            nullptr
    };
    std::shared_ptr< play::HUDDisplay > hud_display_ {
            nullptr
    };


    [[nodiscard]] WINDOW *GetMain () const {
        return main_;
    }

  private:
    WINDOW *main_;

  private:

    const std::string setup_ui_name_    = "setup_ui";
    const std::string running_ui_name_  = "running_ui";
    const std::string hud_ui_name_      = "hud_ui";
    const std::string nav_manager_name_ = "nav_manager";
};

}





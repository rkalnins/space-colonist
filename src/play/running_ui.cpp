//
// Created by Roberts Kalnins on 20/12/2020.
//

#include "running_ui.h"

#include <utility>


namespace sc::play {


RunningUI::RunningUI ( const std::string &name, TaskType taskType,
                       std::shared_ptr< SpaceshipHandler > spaceship_handler,
                       std::shared_ptr< InputListener > listener,
                       WINDOW *main )
        : Task(name, taskType),
          spaceship_handler_(std::move(spaceship_handler)),
          listener_(std::move(listener)),
          main_(main) {

}

void RunningUI::Init () {

}

GameState RunningUI::OnLoop ( GameState state ) {
    return GameState::SETUP;
}


}
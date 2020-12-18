//
// Created by Roberts Kalnins on 18/12/2020.
//

#include "game_ui.h"

#include <utility>


namespace sc::play {


void GameUI::Init () {

}

GameState GameUI::OnLoop () {
    return GameState::RUNNING;
}

GameUI::GameUI ( const std::string &name, TaskType taskType,
                 std::shared_ptr< Spaceship > spaceship,
                 std::shared_ptr< InputListener > listener,
                 WINDOW *main )
        : Task(name, taskType),
          spaceship_(std::move(spaceship)),
          listener_(std::move(listener)),
          main_(main) {}
}
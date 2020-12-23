//
// Created by Roberts Kalnins on 15/12/2020.
//

#include "../game.h"
#include <thread>
#include "task_pool.h"


namespace sc {

[[maybe_unused]] TaskPool::TaskPool ( const std::string &name ) {
    logger_ = CreateLogger(name);
}

void TaskPool::AddTask ( const std::shared_ptr< Task > &task ) {
    pool_.push_back(task);
    task->Init();
}

GameState TaskPool::Loop ( GameState state ) {
    new_state_ = state;
    for ( auto &f : pool_ ) {
        // if state change occurs, end
        if ( !f->IsFinished() && new_state_ == state ) {
            new_state_ = f->OnLoop(state);

            if ( new_state_ != state ) {
                logger_->debug("{} changed state from {} to {}",
                               f->GetName(), state, new_state_);
            }
        }

        if ( f->Exit()) {
            return GameState::EXITING;
        }
    }

    return new_state_;
}

void TaskPool::SetNewState ( GameState newState ) {
    new_state_ = newState;
}

}
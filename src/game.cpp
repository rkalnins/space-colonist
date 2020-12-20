//
// Created by Roberts Kalnins on 15/12/2020.
//

#include <chrono>
#include <thread>
#include <functional>
#include <utility>

#include "logging/logging.h"
#include "loop_control/watchdog.h"
#include "game.h"


namespace sc {

Game::Game ( std::shared_ptr< GameTasks > tasks,
             std::shared_ptr< InputListener > listener )
        : tasks_(std::move(tasks)),
          input_listener_(std::move(listener)) {
    logger_ = spdlog::basic_logger_mt("game",
                                      "logs/space-colonist-log.log");
    logger_->set_level(spdlog::level::debug);

    main_ = tasks_->GetMain();
    box(main_, 0, 0);

    mousemask(ALL_MOUSE_EVENTS, nullptr);
}

Game::~Game () = default;

void Game::LoopController () {
    using namespace std::chrono;
    auto next = steady_clock::now() + freq60_t { 1 };

    GameState new_state { GameState::SETUP };
    logger_->debug(new_state);


    while ( !done_ ) {

        std::this_thread::sleep_until(next);
        next += freq60_t { 1 };

        werase(main_);
        // loop control
        if ( state_ == GameState::SETUP ) {
            new_state = setup_tasks_.Loop(state_);
        } else if ( state_ == GameState::RUNNING ) {
            new_state = running_tasks_.Loop(state_);
        }

        if ( new_state != state_ ) {
            logger_->debug("{} {}", new_state, state_);
            switch ( new_state ) {
                case GameState::SETUP:
                    break;
                case GameState::RUNNING:
                    OnRun();
                    break;
                case GameState::EXITING:
                    logger_->debug("Preparing to exit");
                    OnExit();
                    return;
            }
        }

        box(main_, 0, 0);
        refresh();
        wrefresh(main_);
        input_listener_->ResetCh();
        state_ = new_state;

        running_tasks_.SetNewState(state_);
        setup_tasks_.SetNewState(state_);

        watchdog_.Notify();
    }
}

void Game::Init () {
    setup_tasks_.AddTask(tasks_->setup_ui_);
    setup_tasks_.AddTask(tasks_->spaceship_handler_);

    running_tasks_.AddTask(tasks_->spaceship_handler_);

    LoopController();
}

void Game::OnExit () {
    for ( auto &f : setup_tasks_ ) {
        f->OnExit();
    }

    for ( auto &f : running_tasks_ ) {
        f->OnExit();
    }
}

void Game::OnRun () {
    for ( auto &f : setup_tasks_ ) {
        f->OnRun();
    }

    for ( auto &f : running_tasks_ ) {
        f->OnRun();
    }
}

}

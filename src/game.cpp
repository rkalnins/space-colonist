//
// Created by Roberts Kalnins on 15/12/2020.
//

#include <chrono>
#include <thread>
#include <ncurses.h>
#include <menu.h>
#include <functional>
#include <sstream>
#include <utility>

#include "logging/logging.h"
#include "game.h"


namespace sc {

Game::Game ( std::shared_ptr< GameTasks > tasks,
             std::shared_ptr< InputListener > listener ) : tasks_(
        std::move(tasks)), input_listener_(std::move(listener)) {
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

    GameState new_state { GameState::RUNNING };

    while ( !done_ ) {
        std::this_thread::sleep_until(next);
        next += freq60_t { 1 };

        // loop control
        if ( state_ == GameState::RUNNING ) {
            for ( auto &f : running_tasks_ ) {
                if ( !f->IsFinished() && new_state == state_ ) {
                    new_state = f->OnLoop();
                }

                if ( f->Exit()) {
                    OnExit();
                    return;
                }
            }
            wrefresh(main_);
        } else if ( state_ == GameState::PAUSED ) {
            for ( auto &f : paused_tasks_ ) {
                if ( !f->IsFinished() && new_state == state_ ) {
                    new_state = f->OnLoop();
                }

                if ( f->Exit()) {
                    OnExit();
                    return;
                }
            }
        }

        for ( auto &f : always_tasks_ ) {
            if ( !f->IsFinished() && new_state == state_ ) {
                new_state = f->OnLoop();
            }

            if ( f->Exit()) {
                OnExit();
                return;
            }
        }
        refresh();

        if ( new_state != state_ ) {

            switch ( new_state ) {
                case GameState::RUNNING:
                    OnRun();
                    break;
                case GameState::PAUSED:
                    OnPause();
                    break;
                case GameState::EXITING:
                    return;
            }
        }

        input_listener_->ResetCh();
        state_ = new_state;
    }
}

void Game::Init () {
    running_tasks_.AddTask(tasks_->running_input_);
    running_tasks_.AddTask(tasks_->spaceship_handler_);
    paused_tasks_.AddTask(tasks_->pause_input_);
    LoopController();
}

void Game::OnExit () {
    for ( auto &f : paused_tasks_ ) {
        f->OnExit();
    }

    for ( auto &f : running_tasks_ ) {
        f->OnExit();
    }
}

void Game::OnRun () {
    for ( auto &f : paused_tasks_ ) {
        f->OnRun();
    }

    for ( auto &f : running_tasks_ ) {
        f->OnRun();
    }
}

void Game::OnPause () {
    for ( auto &f : running_tasks_ ) {
        f->OnPause();
    }

    for ( auto &f : paused_tasks_ ) {
        f->OnPause();
    }
}

}

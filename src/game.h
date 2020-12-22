//
// Created by Roberts Kalnins on 15/12/2020.
//

#pragma once

#include <chrono>
#include <memory>
#include <thread>

#include <atomic>
#include <thread>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <ncurses.h>

#include "logging/logging.h"
#include "loop_control/task_pool.h"
#include "game_deps.h"
#include "game_state.h"

#include "play/spaceship.h"
#include "play/spaceship_handler.h"
#include "loop_control/watchdog.h"

#include "input_listener.h"


namespace sc {

using freq60_t = std::chrono::duration< std::chrono::steady_clock::rep, std::ratio< 1, 60>>;


class Game {
  public:
    explicit Game ( std::shared_ptr< GameDependencies > deps,
                    std::shared_ptr< InputListener > listener );

    virtual ~Game ();

    void Init ();

  private:
    void LoopController ();

    void OnRun ();

    void OnExit ();

  private:

    std::shared_ptr< GameDependencies > deps_ { nullptr };
    std::shared_ptr< InputListener >    input_listener_ { nullptr };

    Watchdog watchdog_;

    std::atomic< bool > done_ { false };

    logger_t logger_ { nullptr };

    TaskPool setup_tasks_ { "setup_tpool" };
    TaskPool running_tasks_ { "running_tpool" };

    GameState state_ { GameState::SETUP };

    WINDOW *main_;
};

}



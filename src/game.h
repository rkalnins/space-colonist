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
#include "game_tasks.h"
#include "game_state.h"

#include "play/objects/spaceship.h"
#include "play/spaceship_handler.h"

#include "input_listener.h"


namespace sc {

using freq60_t = std::chrono::duration< std::chrono::steady_clock::rep, std::ratio< 1, 60>>;


class Game {
  public:
    explicit Game ( std::shared_ptr< GameTasks > tasks, std::shared_ptr<InputListener> listener );

    virtual ~Game ();

    void Init ();

  private:
    void LoopController ();

    void OnPause ();

    void OnRun ();

  private:

    std::shared_ptr< GameTasks > tasks_;
    std::shared_ptr< InputListener > input_listener_;

    std::atomic< bool > done_ { false };



    logger_t logger_ { nullptr };

    TaskPool paused_tasks_ { "paused_tpool" };
    TaskPool running_tasks_ { "running_tpool" };
    TaskPool always_tasks_ { "always_tpool" };

    GameState state_ { GameState::RUNNING };

    WINDOW *main_;
};

}



//
// Created by Roberts Kalnins on 15/12/2020.
//

#pragma once

#include <chrono>
#include <memory>
#include <thread>
#include <functional>
#include <vector>
#include <atomic>
#include <thread>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <ncurses.h>
#include <menu.h>

#include "windows/pause_menu.h"
#include "logging.h"


namespace sc {


enum class GameState {
    RUNNING, PAUSED
};

enum class TaskType {
    RUNNING_INPUT, PAUSE_INPUT
};

using freq60_t = std::chrono::duration< std::chrono::steady_clock::rep, std::ratio< 1, 60>>;
using task_t = std::pair< TaskType, std::function< void ()>>;
using task_pool_t = std::vector< task_t >;


class Game {
  public:
    Game ();

    virtual ~Game ();

    void Init ();

  private:
    void LoopController ();

    void ProcessRunningInput ();

    void ProcessPauseInput ();

    void InputListener ();

    void AddTask ( task_pool_t &tasks, TaskType task_type );

    static void RemoveTask ( task_pool_t &tasks, TaskType task_type );

  private:

    std::unique_ptr< PauseMenu > pause_menu_ { nullptr };

    std::atomic< bool > done_ { false };

    logger_t logger_ { nullptr };

    MEVENT mouse_event {};

    task_pool_t paused_tasks_;
    task_pool_t running_tasks_;
    task_pool_t always_tasks_;

    GameState state_ { GameState::RUNNING };

    WINDOW *main_;

    std::thread input_listener;

    std::mutex         ch_mtx_;
    std::atomic< int > ch_{};
};

}



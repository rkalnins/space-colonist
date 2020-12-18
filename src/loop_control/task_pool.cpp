//
// Created by Roberts Kalnins on 15/12/2020.
//

#include "../game.h"
#include "../logging/logging.h"
#include <menu.h>
#include <ncurses.h>
#include <thread>
#include <chrono>
#include "task_pool.h"


namespace sc {

[[maybe_unused]] TaskPool::TaskPool (const std::string& name) {
    logger_ = spdlog::basic_logger_mt(name,
                                      "logs/space-colonist-log.log");
    logger_->set_level(spdlog::level::debug);
}

void TaskPool::AddTask ( const std::shared_ptr< Task > &task ) {
    pool_.push_back(task);
    task->Init();
}

}
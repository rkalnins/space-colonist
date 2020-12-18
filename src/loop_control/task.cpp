//
// Created by Roberts Kalnins on 16/12/2020.
//

#include "task.h"

#include <utility>


sc::Task::Task ( std::string name, sc::TaskType taskType ) : name_(std::move(
        name)), task_type_(taskType) {}

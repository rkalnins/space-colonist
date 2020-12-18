//
// Created by Roberts Kalnins on 16/12/2020.
//

#pragma once

#include <string>

#include "../game_state.h"

namespace sc {

enum class TaskType {
    RUNNING, PAUSE
};

class Task {

  public:
    Task ( std::string name, TaskType taskType );

    virtual void Init() = 0;

    virtual GameState OnLoop () = 0;

    virtual void OnPause () {};

    virtual void OnRun () {};

    virtual void OnExit () {};

    virtual bool IsFinished() { return false; };

    virtual bool Exit() { return false; }

    std::string &GetName() {
        return name_;
    }

  protected:
    std::string name_;
    TaskType    task_type_ {};


};

}





//
// Created by Roberts Kalnins on 16/12/2020.
//

#ifndef SC_RUNNING_INPUT_H
#define SC_RUNNING_INPUT_H

#include <utility>

#include "../loop_control/task.h"
#include "../logging/logging.h"

#include "../input_listener.h"
#include "pause_input.h"
#include "../game_state.h"


namespace sc {
class RunningInput : public Task {
  public:
    RunningInput ( const std::string &name, TaskType taskType,
                   std::shared_ptr< InputListener > listener,
                   WINDOW *main )
            : Task(name, taskType), listener_(std::move(listener)),
              main_(main) {

        logger_ = spdlog::basic_logger_mt("running_input",
                                          "logs/space-colonist-log.log");
        logger_->set_level(spdlog::level::debug);
    }

  protected:
    void Init () override {
        logger_->debug("Running init");
        done_ = false;
    }

    GameState OnLoop () override {
        GameState ret_state;

        switch ( listener_->GetCh()) {
            case KEY_F(1):
            case 'p': {
                logger_->debug("returning new state pause");
                ret_state = GameState::PAUSED;
                break;
            }
            case '\n': {
                done_ = true;
                logger_->debug("Return pressed. Game done.");
                ret_state = GameState::EXITING;
                break;
            }
            default: {
                ret_state = GameState::RUNNING;
                break;
            }
        }

        return ret_state;
    }

    bool Exit () override {
        return done_;
    }

  private:
    std::shared_ptr< InputListener > listener_ { nullptr };

    logger_t logger_;

    WINDOW *main_;

    bool done_ { false };


};

}
#endif //SC_RUNNING_INPUT_H

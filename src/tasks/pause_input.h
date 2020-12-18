//
// Created by Roberts Kalnins on 16/12/2020.
//

#ifndef SC_PAUSE_INPUT_H
#define SC_PAUSE_INPUT_H

#include <utility>

#include "../loop_control/task.h"
#include "../logging/logging.h"
#include "../input_listener.h"
#include "pause_input.h"
#include "../game_state.h"


namespace sc {
class PauseInput : public Task {
  public:
    PauseInput ( const std::string &name, TaskType taskType,
                 std::shared_ptr< InputListener > listener ) : Task(
            name, taskType), listener_(std::move(listener)) {
        pause_menu_ = std::make_unique< PauseMenu >();

        logger_ = spdlog::basic_logger_mt("pause_input",
                                          "logs/space-colonist-log.log");
        logger_->set_level(spdlog::level::debug);
    }

  protected:
    void Init () override {
        logger_->debug("Pause init");
        done_ = false;
        pause_menu_->Show();
    }

    GameState OnLoop () override {
        GameState ret_state;

        switch ( listener_->GetCh()) {
            case KEY_DOWN: {
                pause_menu_->Navigate(REQ_DOWN_ITEM);
                ret_state = GameState::PAUSED;
                break;
            }
            case KEY_UP: {
                pause_menu_->Navigate(REQ_UP_ITEM);
                ret_state = GameState::PAUSED;
                break;
            }
            case 10: {
                int item = pause_menu_->Select();

                if ( item == 3 ) {
                    done_     = true;
                    ret_state = GameState::EXITING;
                    logger_->debug("Exiting from menu, and from game");
                    pause_menu_->Hide();
                    break;
                }

                // fallthrough
            }
            case KEY_F(1):
            case 'p': {
                pause_menu_->Hide();
                logger_->debug("Unpausing");
                ret_state = GameState::RUNNING;
                break;
            }
            default: {
                ret_state = GameState::PAUSED;
                break;
            }
        }

        listener_->ResetCh();
        pause_menu_->Refresh();
        return ret_state;
    }

    bool IsFinished () override {
        return done_;
    }

    bool Exit () override {
        return done_;
    }

  private:
    std::shared_ptr< InputListener > listener_ { nullptr };
    std::unique_ptr< PauseMenu >     pause_menu_ { nullptr };
    logger_t                         logger_;

    bool done_ { false };

};

}
#endif //SC_PAUSE_INPUT_H

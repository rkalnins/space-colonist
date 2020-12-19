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
                 std::shared_ptr< InputListener > listener )
            : Task(name, taskType),
              listener_(std::move(listener)) {
        pause_menu_ = std::make_unique< PauseMenu >();

        logger_ = spdlog::basic_logger_mt("pause_input",
                                          "logs/space-colonist-log.log");
        logger_->set_level(spdlog::level::debug);
    }

  protected:
    void Init () override {
        logger_->debug("Pause init");
    }

    void OnPause () override {
        logger_->debug("Pausing");
        pause_menu_->Show();
        pause_menu_->Refresh();
    }

    void OnRun () override {
        logger_->debug("hiding menu");
        pause_menu_->Hide();
        pause_menu_->Refresh();
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
            case 32: {
                int item = pause_menu_->Select();

                if ( item == exit_option_ ) {
                    done_     = true;
                    ret_state = GameState::EXITING;
                    logger_->debug("Exiting from menu, and from game");
                } else if ( item == resume_option_ ) {
                    ret_state = GameState::RUNNING;
                    logger_->debug("Resuming game");

                }

                break;
            }
            case KEY_F(1):
            case 'p': {
                logger_->debug("Unpausing");
                ret_state = GameState::RUNNING;
                break;
            }
            default: {
                ret_state = GameState::PAUSED;
                break;
            }
        }

        pause_menu_->Refresh();
        return ret_state;
    }

    bool Exit () override {
        return done_;
    }

    void OnExit () override {
        pause_menu_->Hide();
        pause_menu_->Refresh();
    }

  private:
    std::shared_ptr< InputListener > listener_ { nullptr };

    std::unique_ptr< PauseMenu > pause_menu_ { nullptr };

    logger_t logger_;

    const int resume_option_ { 0 };
    const int exit_option_ = { 1 };

    bool done_ { false };

};

}
#endif //SC_PAUSE_INPUT_H

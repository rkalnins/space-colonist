//
// Created by Roberts Kalnins on 16/12/2020.
//

#ifndef SC_INPUTLISTENER_H
#define SC_INPUTLISTENER_H

#include <thread>
#include <ncurses.h>

#include "logging/logging.h"


namespace sc {

struct MousePosition {
    MousePosition ( int y, int x ) : y(y), x(x) {}

    int y { 0 };
    int x { 0 };
};


class InputListener {
  public:
    InputListener () : logger_(CreateLogger("input_listener")) {
        logger_->debug("listener active");

        input_listener_ = std::thread([this] { Listen(); });
    }

    void Stop () {
        done_ = true;
        input_listener_.join();
    }

    int GetCh () {
        return ch_;
    }

    const MousePosition &GetMousePos () const {
        return mouse_pos_;
    }

    void ResetCh () {
        std::lock_guard lg(ch_mtx_);
        ch_        = 0;
        mouse_pos_ = MousePosition { 0, 0 };
    }

  private:
    void Listen () {

        int ch_tmp;
        while ( !done_ ) {

            ch_tmp = getch();

            logger_->debug(ch_tmp);

            switch ( ch_tmp ) {
                case KEY_MOUSE: {
                    getmouse(&mouse_event_);
                    std::lock_guard lg(ch_mtx_);
                    ch_        = ch_tmp;
                    mouse_pos_ = MousePosition {
                            mouse_event_.y, mouse_event_.x
                    };
                    break;
                }
                default: {
                    std::lock_guard lg(ch_mtx_);
                    ch_ = ch_tmp;
                    break;
                }
            }
        }
    }

  private:
    logger_t logger_;

    MEVENT mouse_event_;

    std::thread         input_listener_;
    std::atomic< bool > done_ { false };
    MousePosition       mouse_pos_ { 0, 0 };

    std::recursive_mutex ch_mtx_;
    std::atomic< int >   ch_ {};
};

}

#endif //SC_INPUTLISTENER_H

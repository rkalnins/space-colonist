//
// Created by Roberts Kalnins on 16/12/2020.
//

#ifndef SC_INPUTLISTENER_H
#define SC_INPUTLISTENER_H

#include <thread>
#include <ncurses.h>

#include "logging/logging.h"


namespace sc {

class InputListener {
  public:
    InputListener () {
        input_listener_ = std::thread([this] { Listen(); });

        logger_ = spdlog::basic_logger_mt("input_listener",
                                          "logs/space-colonist-log.log");
        logger_->set_level(spdlog::level::debug);

        logger_->debug("listener active");
    }

    void Stop () {
        done_ = true;
        input_listener_.join();
    }

    int GetCh () {
        return ch_;
    }

    void ResetCh () {
        std::lock_guard lg(ch_mtx_);
        ch_ = 0;
    }

  private:
    void Listen () {

        int ch_tmp;
        while ( !done_ ) {

            ch_tmp = getch();

            logger_->debug(ch_tmp);

            std::lock_guard lg(ch_mtx_);
            ch_ = ch_tmp;
        }
    }

  private:
    logger_t logger_;

    std::thread         input_listener_;
    std::atomic< bool > done_ { false };

    std::recursive_mutex ch_mtx_;
    std::atomic< int >   ch_ {};
};

}

#endif //SC_INPUTLISTENER_H

//
// Created by Roberts Kalnins on 19/12/2020.
//

#pragma once

#include <chrono>

#include "../logging/logging.h"


namespace sc {

class Watchdog {
  public:
    Watchdog () {
        logger_ = spdlog::basic_logger_mt("watchdog",
                                          "logs/space-colonist-log.log");
        logger_->set_level(spdlog::level::debug);
    }

    void Notify () {
        auto now = std::chrono::steady_clock::now();

        if ( now - last > std::chrono::duration< double >(1.0 / 45.0)) {
            logger_->warn("Watchdog overrun");
        }

        last = now;
    }


  private:

    logger_t logger_;

    std::chrono::time_point< std::chrono::steady_clock > last {
            std::chrono::steady_clock::now()
    };
};

}





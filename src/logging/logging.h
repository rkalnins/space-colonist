//
// Created by Roberts Kalnins on 15/12/2020.
//

#ifndef SC_LOGGING_H
#define SC_LOGGING_H

#include <map>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>


namespace sc {

using logger_t = std::shared_ptr< spdlog::logger >;

static std::map< std::string, logger_t > loggers;

static logger_t CreateLogger ( const std::string &name ) {

    auto logger_found = loggers.find(name);

    if ( logger_found != loggers.end()) {
        return logger_found->second;
    }

    logger_t logger = spdlog::basic_logger_mt(name,
                                              "logs/space-colonist-log.log");
    logger->set_level(spdlog::level::debug);

    loggers[name] = logger;

    return logger;
}

}

#endif //SC_LOGGING_H

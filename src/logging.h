//
// Created by Roberts Kalnins on 15/12/2020.
//

#ifndef SC_LOGGING_H
#define SC_LOGGING_H

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

namespace sc {
    using logger_t = std::shared_ptr< spdlog::logger >;

}

#endif //SC_LOGGING_H

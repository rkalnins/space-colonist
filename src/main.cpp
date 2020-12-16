
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include "game.h"


int main () {
    auto logger = spdlog::basic_logger_mt("main",
                                          "logs/space-colonist-log.log");
    logger->set_level(spdlog::level::debug);
    spdlog::flush_every(std::chrono::seconds(1));

    using namespace sc;
    logger->info("++++++++++++++++++++++++++++++++++++++++++++++");
    logger->info("Starting game");

    Game game;
    game.Init();

    logger->info("Quit game");

    return 0;
}


#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include "game.h"
#include "windows/intro.h"
#include "input_listener.h"


int main () {
    auto logger = spdlog::basic_logger_mt("main",
                                          "logs/space-colonist-log.log");
    logger->set_level(spdlog::level::debug);
    spdlog::flush_on(spdlog::level::debug);
    spdlog::flush_every(std::chrono::seconds(1));

    using namespace sc;
    logger->info("++++++++++++++++++++++++++++++++++++++++++++++");

    initscr();
    noecho();
    keypad(stdscr, true);
    curs_set(0);

    WINDOW *main = subwin(stdscr, LINES - 2, COLS - 42, 1, 41);

    IntroWindow intro;
    intro.HideOnInput();

    std::shared_ptr< InputListener >    listener = std::make_shared< InputListener >();
    std::shared_ptr< GameDependencies > deps     = std::make_shared< GameDependencies >(
            main, listener);


    mvaddstr(18, 3, "How to play");
    mvaddstr(20, 3, "1. Choose spaceship");
    mvaddstr(21, 3, "2. Choose destination");
    mvaddstr(22, 3, "3. Choose crew");
    mvaddstr(23, 3, "4. Choose inventory");
    mvaddstr(24, 3, "5. Cross fingers & don't die");

    logger->info("Creating game");

    Game game(deps, listener);

    logger->info("Game start");
    game.Init();

    listener->Stop();
    logger->info("Quit game");

    werase(main);
    erase();
    endwin();

    return 0;
}

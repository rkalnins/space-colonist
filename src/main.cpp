
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include "config/config.h"
#include "game.h"
#include "windows/intro.h"
#include "input_listener.h"


int main () {
    auto logger = sc::CreateLogger("main");
    spdlog::flush_on(spdlog::level::debug);
    spdlog::flush_every(std::chrono::seconds(1));

    using namespace sc;
    logger->info("++++++++++++++++++++++++++++++++++++++++++++++");


    Config &config = Config::GetInstance();

    logger->debug("test: {}", config.GetConfig< int >("test", -1));

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

    int x, y;
    getmaxyx(main, y, x);

    y /= 2;

    x = x / 2 - 4;

    werase(main);

    if ( deps->nav_manager_->GetDistanceRemaining() <= 0 ) {
        werase(main);
        mvwaddstr(main, y, x, "You win!");
        y += 2;

        std::stringstream disp;

        int crew_left = deps->spaceship_handler_->GetSpaceship()->GetCrew().size();
        disp << "Crew remaining: " << crew_left;
        mvwaddstr(main, y++, x - 10, disp.str().c_str());
        disp.str("");

        int food_left = deps->spaceship_handler_->GetSpaceship()->GetFood();
        disp << "Food remaining: " << food_left;
        mvwaddstr(main, y++, x - 10, disp.str().c_str());
        disp.str("");

        disp.precision(4);
        double distance_traveled =
                       deps->nav_manager_->GetInitialDistance() -
                       deps->nav_manager_->GetDistanceRemaining();
        disp << "Distance remaining: " << distance_traveled;
        mvwaddstr(main, y++, x - 10, disp.str().c_str());
        disp.str("");

        ++y;

        double score = crew_left * 500 + food_left * 100 +
                       distance_traveled * 150;
        disp << "Score: " << score;
        mvwaddstr(main, y++, x - 10, disp.str().c_str());

    } else {
        mvwaddstr(main, y, x, "Game Over");
        y += 2;
    }

    wrefresh(main);

    getch();

    erase();
    endwin();

    return 0;
}

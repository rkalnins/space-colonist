
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

    logger->debug("test: {}", config.GetValue< int >("test", -1));

    initscr();
    noecho();
    keypad(stdscr, true);
    curs_set(0);

    int main_x         = config.GetValue("window.main-x", 0);
    int main_y         = config.GetValue("window.main-y", 0);
    int left_bar_width = config.GetValue("window.left-bar-width", 0);

    WINDOW *main = subwin(stdscr, LINES, COLS - left_bar_width, main_y,
                          main_x);

    IntroWindow intro;
    intro.HideOnInput();

    std::shared_ptr< InputListener >    listener = std::make_shared< InputListener >();
    std::shared_ptr< GameDependencies > deps     = std::make_shared< GameDependencies >(
            main, listener);

    int  title_y = config.GetValue("window.title-y", 0);
    int  title_x = config.GetValue("window.title-x", 0);
    auto title   = config.GetValue< std::string >("title", "");
    auto version = config.GetValue< std::string >("version", "");

    mvaddstr(title_y++, title_x, title.c_str());
    mvaddstr(title_y, title_x, version.c_str());

    int left_bar_start = config.GetValue("window.left-bar-start", 0);
    int help_start_y   = config.GetValue("window.help-start-y", 0);

    mvaddstr(help_start_y, left_bar_start, "How to play");

    help_start_y += 2;

    mvaddstr(help_start_y++, left_bar_start, "1. Choose spaceship");
    mvaddstr(help_start_y++, left_bar_start, "2. Choose destination");
    mvaddstr(help_start_y++, left_bar_start, "3. Choose crew");
    mvaddstr(help_start_y++, left_bar_start, "4. Choose inventory");
    mvaddstr(help_start_y, left_bar_start, "5. Cross fingers & don't die");

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

        int horizontal_move = config.GetValue< int >(
                "end.horizontal-move", 0);

        mvwaddstr(main, y, x, "You win!");
        y += 2;

        std::stringstream disp;

        int crew_left = deps->spaceship_handler_->GetSpaceship()->GetCrew().size();
        disp << "Crew remaining: " << crew_left;
        mvwaddstr(main, y++, x + horizontal_move, disp.str().c_str());
        disp.str("");

        int food_left = deps->spaceship_handler_->GetSpaceship()->GetFood();
        disp << "Food remaining: " << food_left;
        mvwaddstr(main, y++, x + horizontal_move, disp.str().c_str());
        disp.str("");

        disp.precision(4);
        double distance_traveled =
                       deps->nav_manager_->GetInitialDistance() -
                       deps->nav_manager_->GetDistanceRemaining();
        disp << "Distance traveled: " << distance_traveled;
        mvwaddstr(main, y++, x + horizontal_move, disp.str().c_str());
        disp.str("");

        ++y;

        auto crew_mult     = config.GetValue< double >("end.score.crew",
                                                       1);
        auto food_mult     = config.GetValue< double >("end.score.food",
                                                       1);
        auto distance_mult = config.GetValue< double >(
                "end.score.distance", 1);

        double score = crew_left * crew_mult + food_left * food_mult +
                       distance_traveled * distance_mult;
        disp.precision(7);
        disp << "Score: " << score;
        mvwaddstr(main, y, x + horizontal_move, disp.str().c_str());

    } else {
        mvwaddstr(main, y, x, "Game Over");
    }

    wrefresh(main);

    getch();

    erase();
    endwin();

    return 0;
}

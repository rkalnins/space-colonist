//
// Created by Roberts Kalnins on 15/12/2020.
//

#include <chrono>
#include <thread>
#include <ncurses.h>
#include <menu.h>

#include "logging.h"
#include "game.h"


namespace sc {

Game::Game () {
    logger_ = spdlog::basic_logger_mt("game",
                                      "logs/space-colonist-log.log");
    logger_->set_level(spdlog::level::debug);
    spdlog::flush_every(std::chrono::seconds { 1 });

    initscr();
    noecho();
    keypad(stdscr, true);

    mousemask(ALL_MOUSE_EVENTS, nullptr);

    main_          = subwin(stdscr, LINES - 2, COLS - 42, 1, 41);
    box(main_, 0, 0);

    pause_menu_ = std::make_unique< PauseMenu >();
}

Game::~Game () {
    input_listener.join();
    endwin();
}

void Game::LoopController () {
    using namespace std::chrono;
    auto next = steady_clock::now() + freq60_t { 1 };

    while ( !done_ ) {
        std::this_thread::sleep_until(next);
        next += freq60_t { 1 };

        // loop control
        if ( state_ == GameState::RUNNING ) {
            for ( const auto &f : running_tasks_ ) {
                f.second();
            }
            wrefresh(main_);
        } else if ( state_ == GameState::PAUSED ) {
            for ( const auto &f : paused_tasks_ ) {
                f.second();
            }
            pause_menu_->Refresh();
        }

        for ( const auto &f : always_tasks_ ) {
            f.second();
        }
        refresh();

        std::lock_guard lg(ch_mtx_);
        ch_ = 0;
    }
}

void Game::Init () {
    AddTask(running_tasks_, TaskType::RUNNING_INPUT);
    AddTask(paused_tasks_, TaskType::PAUSE_INPUT);
    input_listener = std::thread([this] { InputListener(); });

    LoopController();
}

void Game::ProcessRunningInput () {
    switch ( ch_ ) {
        case KEY_MOUSE: {
            getmouse(&mouse_event);
            logger_->debug("Mouse click at (y,x) = {}, {}", mouse_event.y,
                           mouse_event.x);

            if ( wmouse_trafo(main_, &mouse_event.y, &mouse_event.x,
                              false)) {
                mvwaddch(main_, mouse_event.y, mouse_event.x, '*');
            }
            break;
        }
        case KEY_F(1):
        case 'p': {
            logger_->debug("Pausing");
            state_ = GameState::PAUSED;
            pause_menu_->Show();
            break;
        }
        case '\n': {
            done_ = true;
            logger_->debug("Return pressed. Game done.");
            break;
        }
        default: {
            return;
        }
    }
}

void Game::ProcessPauseInput () {
    switch ( ch_ ) {
        case KEY_DOWN: {
            pause_menu_->Navigate(REQ_DOWN_ITEM);
            break;
        }
        case KEY_UP: {
            pause_menu_->Navigate(REQ_UP_ITEM);
            break;
        }
        case 10: {
            int item = pause_menu_->Select();

            if ( item == 4 ) {
                done_ = true;
            }
            // fallthrough
        }
        case KEY_F(1):
        case 'p': {
            pause_menu_->Hide();
            state_ = GameState::RUNNING;
            logger_->debug("Unpausing");
        }
        default: {
            return;
        }
    }
}

void Game::AddTask ( task_pool_t &tasks, TaskType task_type ) {
    switch ( task_type ) {
        case TaskType::RUNNING_INPUT:
            logger_->debug("Added running listener");
            tasks.emplace_back(std::make_pair(TaskType::RUNNING_INPUT,
                                              [this] { ProcessRunningInput(); }));
            break;
        case TaskType::PAUSE_INPUT:
            logger_->debug("Added pause listener");
            tasks.emplace_back(std::make_pair(TaskType::PAUSE_INPUT,
                                              [this] { ProcessPauseInput(); }));
            break;
    }
}

void Game::RemoveTask ( task_pool_t &tasks, TaskType task_type ) {
    auto check_type = [&] ( const task_t &t ) {
        return t.first == task_type;
    };
    auto task_it    = std::find_if(tasks.begin(), tasks.end(), check_type);
    if ( task_it != tasks.end()) {
        tasks.erase(task_it);
    }
}

void Game::InputListener () {
    int ch_tmp;
    while ( !done_ ) {

        ch_tmp = getch();

        std::lock_guard lg(ch_mtx_);
        ch_ = ch_tmp;
    }

}


}

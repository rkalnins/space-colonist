//
// Created by Roberts Kalnins on 17/12/2020.
//

#include <sstream>
#include <ncurses.h>
#include <cmath>
#include <ncurses.h>

#include "spaceship_handler.h"
#include "objects/crew_member.h"


namespace sc::play {

SpaceshipHandler::SpaceshipHandler ( const std::string &name,
                                     TaskType taskType,
                                     std::shared_ptr< InputListener > listener,
                                     WINDOW *main ) : Task(name, taskType),
                                                      listener_(std::move(
                                                              listener)),
                                                      main_(main) {

    logger_ = spdlog::basic_logger_mt("ss_handler",
                                      "logs/space-colonist-log.log");
    logger_->set_level(spdlog::level::debug);

    spaceship_.ResetFuel();
}

void SpaceshipHandler::Init () {
    logger_->debug("Spaceship init");
    spaceship_.SetMoney(1000);
    spaceship_.SetWeight(0);
    spaceship_.SetMaxWeight(100);
}

GameState SpaceshipHandler::OnLoop () {
    spaceship_.UseFuel(fuel_use_);

    ProcessInput();

    PrintHUD();

    if ( show_crew_ ) {
        PrintCrew();
    }

    if ( show_items_ ) {
        PrintItems();
    }

    return GameState::RUNNING;
}

bool SpaceshipHandler::IsFinished () {
    return false;
}

void SpaceshipHandler::PrintCrew () {
    int crew_y = spaceship_display_y_ + 3;

    mvwaddstr(main_, crew_y, spaceship_display_x_, "Crew");

    ++crew_y;

    for ( auto &c : spaceship_.GetCrew()) {
        std::stringstream crew_member;
        crew_member << c.GetName() << " " << c.GetHealth() << "/"
                    << c.GetMaxHealth();
        mvwaddstr(main_, crew_y, spaceship_display_x_,
                  crew_member.str().c_str());
        ++crew_y;
    }
}

void SpaceshipHandler::PrintItems () {
    int item_y = spaceship_display_y_ + 3;
    int item_x = spaceship_display_x_ + 20;

    mvwaddstr(main_, item_y, item_x, "Items");

    ++item_y;
    std::stringstream items;

    for ( auto &c : spaceship_.GetItems()) {
        mvwaddstr(main_, item_y, item_x, c.first.c_str());

        items.str("");

        for ( auto &i : c.second ) {
            items << i.GetName() << " ";
        }

        mvwaddstr(main_, item_y, item_x + 10, items.str().c_str());
        ++item_y;
    }
}

void SpaceshipHandler::PrintHUD () {
    std::stringstream display;
    display << "Crew: " << spaceship_.GetCrew().size()
            << "\tFuel: "
            << std::round(spaceship_.GetFuel() / spaceship_.GetFullFuel() *
                          100.0)
            << "\tHull: " << spaceship_.GetHull();

    mvwaddstr(main_, spaceship_display_y_, spaceship_display_x_,
              display.str().c_str());

    display.str("");

    display << "Storage: "
            << ( spaceship_.GetMaxWeight() - spaceship_.GetWeight())
            << "\tMoney: " << spaceship_.GetMoney();

    mvwaddstr(main_, ( spaceship_display_y_ + 1 ), spaceship_display_x_,
              display.str().c_str());
}

void SpaceshipHandler::ProcessInput () {
    switch ( listener_->GetCh()) {
        case KEY_MOUSE: {
            getmouse(&mouse_event_);

            spaceship_.ResetFuel();
            logger_->debug("Reset fuel", spaceship_.GetFuel());
            break;
        }
        case 'i': {
            show_items_ = !show_items_;
            logger_->debug("toggle items to {}", show_items_);
            break;
        }
        case 'c': {
            show_crew_ = !show_crew_;
            logger_->debug("toggle crew to {}", show_crew_);
            break;
        }
        case 'j': {
            CrewMember c("Joe", 10, 10, std::map< std::string, int >());
            spaceship_.AddCrewMember(c);
            break;
        }
        case 'h': {
            Item i("tools", "hammer", 10, 3);
            spaceship_.AddItem(i);
            break;
        }
        case 'f': {
            Item i("food", "potato", 1, 1);
            spaceship_.AddItem(i);
            break;
        }
        case 'a': {
            Item i("food", "apple", 1, 1);
            spaceship_.AddItem(i);
            break;
        }
        case 'b': {
            Item i("transport", "bike", 1, 10);
            spaceship_.AddItem(i);
            break;
        }
        default: {
            break;
        }
    }

    listener_->ResetCh();
}

}
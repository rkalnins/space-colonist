//
// Created by Roberts Kalnins on 17/12/2020.
//

#include <sstream>
#include <ncurses.h>
#include <cmath>

#include "spaceship_handler.h"
#include "objects/crew_member.h"


namespace sc::play {

void SpaceshipHandler::Init () {

}

GameState SpaceshipHandler::OnLoop () {
    spaceship_.UseFuel(fuel_use_);

    switch ( listener_->GetCh()) {
        case KEY_MOUSE: {
            getmouse(&mouse_event_);

            spaceship_.ResetFuel();
            logger_->debug("Reset fuel", spaceship_.GetFuel());
            break;
        }
        default: {
            break;
        }
    }


    std::stringstream display;
    display << "Crew: " << spaceship_.GetCrew().size()
            << "\tFuel: "
            << std::round(spaceship_.GetFuel() / spaceship_.GetFullFuel() *
                          100.0)
            << "\tHull: " << spaceship_.GetHull();
    mvaddstr(spaceship_display_y_, spaceship_display_x_,
             display.str().c_str());

    listener_->ResetCh();
    return GameState::RUNNING;
}

bool SpaceshipHandler::IsFinished () {
    return false;
}

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

}
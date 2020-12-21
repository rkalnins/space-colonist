//
// Created by Roberts Kalnins on 20/12/2020.
//

#include "running_ui.h"


#include <effolkronium/random.hpp>
#include <utility>
#include <sstream>


namespace sc::play {

using Random = effolkronium::random_static;

RunningUI::RunningUI ( const std::string &name, TaskType taskType,
                       std::shared_ptr< SpaceshipHandler > spaceship_handler,
                       std::shared_ptr< InputListener > listener,
                       std::shared_ptr< SpaceshipFactory > spaceship_factory,
                       WINDOW *main )
        : Task(name, taskType),
          spaceship_handler_(std::move(spaceship_handler)),
          listener_(std::move(listener)),
          spaceship_factory_(std::move(spaceship_factory)),
          main_(main) {

    logger_ = spdlog::basic_logger_mt("running_ui",
                                      "logs/space-colonist-log.log");
    logger_->set_level(spdlog::level::debug);


}

void RunningUI::Init () {
    logger_->debug("Running UI init");
    appearance_code_ = spaceship_handler_->GetSpaceship()->GetAppearanceCode();
    spaceship_handler_->GetSpaceship()->StartMoving();
}

void RunningUI::ProcessInput () {
    switch ( listener_->GetCh()) {
        case 32: {
            if ( running_state_ == RunningState::FLYING ) {
                running_state_ = RunningState::PAUSED;
                spaceship_handler_->GetSpaceship()->StopMoving();
            } else if ( running_state_ == RunningState::PAUSED ) {
                spaceship_handler_->GetSpaceship()->StartMoving();
                running_state_ = RunningState::FLYING;
            }
        }

    }
}

GameState RunningUI::OnLoop ( GameState state ) {

    ProcessInput();

    std::stringstream disp;
    disp.precision(3);
    disp << distance_remaining_
         << spaceship_handler_->GetDistanceRemaining();
    mvwaddstr(main_, dist_disp_y_, dist_disp_x_, disp.str().c_str());

    disp.str("");

    switch ( running_state_ ) {
        case RunningState::FLYING:
            MoveSpaceship();
            UpdateSpaceshipState();
            break;
        case RunningState::PAUSED:
            ShowPauseOptions();
            break;
        case RunningState::SITUATION:
            break;
    }
    spaceship_factory_->PrintSpaceship(main_, ss_pos_y_, ss_pos_x_,
                                       appearance_code_);

    return GameState::RUNNING;
}

void RunningUI::MoveSpaceship () {
    if ( ss_mvmt_counter_++ > ss_mvmt_period_ ) {

        bool move_x = Random::get< bool >(ss_mvmt_x_prob_);
        bool move_y = Random::get< bool >(ss_mvmt_y_prob_);

        if ( move_x ) {
            ss_pos_x_ += Random::get({ -1, 1 });
        }

        if ( move_y ) {
            ss_pos_y_ += Random::get({ -1, 1 });
        }

        BoundSpaceshipPosition();

        ss_mvmt_counter_ = 0;
    }
}

void RunningUI::BoundSpaceshipPosition () {
    if ( ss_pos_x_ > ss_max_x_ ) {
        ss_pos_x_ = ss_max_x_;
    }

    if ( ss_pos_x_ < ss_min_x_ ) {
        ss_pos_x_ = ss_min_x_;
    }

    if ( ss_pos_y_ > ss_max_y_ ) {
        ss_pos_y_ = ss_max_y_;
    }

    if ( ss_pos_y_ < ss_min_y_ ) {
        ss_pos_y_ = ss_min_y_;
    }
}

void RunningUI::UpdateSpaceshipState () {
    spaceship_handler_->GetSpaceship()->UseFuel(fuel_usage_);
    spaceship_handler_->UpdateDistanceRemaining(-1 * distance_traveled_);


    if ( ss_food_usage_counter_++ > ss_food_usage_period_ ) {
        spaceship_handler_->GetSpaceship()->UseFood();
        ss_food_usage_counter_ = 0;
    }

}

void RunningUI::ShowPauseOptions () {


}


}
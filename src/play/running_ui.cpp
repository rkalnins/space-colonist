//
// Created by Roberts Kalnins on 20/12/2020.
//

#include "running_ui.h"


#include <effolkronium/random.hpp>
#include <utility>
#include <queue>
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
            switch ( running_state_ ) {
                case RunningState::FLYING:
                    Pause();
                    logger_->debug("Pausing");
                    break;
                case RunningState::PAUSED:
                    spaceship_handler_->GetSpaceship()->StartMoving();

                    if ( situation_type_ != SituationType::NONE ) {
                        running_state_ = RunningState::SITUATION;
                        logger_->debug("Returning to situation");
                    } else {
                        running_state_ = RunningState::FLYING;
                        logger_->debug("Returning to flying");
                    }

                    if ( !notifications_.empty()) {
                        notifications_.pop();
                    }
                    break;
                case RunningState::SITUATION:
                    Pause();
                    logger_->debug("Pausing");
                    break;
            }
            break;
        }
        case '1': {
            switch ( running_state_ ) {
                case RunningState::PAUSED: {
                    switch ( menu_options_ ) {
                        case MenuOptions::MAIN:
                            if ( situation_type_ !=
                                 SituationType::ENGINE_FAILURE ) {
                                menu_options_ = MenuOptions::VELOCITY_CHANGE;
                            }
                            break;
                        case MenuOptions::VELOCITY_CHANGE:
                            UpdateVelocity(stop_);
                            break;
                        case MenuOptions::RATION_CHANGE:
                            ss_food_usage_period_ = ss_half_rations_;
                            rations_              = "Half";
                            menu_options_         = MenuOptions::MAIN;
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case RunningState::SITUATION: {
                    switch ( situation_type_ ) {
                        case SituationType::MINOR:

                            if ( !situations_.empty()) {
                                situations_.pop();
                            }
                            ++ignored_minor_mech_failures_;
                            situation_type_ = SituationType::NONE;
                            running_state_  = RunningState::FLYING;
                            break;
                        case SituationType::ENGINE_FAILURE:
                        case SituationType::AIR_FILTER_FAILURE:
                            if ( !UseGenericSpareParts()) { break; }
                            fixing_ = true;
                            break;
                        default:
                            break;
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case '2': {
            switch ( running_state_ ) {
                case RunningState::PAUSED: {
                    switch ( menu_options_ ) {
                        case MenuOptions::MAIN:
                            menu_options_ = MenuOptions::RATION_CHANGE;
                            break;
                        case MenuOptions::VELOCITY_CHANGE:
                            UpdateVelocity(slow_);
                            break;
                        case MenuOptions::RATION_CHANGE:
                            ss_food_usage_period_ = ss_normal_rations_;
                            rations_              = "Normal";
                            menu_options_         = MenuOptions::MAIN;
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case RunningState::SITUATION: {
                    switch ( situation_type_ ) {
                        case SituationType::MINOR:
                            if ( !UseGenericSpareParts()) { break; }
                            fixing_ = true;
                            break;
                        default:
                            break;
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case '3': {
            switch ( running_state_ ) {
                case RunningState::PAUSED:
                    switch ( menu_options_ ) {
                        case MenuOptions::MAIN:
                            if ( trickle_nominal_ ) {
                                fuel_trickle_ = nominal_trickle_;
                            } else {
                                fuel_trickle_ = low_e_trickle_;
                            }

                            trickle_nominal_ = !trickle_nominal_;

                            break;
                        case MenuOptions::VELOCITY_CHANGE:
                            UpdateVelocity(moderate_);
                            break;
                        case MenuOptions::RATION_CHANGE:
                            ss_food_usage_period_ = ss_filling_rations_;
                            rations_              = "Filling";
                            menu_options_         = MenuOptions::MAIN;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
        }
        case '4': {
            switch ( running_state_ ) {
                case RunningState::PAUSED:
                    switch ( menu_options_ ) {
                        case MenuOptions::MAIN: {
                            if ( ignored_minor_mech_failures_ <= 0 ) {
                                ignored_minor_mech_failures_ = 0;
                                break;
                            }
                            if ( !UseGenericSpareParts()) { break; }
                            fixing_minor_  = true;
                            running_state_ = RunningState::FLYING;
                            logger_->debug("Trying to fix minor issue");
                            if ( !notifications_.empty()) {
                                notifications_.pop();
                            }
                            break;
                        }
                        case MenuOptions::VELOCITY_CHANGE:
                            UpdateVelocity(fast_);
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
        }
        case '5': {
            switch ( running_state_ ) {
                case RunningState::PAUSED:
                    switch ( menu_options_ ) {
                        case MenuOptions::VELOCITY_CHANGE:
                            UpdateVelocity(dangerous_);
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
        }
        case 'y': {
            if ( running_state_ == RunningState::SITUATION &&
                 !enough_spares_ ) {
                waiting_for_help_ = true;
            }
            break;
        }
    }
}

GameState RunningUI::OnLoop ( GameState state ) {

    ProcessInput();

    std::stringstream disp;
    disp.precision(3);
    disp << distance_remaining_
         << spaceship_handler_->GetDistanceRemaining() << "\t"
         << velocity_name_ << ( velocity_ * 100.0 ) << "\tRations: "
         << rations_;
    mvwaddstr(main_, dist_disp_y_, dist_disp_x_, disp.str().c_str());

    disp.str("");

    spaceship_factory_->PrintSpaceship(main_, ss_pos_y_, ss_pos_x_,
                                       appearance_code_);


    switch ( running_state_ ) {
        case RunningState::FLYING:
            IsSituation();

            if ( fixing_minor_ ) {
                FixMinorIgnored();
            }

            if ( velocity_ != stop_ ) {
                MoveSpaceship();
            }

            UpdateSpaceshipState();
            UpdateCrew();
            break;
        case RunningState::PAUSED:
            switch ( menu_options_ ) {
                case MenuOptions::MAIN:
                    ShowPauseOptions();
                    break;
                case MenuOptions::VELOCITY_CHANGE:
                    ShowVelocityChangeOptions();
                    break;
                case MenuOptions::RATION_CHANGE:
                    ShowChangeRationsOptions();
                    break;
            }
            break;
        case RunningState::SITUATION:
            if ( velocity_ != stop_ ) {
                MoveSpaceship();
            }

            UpdateSpaceshipState();
            UpdateCrew();

            ShowSituationReport();

            if ( fixing_ ) {
                AttemptFix();
            }

            if ( waiting_for_help_ ) {
                WaitForHelp();
            }

            break;
    }

    if ( !notifications_.empty()) {
        Pause();
        return GameState::RUNNING; // clear queue before exiting
    }

    return ret_state;
}

void RunningUI::WaitForHelp () {
    if ( Random::get< bool >(successful_distress_)) {
        notifications_.push("Saved by friendly aliens.");
        situation_type_   = SituationType::NONE;
        running_state_    = RunningState::FLYING;
        waiting_for_help_ = false;

        if ( !situations_.empty()) {
            situations_.pop();
        }
    }
}

void RunningUI::FixMinor () {
    if ( Random::get< bool >(minor_fix_prob_)) {
        std::string fixed = "Fixed \"" + situations_.front() +
                            "\"";
        notifications_.push(fixed);
        situation_type_ = SituationType::NONE;
        running_state_  = RunningState::FLYING;
        fixing_         = false;

        if ( !situations_.empty()) {
            situations_.pop();
        }
    }
}

void RunningUI::FixMinorIgnored () {
    if ( Random::get< bool >(minor_fix_prob_)) {
        logger_->debug("Fixed ignored issue");
        std::string fixed = "Fixed ignored minor issue";
        notifications_.push(fixed);
        fixing_minor_ = false;

        --ignored_minor_mech_failures_;
    }
}

bool RunningUI::UseGenericSpareParts () {
    if ( !spaceship_handler_->GetSpaceship()->UseSpareParts(req_cabling_,
                                                            req_components_)) {
        notifications_.push("Not enough spare parts");
        enough_spares_ = false;
        return false;
    } else {
        logger_->debug("Enough spare parts");
        enough_spares_ = true;
        return true;
    }
}

void RunningUI::AttemptFix () {
    switch ( situation_type_ ) {
        case SituationType::MINOR:
            FixMinor();
            break;
        case SituationType::ENGINE_FAILURE: {
            if ( Random::get< bool >(major_fix_prob_)) {
                notifications_.push("Fixed engine. Engine nominal.");
                situation_type_ = SituationType::NONE;
                running_state_  = RunningState::FLYING;
                fixing_         = false;

                if ( !situations_.empty()) {
                    situations_.pop();
                }
            }
            break;
        }
        case SituationType::AIR_FILTER_FAILURE: {
            if ( Random::get< bool >(major_fix_prob_)) {
                notifications_.push("Fixed air filter.");
                situation_type_ = SituationType::NONE;
                running_state_  = RunningState::FLYING;
                fixing_         = false;

                if ( !situations_.empty()) {
                    situations_.pop();
                }
            }
            break;
        }
        default:
            break;
    }

}

void RunningUI::ShowSituationReport () {
    int y = pause_y_;
    int x = pause_x_;

    mvwaddstr(main_, y++, x - 8, "Situation Report");
    mvwaddstr(main_, y++, x - 23,
              "---------------------------------------------");
    if ( !situations_.empty()) {
        mvwaddstr(main_, y++, x - situations_.front().length() / 2,
                  situations_.front().c_str());
        mvwaddstr(main_, y++, x - 23,
                  "---------------------------------------------");
    }

    for ( auto &o : sitrep_options_[situation_type_] ) {
        mvwaddstr(main_, y++, x - 12, o.c_str());
    }

    if ( !enough_spares_ ) {
        mvwaddstr(main_, y++, x - 12,
                  "Not enough spares. Issue distress signal (y/n)?");
        return;
    }

    y++;
    if ( enough_spares_ && fixing_ ) {
        mvwaddstr(main_, y, x - 12, "Current Action: Fixing");
    } else if ( !enough_spares_ && waiting_for_help_ ) {
        mvwaddstr(main_, y, x - 12, "Current Action: Waiting for help");
    }

}

bool RunningUI::UpdateVelocity ( double new_velocity ) {
    if ( Random::get< bool >(unresponsive_engines_)) {
        situations_.push("Unresponsive engines. Cause unknown.");
        situation_type_ = SituationType::ENGINE_FAILURE;
        menu_options_   = MenuOptions::MAIN;
        return true;
    } else {
        spaceship_handler_->GetSpaceship()->UseFuel(
                std::abs(( new_velocity - velocity_ )) *
                velocity_to_fuel_ratio_);
        velocity_     = new_velocity;
        menu_options_ = MenuOptions::MAIN;
        return false;
    }

}

bool RunningUI::IsSituation () {
    if ( situation_type_ != SituationType::NONE ) {
        return false;
    }

    double major_p_ = std::min(1.0,
                               major_failure_prob_ *
                               ignored_minor_mech_failures_);

    if ( Random::get< bool >(major_p_)) {
        char f = Random::get({ 'e', 'a' });
        switch ( f ) {
            case 'e':
                situation_type_ = SituationType::ENGINE_FAILURE;
                situations_.push(*Random::get(engine_failures_));
                running_state_ = RunningState::SITUATION;
                break;
            case 'a':
                situation_type_ = SituationType::AIR_FILTER_FAILURE;
                situations_.push(*Random::get(air_filter_failure_));
                running_state_ = RunningState::SITUATION;
                break;
            default:
                situation_type_ = SituationType::NONE;
        }

        req_cabling_    = *Random::get(cabling_used_major_);
        req_components_ = *Random::get(components_used_major_);

        logger_->debug("Req cabling/comp: {} {}", req_cabling_,
                       req_components_);

        logger_->debug("Major failure");
        return true;
    }


    if ( Random::get< bool >(minor_failure_prob_)) {
        situation_type_ = SituationType::MINOR;
        running_state_  = RunningState::SITUATION;
        situations_.push(*Random::get(minor_failures_));


        req_cabling_    = *Random::get(cabling_used_minor_);
        req_components_ = *Random::get(components_used_minor_);

        logger_->debug("Req cabling/comp: {} {}", req_cabling_,
                       req_components_);
        logger_->debug("Minor failure");
        return true;
    }

    return false;
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
    if ( ret_state == GameState::EXITING ) {
        return;
    }

    spaceship_handler_->GetSpaceship()->UseFuel(fuel_trickle_);
    spaceship_handler_->UpdateDistanceRemaining(-1 * velocity_);


    if ( ss_food_usage_counter_++ > ss_food_usage_period_ ) {
        spaceship_handler_->GetSpaceship()->UseFood();
        ss_food_usage_counter_ = 0;
    }

    if ( spaceship_handler_->GetSpaceship()->GetFuel() <= 0 ) {
        notifications_.push("Out of fuel :(");
        ret_state = GameState::EXITING;
    }

    if ( spaceship_handler_->GetSpaceship()->GetCrew().empty()) {
        notifications_.push("All crew dead :(");
        ret_state = GameState::EXITING;
    }

    if ( !notified_no_food_ &&
         spaceship_handler_->GetSpaceship()->GetFood() <= 0 ) {
        notifications_.push("Out of food :(");
        notified_no_food_ = true;
    } else if ( spaceship_handler_->GetSpaceship()->GetFood() > 0 ) {
        notified_no_food_ = false;
    }


}

void RunningUI::ShowPauseOptions () {
    int y = pause_y_;
    int x = pause_x_;

    mvwaddstr(main_, y++, x - 3, "Paused");
    mvwaddstr(main_, y++, x - 23,
              "---------------------------------------------");

    if ( !notifications_.empty()) {
        mvwaddstr(main_, y++, x - notifications_.front().length() / 2,
                  notifications_.front().c_str());
        mvwaddstr(main_, y++, x - 23,
                  "---------------------------------------------");
    }
    std::string vel = ( situation_type_ != SituationType::ENGINE_FAILURE
                        ? "1. Change velocity"
                        : "1. <Unable to change velocity>" );
    mvwaddstr(main_, y++, x - 12, vel.c_str());
    mvwaddstr(main_, y++, x - 12, "2. Change rations");
    std::string       en     = ( !trickle_nominal_ ? "enabled"
                                                   : "disabled" );
    const std::string &power = "3. Toggle low power mode. Currently " + en;
    mvwaddstr(main_, y++, x - 12, power.c_str());

    if ( ignored_minor_mech_failures_ > 0 ) {
        std::stringstream out;
        out << "4. Attempt to fix one of (" << ignored_minor_mech_failures_
            << ") minor issues";
        mvwaddstr(main_, y++, x - 12, out.str().c_str());
    }
    ++y;
    mvwaddstr(main_, y++, x - 12, "Hit [Space] to continue.");
}

void RunningUI::UpdateCrew () {

    if ( ret_state == GameState::EXITING ) {
        return;
    }

    if ( health_update_counter_++ > health_update_period_ ) {
        UpdateCrewFood();
        health_update_counter_ = 0;
    }
}

void RunningUI::UpdateCrewFood () {
    std::vector< CrewMember > &crew = spaceship_handler_->GetSpaceship()->GetCrew();

    if ( spaceship_handler_->GetSpaceship()->GetFood() <= 0 ) {
        ss_food_usage_period_ = starve_period_;
        for ( auto &c : crew ) {
            c.UpdateHealth(-1);
        }
    } else {

        if ( ss_food_usage_period_ == ss_filling_rations_ &&
             Random::get< bool >(filling_ration_health_change_prob_)) {
            Random::get(crew)->UpdateHealth(1);

        } else if ( ss_food_usage_period_ == ss_half_rations_ &&
                    Random::get< bool >(half_ration_health_change_prob_)) {
            Random::get(crew)->UpdateHealth(-1);

        } else if ( ss_food_usage_period_ == ss_normal_rations_ &&
                    Random::get< bool >(
                            normal_ration_health_change_prob_)) {
            Random::get(crew)->UpdateHealth(1);
        }
    }


    for ( auto &c : crew ) {
        if ( c.IsDead()) {
            notifications_.push(
                    c.GetName() + " has starved to death :(");
        }
    }

    spaceship_handler_->GetSpaceship()->RemoveDeadCrew();
}

void RunningUI::Pause () {
    running_state_ = RunningState::PAUSED;
    menu_options_  = MenuOptions::MAIN;
    spaceship_handler_->GetSpaceship()->StopMoving();
}

void RunningUI::ShowVelocityChangeOptions () {
    int y = pause_y_;
    int x = pause_x_;

    std::stringstream disp;
    disp.precision(3);

    double stop_f = std::abs(( stop_ - velocity_ )) *
                    velocity_to_fuel_ratio_;
    double slow_f = std::abs(( slow_ - velocity_ )) *
                    velocity_to_fuel_ratio_;
    double mod_f  = std::abs(( moderate_ - velocity_ )) *
                    velocity_to_fuel_ratio_;
    double fast_f = std::abs(( fast_ - velocity_ )) *
                    velocity_to_fuel_ratio_;
    double dang_f = std::abs(( dangerous_ - velocity_ )) *
                    velocity_to_fuel_ratio_;

    disp << spaceship_handler_->GetSpaceship()->GetFuel();
    std::string available = "Available: " + disp.str();
    disp.str("");

    disp << stop_f;
    std::string stop = "1. Stop (" + disp.str() + " fuel)";
    disp.str("");

    disp << slow_f;
    std::string slow = "2. Slow (" + disp.str() + " fuel)";
    disp.str("");

    disp << mod_f;
    std::string moderate = "3. Moderate (" + disp.str() + " fuel)";
    disp.str("");

    disp << fast_f;
    std::string fast = "4. Fast (" + disp.str() + " fuel)";
    disp.str("");

    disp << dang_f;
    std::string dangerous = "5. Dangerous (" + disp.str() + " fuel)";
    disp.str("");

    mvwaddstr(main_, y++, x - 3, "Paused");
    mvwaddstr(main_, y++, x - 23,
              "---------------------------------------------");
    mvwaddstr(main_, y++, x - 26,
              "Any velocity change requires fuel. Save enough fuel to stop.");
    mvwaddstr(main_, y++, x - 12, "Would be unfortunate...");
    mvwaddstr(main_, y++, x - 10, available.c_str());
    ++y;
    mvwaddstr(main_, y++, x - 10, stop.c_str());
    mvwaddstr(main_, y++, x - 10, slow.c_str());
    mvwaddstr(main_, y++, x - 10, moderate.c_str());
    mvwaddstr(main_, y++, x - 10, fast.c_str());
    mvwaddstr(main_, y++, x - 10, dangerous.c_str());
}

void RunningUI::ShowChangeRationsOptions () {

    int y = pause_y_;
    int x = pause_x_;

    mvwaddstr(main_, y++, x - 3, "Paused");
    mvwaddstr(main_, y++, x - 23,
              "---------------------------------------------");
    mvwaddstr(main_, y++, x - 10, "1. Half");
    mvwaddstr(main_, y++, x - 10, "2. Normal");
    mvwaddstr(main_, y++, x - 10, "3. Filling");
}


}
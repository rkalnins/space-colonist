//
// Created by Roberts Kalnins on 20/12/2020.
//

#include "running_ui.h"


#include <effolkronium/random.hpp>
#include <utility>
#include <queue>
#include <sstream>


namespace sc::play {


RunningUI::RunningUI ( const std::string &name, TaskType taskType,
                       std::shared_ptr< SpaceshipHandler > spaceship_handler,
                       std::shared_ptr< play::NavigationControlManager > nav_manager,
                       std::shared_ptr< InputListener > listener,
                       WINDOW *main )
        : Task(name, taskType),
          spaceship_handler_(std::move(spaceship_handler)),
          nav_manager_(std::move(nav_manager)),
          listener_(std::move(listener)),
          main_(main), logger_(CreateLogger(name)) {}

void RunningUI::Init () {
    logger_->debug("Running UI init");
    spaceship_ = spaceship_handler_->GetSpaceship();
    nav_manager_->SetVelocity(Velocity::SLOW);
    pause_menu_ = std::make_unique< PauseMenu >(spaceship_, nav_manager_,
                                                main_);
}

void RunningUI::ProcessInput () {
    switch ( listener_->GetCh()) {
        case 32: {
            switch ( running_state_ ) {
                case RunningState::FLYING:
                    Pause();
                    logger_->debug("Pausing");
                    spaceship_->StopMoving();
                    break;
                case RunningState::PAUSED:
                    spaceship_->StartMoving();
                    if ( situation_type_ != SituationType::NONE ) {
                        running_state_ = RunningState::SITUATION;
                        logger_->debug("Returning to situation");
                    } else {
                        running_state_ = RunningState::FLYING;
                        logger_->debug("Returning to flying");
                    }

                    pause_menu_->ClearLastNotification();
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
                            UpdateVelocity(Velocity::STOP);
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
                            UpdateVelocity(Velocity::SLOW);
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
                            nav_manager_->TogglePowerMode();
                            break;
                        case MenuOptions::VELOCITY_CHANGE:
                            UpdateVelocity(Velocity::MODERATE);
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
                            pause_menu_->ClearLastNotification();
                            break;
                        }
                        case MenuOptions::VELOCITY_CHANGE:
                            UpdateVelocity(Velocity::FAST);
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
                            UpdateVelocity(Velocity::DANGEROUS);
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
         << nav_manager_->GetDistanceRemaining() << "\t"
         << velocity_name_ << ( nav_manager_->GetVelocity() * 100.0 )
         << "\tRations: "
         << rations_;
    mvwaddstr(main_, dist_disp_y_, dist_disp_x_, disp.str().c_str());

    disp.str("");

    spaceship_handler_->PrintSpaceship(main_);

    switch ( running_state_ ) {
        case RunningState::FLYING:
            IsSituation();

            if ( fixing_minor_ ) {
                FixMinorIgnored();
            }

            Update();

            break;
        case RunningState::PAUSED:
            switch ( menu_options_ ) {
                case MenuOptions::MAIN:
                    pause_menu_->ShowPauseOptions(situation_type_,
                                                  ignored_minor_mech_failures_);
                    break;
                case MenuOptions::VELOCITY_CHANGE:
                    pause_menu_->ShowVelocityChangeOptions();
                    break;
                case MenuOptions::RATION_CHANGE:
                    pause_menu_->ShowChangeRationsOptions();
                    break;
            }
            break;
        case RunningState::SITUATION:
            if ( situation_type_ == SituationType::AIR_FILTER_FAILURE ) {
                ++situation_counter_;
            }

            Update();

            ShowSituationReport();

            if ( fixing_ ) {
                AttemptFix();
            }

            if ( waiting_for_help_ ) {
                WaitForHelp();
            }

            break;
    }

    if ( pause_menu_->HasNotifications()) {
        Pause();
        return GameState::RUNNING; // clear queue before exiting
    }

    return ret_state;
}

void RunningUI::Update () {
    MoveFlyingObject();

    if ( !nav_manager_->IsStopped()) {
        MoveSpaceship();
    }

    UpdateSpaceshipState();
    UpdateCrew();
}

void RunningUI::WaitForHelp () {
    if ( Random::get< bool >(successful_distress_)) {
        pause_menu_->PushNotification("Saved by friendly aliens.");
        situation_type_    = SituationType::NONE;
        running_state_     = RunningState::FLYING;
        waiting_for_help_  = false;
        air_is_poisoned_   = false;
        enough_spares_     = true;
        situation_counter_ = 0;

        if ( !situations_.empty()) {
            situations_.pop();
        }
    }
}

void RunningUI::FixMinor () {
    if ( Random::get< bool >(minor_fix_prob_)) {
        std::string fixed = "Fixed \"" + situations_.front() +
                            "\"";
        pause_menu_->PushNotification(fixed);
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
        pause_menu_->PushNotification(fixed);
        fixing_minor_  = false;
        enough_spares_ = true;

        --ignored_minor_mech_failures_;
    }
}

bool RunningUI::UseGenericSpareParts () {
    if ( !spaceship_->UseSpareParts(req_cabling_, req_components_)) {
        pause_menu_->PushNotification("Not enough spare parts");
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
                pause_menu_->PushNotification(
                        "Fixed engine. Engine nominal.");
                situation_type_    = SituationType::NONE;
                running_state_     = RunningState::FLYING;
                situation_counter_ = 0;
                fixing_            = false;
                enough_spares_     = true;

                if ( !situations_.empty()) {
                    situations_.pop();
                }
            }
            break;
        }
        case SituationType::AIR_FILTER_FAILURE: {
            if ( Random::get< bool >(major_fix_prob_)) {
                pause_menu_->PushNotification("Fixed air filter.");
                situation_type_    = SituationType::NONE;
                running_state_     = RunningState::FLYING;
                air_is_poisoned_   = false;
                enough_spares_     = true;
                situation_counter_ = 0;
                fixing_            = false;

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

    y++;
    if ( !waiting_for_help_ && !enough_spares_ ) {
        mvwaddstr(main_, y++, x - 12,
                  "Not enough spares. Issue distress signal (y/n)?");
    }

    if ( enough_spares_ && fixing_ ) {
        mvwaddstr(main_, y++, x - 12, "Current Action: Fixing");
    }

    if ( waiting_for_help_ ) {
        mvwaddstr(main_, y++, x - 12, "Current Action: Waiting for help");
    }

    if ( situation_type_ == SituationType::AIR_FILTER_FAILURE ) {
        int seconds = air_response_time_ -
                      ( situation_counter_ / second_count_period_ );
        if ( seconds >= 0 ) {
            std::stringstream disp;
            disp << seconds << " before poisoning begins";
            mvwaddstr(main_, y++, x - 12, disp.str().c_str());
        } else {
            mvwaddstr(main_, y++, x - 12,
                      "Crew is dying of poisoned air.");
        }
    }
}

bool RunningUI::UpdateVelocity ( Velocity new_velocity ) {
    if ( Random::get< bool >(unresponsive_engines_)) {
        situations_.push("Unresponsive engines. Cause unknown.");
        situation_type_ = SituationType::ENGINE_FAILURE;
        menu_options_   = MenuOptions::MAIN;
        return true;
    } else {
        nav_manager_->SetVelocity(new_velocity);
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
                               ( ignored_minor_mech_failures_ + 1 ));

    if ( Random::get< bool >(major_p_)) {
        char f = Random::get({ 'a', 'e' });
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
        spaceship_handler_->MoveSpaceship();
        ss_mvmt_counter_ = 0;
    }
}

void RunningUI::UpdateSpaceshipState () {
    if ( ret_state == GameState::EXITING ) {
        return;
    }

    if ( ss_food_usage_counter_++ > ss_food_usage_period_ ) {
        spaceship_->UseFood();
        ss_food_usage_counter_ = 0;
    }

    if ( !spaceship_->HasFuel()) {
        pause_menu_->PushNotification("Out of fuel :(");
        ret_state = GameState::EXITING;
    }

    if ( spaceship_->GetCrew().empty()) {
        pause_menu_->PushNotification("All crew dead :(");
        ret_state = GameState::EXITING;
    }

    if ( !notified_no_food_ && !spaceship_->HasFood()) {
        pause_menu_->PushNotification("Out of food :(");
        notified_no_food_ = true;
    } else if ( spaceship_->HasFood()) {
        notified_no_food_ = false;
    }

    if ( situation_type_ == SituationType::AIR_FILTER_FAILURE &&
         !air_is_poisoned_ && air_response_time_ -
                              ( situation_counter_ /
                                second_count_period_ ) <= 0 ) {
        logger_->debug("Air is poisoned");
        air_is_poisoned_ = true;
    }


}

void RunningUI::UpdateAllCrewHealth () {
    std::vector< CrewMember > &crew = spaceship_->GetCrew();

    if ( air_is_poisoned_ ) {
        for ( auto &c : crew ) {
            c.UpdateHealth(-2);
        }
    }

    for ( auto &c : crew ) {
        if ( c.IsDead()) {
            pause_menu_->PushNotification(
                    c.GetName() +
                    " has suffocated to death from poisoned air :(");
        }
    }

    spaceship_->RemoveDeadCrew();

}

void RunningUI::UpdateCrew () {

    if ( ret_state == GameState::EXITING ) {
        return;
    }

    if ( health_update_counter_++ > health_update_period_ ) {
        UpdateAllCrewHealth();
        UpdateCrewFood();

        health_update_counter_ = 0;
    }
}

void RunningUI::UpdateCrewFood () {
    std::vector< CrewMember > &crew = spaceship_->GetCrew();

    if ( spaceship_->GetFood() <= 0 ) {
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
            pause_menu_->PushNotification(
                    c.GetName() + " has starved to death :(");
        }
    }

    spaceship_->RemoveDeadCrew();
}

void RunningUI::Pause () {
    running_state_ = RunningState::PAUSED;
    menu_options_  = MenuOptions::MAIN;
}

void RunningUI::MoveFlyingObject () {

    if ( !flying_debris_ && Random::get< bool >(flying_object_prob_)) {
        flying_debris_ = std::make_unique< FlyingDebris >(main_);
    } else {
        if ( !flying_debris_ ) { return; }

        if ( flying_debris_->IsDone()) {
            flying_debris_.reset();
            return;
        }

        flying_debris_->Move();
    }
}


}
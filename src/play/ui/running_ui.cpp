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
    pause_menu_        = std::make_shared< PauseMenu >(spaceship_,
                                                       nav_manager_,
                                                       main_);
    situation_manager_ = std::make_unique< SituationManager >(main_,
                                                              pause_menu_);
}

void RunningUI::ProcessInput () {

    int c = listener_->GetCh();

    switch ( running_state_ ) {

        case RunningState::FLYING: {
            switch ( c ) {
                case 32:
                    Pause();
                    break;
                default:
                    break;
            }
            break;
        }
        case RunningState::PAUSED: {
            switch ( c ) {
                case 32:
                    Unpause();
                    break;
                case '1': {
                    switch ( menu_options_ ) {
                        case MenuOptions::MAIN:
                            if ( !situation_manager_->IsEngineFailure()) {
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
                case '2': {
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
                case '3': {
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
                }
                case '4': {
                    switch ( menu_options_ ) {
                        case MenuOptions::MAIN: {

                            if ( situation_manager_->GetIgnoredFailures() >
                                 0 &&
                                 !situation_manager_->IsFixingMinor() &&
                                 !UseGenericSpareParts()) { break; }
                            situation_manager_->StartFixingMinor();
                            running_state_ = RunningState::FLYING;
                            break;
                        }
                        case MenuOptions::VELOCITY_CHANGE:
                            UpdateVelocity(Velocity::FAST);
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case '5': {
                    switch ( menu_options_ ) {
                        case MenuOptions::VELOCITY_CHANGE:
                            UpdateVelocity(Velocity::DANGEROUS);
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
        case RunningState::SITUATION: {
            switch ( c ) {
                case 32:
                    Pause();
                    break;
                case '1': {
                    switch ( situation_manager_->GetSituationType()) {
                        case SituationType::MINOR:
                            situation_manager_->IgnoreMinorFailure();
                            running_state_ = RunningState::FLYING;
                            break;
                        case SituationType::ENGINE_FAILURE:
                        case SituationType::AIR_FILTER_FAILURE:
                            if ( !UseGenericSpareParts()) { break; }
                            situation_manager_->StartImmediateFixing();
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case '2': {
                    switch ( situation_manager_->GetSituationType()) {
                        case SituationType::MINOR:
                            situation_manager_->StartImmediateFixing();
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case 'y': {
                    situation_manager_->StartWaitingForHelp();
                    break;
                }
                default:
                    break;
            }

            break;
        }
    }
}

void RunningUI::Unpause () {
    spaceship_->StartMoving();
    if ( situation_manager_->IsSituation()) {
        running_state_ = RunningState::SITUATION;
        logger_->debug("Returning to situation");
    } else {
        running_state_ = RunningState::FLYING;
        logger_->debug("Returning to flying");
    }

    pause_menu_->ClearLastNotification();
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
            if ( situation_manager_->CheckNewSituation()) {
                running_state_ = RunningState::SITUATION;
            }

            situation_manager_->FixMinorIgnored();

            StandardLoopUpdate();

            break;
        case RunningState::PAUSED:
            switch ( menu_options_ ) {
                case MenuOptions::MAIN:
                    pause_menu_->ShowPauseOptions(
                            situation_manager_->GetSituationType(),
                            situation_manager_->GetIgnoredFailures());
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
            situation_manager_->UpdateCounter();

            StandardLoopUpdate();

            situation_manager_->ShowSituationReport();

            if ( situation_manager_->AttemptFix()) {
                running_state_ = RunningState::FLYING;
            }

            if ( situation_manager_->WaitForHelp()) {
                running_state_ = RunningState::FLYING;
            }

            break;
    }

    if ( pause_menu_->HasNotifications()) {
        Pause();
        return GameState::RUNNING; // clear queue before exiting
    }

    return ret_state;
}

void RunningUI::StandardLoopUpdate () {
    MoveFlyingObject();

    if ( !nav_manager_->IsStopped()) {
        MoveSpaceship();
    }

    UpdateSpaceshipState();
    UpdateCrew();
}

bool RunningUI::UseGenericSpareParts () {
    if ( !spaceship_->UseSpareParts(situation_manager_->GetReqCabling(),
                                    situation_manager_->GetReqComponents())) {
        pause_menu_->PushNotification("Not enough spare parts");
        situation_manager_->SetEnoughSpares(false);
        return false;
    } else {
        logger_->debug("Enough spare parts");
        situation_manager_->SetEnoughSpares(true);
        return true;
    }
}

bool RunningUI::UpdateVelocity ( Velocity new_velocity ) {
    if ( situation_manager_->SuddenEngineFailure()) {
        menu_options_ = MenuOptions::MAIN;
        return true;
    } else {
        nav_manager_->SetVelocity(new_velocity);
        menu_options_ = MenuOptions::MAIN;
        return false;
    }

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
}

void RunningUI::UpdateAllCrewHealth () {
    std::vector< CrewMember > &crew = spaceship_->GetCrew();

    if ( situation_manager_->IsAirPoisoned()) {
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
    spaceship_->StopMoving();
    logger_->debug("Pausing");
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
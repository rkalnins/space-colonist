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
                       shared_spaceship_handler_t spaceship_handler,
                       shared_nav_manager_t nav_manager,
                       shared_input_listener_t listener,
                       WINDOW *main )
        : Task(name, taskType),
          spaceship_handler_(std::move(spaceship_handler)),
          nav_manager_(std::move(nav_manager)),
          listener_(std::move(listener)),
          main_(main), logger_(CreateLogger(name)) {

    Config &config = Config::GetInstance();

    flying_object_prob_ = config.GetValue("running-ui.flying-object-p",
                                          0.0);

    dist_disp_y_ = config.GetValue("running-ui.distance-disp-y", 0);
    dist_disp_x_ = config.GetValue("running-ui.distance-disp-x", 0);

    health_update_period_ = config.GetValue(
            "running-ui.health-update-period", 0);

    filling_ration_health_change_prob_ = config.GetValue(
            "running-ui.filling-ration-health-p", 0.0);
    normal_ration_health_change_prob_  = config.GetValue(
            "running-ui.normal-ration-health-p", 0.0);
    half_ration_health_change_prob_    = config.GetValue(
            "running-ui.half-ration-health-p", 0.0);

    ss_mvmt_period_       = config.GetValue("running-ui.ss-mvmt-period",
                                            0);
    ss_food_usage_period_ = config.GetValue(
            "running-ui.ss-food-usage-period", 0);
    starve_period_        = config.GetValue("running-ui.ss-starve-period",
                                            0);

    ss_half_rations_    = config.GetValue("running-ui.ss-half-rations", 0);
    ss_normal_rations_  = config.GetValue("running-ui.ss-normal-rations",
                                          0);
    ss_filling_rations_ = config.GetValue("running-ui.ss-filling-rations",
                                          0);

    pause_tasks_[MenuOptions::MAIN].emplace('1', [this] {
        if ( !situation_manager_->IsEngineFailure()) {
            menu_options_ = MenuOptions::VELOCITY_CHANGE;
        }
    });
    pause_tasks_[MenuOptions::MAIN].emplace('2',
                                            [this] { menu_options_ = MenuOptions::RATION_CHANGE; });
    pause_tasks_[MenuOptions::MAIN].emplace('3',
                                            [this] { nav_manager_->TogglePowerMode(); });
    pause_tasks_[MenuOptions::MAIN].emplace('4', [this] {
        if ( situation_manager_->CanFixMinorIgnoredIssue()) {
            running_state_ = RunningState::FLYING;
        }
    });

    pause_tasks_[MenuOptions::VELOCITY_CHANGE].emplace('1', [this] {
        UpdateVelocity(Velocity::STOP);
    });
    pause_tasks_[MenuOptions::VELOCITY_CHANGE].emplace('2', [this] {
        UpdateVelocity(Velocity::SLOW);
    });
    pause_tasks_[MenuOptions::VELOCITY_CHANGE].emplace('3', [this] {
        UpdateVelocity(Velocity::MODERATE);
    });
    pause_tasks_[MenuOptions::VELOCITY_CHANGE].emplace('4', [this] {
        UpdateVelocity(Velocity::FAST);
    });
    pause_tasks_[MenuOptions::VELOCITY_CHANGE].emplace('5', [this] {
        UpdateVelocity(Velocity::DANGEROUS);
    });

    pause_tasks_[MenuOptions::RATION_CHANGE].emplace('1', [this] {
        ss_food_usage_period_ = ss_half_rations_;
        rations_              = "Half";
        menu_options_         = MenuOptions::MAIN;
    });
    pause_tasks_[MenuOptions::RATION_CHANGE].emplace('2', [this] {
        ss_food_usage_period_ = ss_normal_rations_;
        rations_              = "Normal";
        menu_options_         = MenuOptions::MAIN;
    });
    pause_tasks_[MenuOptions::RATION_CHANGE].emplace('3', [this] {
        ss_food_usage_period_ = ss_filling_rations_;
        rations_              = "Filling";
        menu_options_         = MenuOptions::MAIN;
    });
}

void RunningUI::Init () {
    logger_->debug("Running UI init");
    spaceship_ = spaceship_handler_->GetSpaceship();
    logger_->debug("Got spaceship");
    pause_menu_ = std::make_shared< PauseMenu >(spaceship_,
                                                nav_manager_,
                                                main_);
    logger_->debug("Created pause menu");
    situation_manager_ = std::make_unique< SituationManager >(main_,
                                                              spaceship_,
                                                              pause_menu_);
    logger_->debug("Created situation manager");
    logger_->debug("Running UI init finished");
}

// FIXME please
void RunningUI::ProcessInput () {

    int c = listener_->GetCh();

    switch ( running_state_ ) {
        case RunningState::DEPARTING:
            break;
        case RunningState::FLYING: {
            switch ( c ) {
                case 32:
                    Pause();
                    break;
                case 't':
                    Store();
                    break;
                default:
                    break;
            }
            break;
        }
        case RunningState::STORE: {
            switch ( c ) {
                case 't':
                    ExitStore();
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
                case '1':
                case '2':
                case '3':
                case '4':
                case '5': {
                    auto f_it = pause_tasks_[menu_options_].find(c);
                    if ( f_it != pause_tasks_[menu_options_].end()) {
                        f_it->second();
                    }
                    break;
                }
                default:
                    break;
            }

            break;
        }
        case RunningState::SITUATION: {

            if ( situation_manager_->ProcessInput(c)) {
                running_state_ = RunningState::FLYING;
            }

            break;
        }
    }
}

void RunningUI::Pause () {
    running_state_ = RunningState::PAUSED;
    menu_options_  = MenuOptions::MAIN;
    spaceship_->Pause();
    logger_->debug("Pausing");
}

void RunningUI::Store () {
    running_state_ = RunningState::STORE;
    spaceship_->Pause();
    logger_->debug("Store");
}

void RunningUI::ExitStore () {
    running_state_ = RunningState::FLYING;
    spaceship_->Unpause();
    logger_->debug("Exiting store");
}

void RunningUI::Unpause () {
    spaceship_->Unpause();
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

    if ( running_state_ != RunningState::DEPARTING ) {
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
    }

    if ( nav_manager_->GetDistanceRemaining() <= 0 ) {
        return GameState::EXITING;
    }

    switch ( running_state_ ) {
        case RunningState::DEPARTING: {

            running_state_ = RunningState::FLYING;
            spaceship_->Unpause(); // sets state correctly
            break;

//            if ( spaceship_handler_->PrintDeparture(main_)) {
//                logger_->debug("Done departing");
//                running_state_ = RunningState::FLYING;
//                spaceship_->Unpause(); // sets state correctly
//            }
//            break;
        }
        case RunningState::FLYING: {
            if ( situation_manager_->CheckNewSituation()) {
                running_state_ = RunningState::SITUATION;
                logger_->debug("New situation");
            }

            spaceship_handler_->PrintSpaceship(main_);

            situation_manager_->FixMinorIgnored();

            StandardLoopUpdate();

            break;
        }
        case RunningState::STORE: {
            break;
        }
        case RunningState::PAUSED: {
            pause_menu_->OnLoop(menu_options_,
                                situation_manager_->GetSituationType(),
                                situation_manager_->GetIgnoredFailures());

            spaceship_handler_->PrintSpaceship(main_);
            break;
        }
        case RunningState::SITUATION: {
            if ( situation_manager_->UpdateSituation()) {
                running_state_ = RunningState::FLYING;
            } else {
                StandardLoopUpdate();
            }

            spaceship_handler_->PrintSpaceship(main_);
            break;
        }
    }

    if ( running_state_ != RunningState::PAUSED &&
         pause_menu_->HasNotifications()) {
        Pause();
        return GameState::RUNNING; // clear queue before exiting
    } else if ( running_state_ == RunningState::PAUSED ) {
        return GameState::RUNNING;
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

void RunningUI::UpdateCrew () {

    if ( ret_state == GameState::EXITING ) {
        return;
    }

    if ( health_update_counter_++ > health_update_period_ ) {
        situation_manager_->UpdateHealth();
        spaceship_->RemoveDeadCrew(); // ensure notifications don't overlap
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

void RunningUI::MoveFlyingObject () {

    if ( !flying_debris_ && Random::get< bool >(flying_object_prob_)) {
        logger_->debug("Creating new flying object");
        flying_debris_ = std::make_unique< FlyingDebris >(main_);
    } else {
        if ( !flying_debris_ ) { return; }

        if ( flying_debris_->IsDone()) {
            logger_->debug("Destroying new flying object");
            flying_debris_.reset();
            return;
        }

        flying_debris_->Move();
        flying_debris_->Render();
    }
}


}
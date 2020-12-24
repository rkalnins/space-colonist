//
// Created by Roberts Kalnins on 22/12/2020.
//

#include "nav_control_manager.h"

#include <utility>


namespace sc::play {

NavigationControlManager::NavigationControlManager (
        const std::string &name, TaskType taskType,
        std::shared_ptr< SpaceshipHandler > spaceship_handler )
        : Task(name, taskType),
          spaceship_handler_(std::move(spaceship_handler)),
          logger_(CreateLogger(name)) {
}

void NavigationControlManager::Init () {
    logger_->debug("Route manager init");
}

GameState NavigationControlManager::OnLoop ( GameState state ) {
    if ( !spaceship_ ) {
        spaceship_ = spaceship_handler_->GetSpaceship();
    } else if ( state != GameState::SETUP && spaceship_->IsMoving()) {
        spaceship_->UseFuel(fuel_trickle_);
        distance_traveled_ += velocity_;
    }

    return state;
}

double NavigationControlManager::GetDistanceRemaining () const {
    return initial_distance_ - distance_traveled_;
}

double NavigationControlManager::GetInitialDistance () const {
    return initial_distance_;
}

void
NavigationControlManager::SetInitialDistance ( double initial_distance ) {
    initial_distance_ = initial_distance;
}

double NavigationControlManager::GetVelocity () const {
    return velocity_;
}

void NavigationControlManager::SetVelocity ( Velocity velocity ) {
    double temp_vel;

    switch ( velocity ) {
        case Velocity::STOP:
            temp_vel = stop_vel_;
            break;
        case Velocity::SLOW:
            temp_vel = slow_vel_;
            break;
        case Velocity::MODERATE:
            temp_vel = moderate_vel_;
            break;
        case Velocity::FAST:
            temp_vel = fast_vel_;
            break;
        case Velocity::DANGEROUS:
            temp_vel = dangerous_vel_;
            break;
    }

    if ( temp_vel * velocity_to_fuel_ratio_ > spaceship_->GetFuel()) {
        return;
    }


    velocity_state_ = velocity;
    velocity_       = temp_vel;
    spaceship_->UseFuel(velocity_ * velocity_to_fuel_ratio_);

    stop_fuel_      = std::abs(( stop_vel_ - velocity_ )) *
                      velocity_to_fuel_ratio_;
    slow_fuel_      = std::abs(( slow_vel_ - velocity_ )) *
                      velocity_to_fuel_ratio_;
    moderate_fuel_  = std::abs(( moderate_vel_ - velocity_ )) *
                      velocity_to_fuel_ratio_;
    fast_fuel_      = std::abs(( fast_vel_ - velocity_ )) *
                      velocity_to_fuel_ratio_;
    dangerous_fuel_ = std::abs(( dangerous_vel_ - velocity_ )) *
                      velocity_to_fuel_ratio_;

}

double NavigationControlManager::GetStopFuel () const {
    return stop_fuel_;
}

double NavigationControlManager::GetSlowFuel () const {
    return slow_fuel_;
}

double NavigationControlManager::GetModerateFuel () const {
    return moderate_fuel_;
}

double NavigationControlManager::GetFastFuel () const {
    return fast_fuel_;
}

double NavigationControlManager::GetDangerousFuel () const {
    return dangerous_fuel_;
}

bool NavigationControlManager::IsStopped () const {
    return velocity_state_ == Velocity::STOP;
}

void NavigationControlManager::TogglePowerMode () {
    switch ( power_mode_ ) {
        case PowerMode::NORMAL:
            power_mode_   = PowerMode::LOW;
            fuel_trickle_ = low_e_trickle_;
            break;
        case PowerMode::LOW:
            power_mode_   = PowerMode::NORMAL;
            fuel_trickle_ = nominal_trickle_;
            break;
    }
}

bool NavigationControlManager::LowPowerEnabled () {
    return power_mode_ == PowerMode::LOW;
}

}
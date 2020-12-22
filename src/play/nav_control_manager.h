//
// Created by Roberts Kalnins on 22/12/2020.
//

#pragma once

#include "../loop_control/task.h"
#include "../logging/logging.h"
#include "spaceship_handler.h"
#include "spaceship.h"


namespace sc::play {

enum class Velocity { STOP, SLOW, MODERATE, FAST, DANGEROUS };
enum class PowerMode { NORMAL, LOW };


class NavigationControlManager : public Task {
  public:

    NavigationControlManager ( const std::string &name, TaskType taskType,
                               std::shared_ptr< SpaceshipHandler > spaceship_handler );

    void Init () override;

    GameState OnLoop ( GameState state ) override;

    [[nodiscard]] double GetDistanceRemaining () const;

    [[nodiscard]] double GetInitialDistance () const;

    void SetInitialDistance ( double initial_distance );

    void SetVelocity ( Velocity velocity );

    [[nodiscard]] bool IsStopped () const;

    [[nodiscard]] double GetStopFuel () const;

    [[nodiscard]] double GetSlowFuel () const;

    [[nodiscard]] double GetModerateFuel () const;

    [[nodiscard]] double GetFastFuel () const;

    [[nodiscard]] double GetDangerousFuel () const;

    [[nodiscard]] double GetVelocity () const;

    void TogglePowerMode ();

    bool LowPowerEnabled ();

  private:

    std::shared_ptr< SpaceshipHandler > spaceship_handler_ { nullptr };
    std::shared_ptr< Spaceship >        spaceship_ { nullptr };

    logger_t logger_;

    Velocity  velocity_state_ { Velocity::SLOW };
    PowerMode power_mode_ { PowerMode::NORMAL };

    double       fuel_trickle_    = 0.0004; // trickle
    const double low_e_trickle_   = 0.00015; // trickle
    const double nominal_trickle_ = 0.0003; // trickle

    const double velocity_to_fuel_ratio_ = 1000;

    const double stop_vel_      = 0.0;
    const double slow_vel_      = 0.0005;
    const double moderate_vel_  = 0.001;
    const double fast_vel_      = 0.005;
    const double dangerous_vel_ = 0.01;

    double stop_fuel_ {};
    double slow_fuel_ {};
    double moderate_fuel_ {};
    double fast_fuel_ {};
    double dangerous_fuel_ {};

    double initial_distance_ { 0 };
    double distance_traveled_ { 0 };
    double velocity_ { 0 };
};

}





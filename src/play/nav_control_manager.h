//
// Created by Roberts Kalnins on 22/12/2020.
//

#pragma once

#include "../loop_control/task.h"
#include "../logging/logging.h"
#include "spaceship_handler.h"
#include "spaceship.h"
#include "../config/config.h"


namespace sc::play {

enum class Velocity { STOP, SLOW, MODERATE, FAST, DANGEROUS };
enum class PowerMode { NORMAL, LOW };


class NavigationControlManager : public Task {
  public:

    NavigationControlManager ( const std::string &name, TaskType taskType,
                               shared_spaceship_handler_t spaceship_handler );

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

    shared_spaceship_handler_t spaceship_handler_ { nullptr };
    shared_spaceship_t         spaceship_ { nullptr };

    logger_t logger_;

    Velocity  velocity_state_ { Velocity::SLOW };
    PowerMode power_mode_ { PowerMode::NORMAL };

    double fuel_trickle_;
    double low_e_trickle_;
    double nominal_trickle_;

    double velocity_to_fuel_ratio_;
    double stop_vel_;
    double slow_vel_;
    double moderate_vel_;
    double fast_vel_;
    double dangerous_vel_;

    double stop_fuel_ {};
    double slow_fuel_ {};
    double moderate_fuel_ {};
    double fast_fuel_ {};
    double dangerous_fuel_ {};

    double initial_distance_ { 0 };
    double distance_traveled_ { 0 };
    double velocity_ { 0 };

    bool reversed { false };
};


using shared_nav_manager_t = std::shared_ptr< play::NavigationControlManager >;

}





//
// Created by Roberts Kalnins on 20/12/2020.
//

#pragma once

#include <map>

#include "../loop_control/task.h"
#include "../logging/logging.h"
#include "spaceship_handler.h"
#include "spaceship_factory.h"


namespace sc::play {

enum class RunningState {
    FLYING,
    PAUSED,
    SITUATION,
};

enum class SituationType {
    NONE,
    MINOR,
    ENGINE_FAILURE,
    AIR_FILTER_FAILURE,
    GYROSCOPE_FAILURE,
    MINOR_HULL_BREACH,
    MAJOR_HULL_BREACH,
    APPROACHING_SHIP,
};

enum class MenuOptions {
    MAIN,
    VELOCITY_CHANGE,
    RATION_CHANGE
};


class RunningUI : public Task {

  public:

    RunningUI ( const std::string &name, TaskType taskType,
                std::shared_ptr< SpaceshipHandler > spaceship_handler,
                std::shared_ptr< InputListener > listener,
                std::shared_ptr< SpaceshipFactory > spaceship_factory,
                WINDOW *main );

    void Init () override;

    GameState OnLoop ( GameState state ) override;

  private:

    void MoveSpaceship ();

    void BoundSpaceshipPosition ();

    void ProcessInput ();

    void UpdateSpaceshipState ();

    void UpdateCrew ();

    void UpdateCrewFood ();

    void ShowPauseOptions ();

    void Pause ();

    void ShowVelocityChangeOptions ();

    void ShowChangeRationsOptions ();

    bool IsSituation ();

    bool UpdateVelocity ( double new_velocity );

    void ShowSituationReport ();

    void AttemptFix ();

    void FixMinor ();

    void FixMinorIgnored ();

  private:

    logger_t logger_;

    std::shared_ptr< SpaceshipHandler > spaceship_handler_;

    std::shared_ptr< SpaceshipFactory > spaceship_factory_;

    std::shared_ptr< InputListener > listener_;

    WINDOW *main_;

    std::string appearance_code_ {};

    const double minor_failure_prob_ { 0.0008 };
    double       major_failure_prob_ { 0.0003 };
    const double unresponsive_engines_ { 0.0001 };

    int    ignored_minor_mech_failures_ { 0 };
    double minor_fix_prob_ { 0.01 };
    double major_fix_prob_ { 0.01 };

    bool fixing_ { false };
    bool fixing_minor_ { false };

    std::map< SituationType, std::vector< std::string>> sitrep_options_ {
            { SituationType::MINOR,              { "1. Ignore", "2. Attempt fix" }},
            { SituationType::ENGINE_FAILURE,     { "1. Attempt fix" }},
            { SituationType::AIR_FILTER_FAILURE, { "1. Attempt fix" }}
    };

    std::vector< const std::string > minor_failures_ = {
            "Toilet control shorting out",
            "Bridge door control browning out",
            "Loose cabling below deck", "Minor coolant leak",
            "Dirty air filter", "Exception thrown by navigation software",
            "Flickering cargo hold power", "High amp draw by HVAC system",
            "Unstable voltage in emergency breach containment system"
    };

    std::vector< const std::string > engine_failures_ = {
            "Engine coolant system failed. Cannot fire engines.",
            "Engine fuel leak. Cannot fire engines.",
            "Engine control system failed. Cannot fire engines.",
    };

    std::vector< const std::string > air_filter_failure_ = {
            "Air filter pump failed. Cannot recycle Oxygen.",
            "Air filter clogged. Cannot recycle Oxygen",
            "Air filter unresponsive. Cannot recycle Oxygen.",
    };


    const std::string distance_remaining_ = "Distance remaining: ";
    const std::string velocity_name_      = "Velocity: ";

    const int dist_disp_y_ { 20 };
    const int dist_disp_x_ { 4 };

    const int health_update_period_ { 200 };
    int       health_update_counter_ { 0 };

    const double filling_ration_health_change_prob_ { 0.6 };
    const double normal_ration_health_change_prob_ { 0.4 };
    const double half_ration_health_change_prob_ { 0.2 };

    const int ss_mvmt_period_ { 80 };
    int       ss_food_usage_period_ { 120 };
    const int starve_period_ { 100 };

    const int ss_half_rations_ { 240 };
    const int ss_normal_rations_ { 120 };
    const int ss_filling_rations_ { 80 };

    std::string rations_ = "Normal";

    int ss_food_usage_counter_ { 0 };

    int ss_mvmt_counter_ { 0 };

    double ss_mvmt_x_prob_ { 0.75 };
    double ss_mvmt_y_prob_ { 0.75 };

    double       fuel_trickle_    = 0.0001; // trickle
    const double low_e_trickle_   = 0.00015; // trickle
    const double nominal_trickle_ = 0.0003; // trickle
    bool         trickle_nominal_ { true };

    const double velocity_to_fuel_ratio_ = 1000;
    double       velocity_               = 0.000;

    const double stop_      = 0.0;
    const double slow_      = 0.0005;
    const double moderate_  = 0.001;
    const double fast_      = 0.005;
    const double dangerous_ = 0.01;

    const int ss_max_y_ { 26 };
    const int ss_min_y_ { 24 };
    const int ss_max_x_ { 55 };
    const int ss_min_x_ { 45 };

    int ss_pos_y_ { 25 };
    int ss_pos_x_ { 50 };

    std::queue< std::string >       notifications_;
    std::queue< const std::string > situations_;

    const int pause_y_ { 32 };
    const int pause_x_ { 30 };

    bool notified_no_food_ = false;

    GameState     ret_state { GameState::RUNNING };
    MenuOptions   menu_options_ { MenuOptions::MAIN };
    RunningState  running_state_ { RunningState::FLYING };
    SituationType situation_type_ { SituationType::NONE };


};

}





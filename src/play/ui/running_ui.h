//
// Created by Roberts Kalnins on 20/12/2020.
//

#pragma once

#include <map>
#include <effolkronium/random.hpp>

#include "../../loop_control/task.h"
#include "../../logging/logging.h"
#include "../spaceship_handler.h"
#include "../spaceship_factory.h"
#include "../nav_control_manager.h"
#include "../flying_debris.h"


namespace sc::play {

using Random = effolkronium::random_static;

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
                std::shared_ptr< play::NavigationControlManager > nav_manager,
                std::shared_ptr< InputListener > listener,
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

    bool UpdateVelocity ( Velocity new_velocity );

    void ShowSituationReport ();

    void AttemptFix ();

    void FixMinor ();

    void FixMinorIgnored ();

    bool UseGenericSpareParts ();

    void WaitForHelp ();

    void UpdateAllCrewHealth ();

    void MoveFlyingObject ();

    void Update ();

  private:

    logger_t logger_;

    std::shared_ptr< NavigationControlManager > nav_manager_ { nullptr };

    std::shared_ptr< SpaceshipHandler > spaceship_handler_ { nullptr };
    std::shared_ptr< Spaceship >        spaceship_ { nullptr };
    std::shared_ptr< InputListener >    listener_ { nullptr };
    std::unique_ptr< FlyingDebris >     flying_debris_ { nullptr };

    WINDOW *main_;

    const double flying_object_prob_ { 0.1 };


    const double minor_failure_prob_ { 0.001 };
    double       major_failure_prob_ { 0.0002 };
    const double unresponsive_engines_ { 0.0001 };

    const double successful_distress_ { 0.008 };
    bool         waiting_for_help_ { false };

    int    ignored_minor_mech_failures_ { 0 };
    double minor_fix_prob_ { 0.01 };
    double major_fix_prob_ { 0.003 };

    bool enough_spares_ { true };

    const std::vector< int > components_used_minor_ = { 1, 2 };
    const std::vector< int > cabling_used_minor_    = { 1, 2 };
    const std::vector< int > components_used_major_ = { 2, 3, 5 };
    const std::vector< int > cabling_used_major_    = { 2, 3, 5 };

    int req_components_ { 0 };
    int req_cabling_ { 0 };

    bool fixing_ { false };
    bool fixing_minor_ { false };

    const int second_count_period_ { 60 };
    int       situation_counter_ { 0 };

    const int air_response_time_ = 8;

    bool air_is_poisoned_ { false };

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

    std::queue< std::string >       notifications_;
    std::queue< const std::string > situations_;

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


    const int pause_y_ { 32 };
    const int pause_x_ { 30 };

    bool notified_no_food_ = false;

    GameState     ret_state { GameState::RUNNING };
    MenuOptions   menu_options_ { MenuOptions::MAIN };
    RunningState  running_state_ { RunningState::FLYING };
    SituationType situation_type_ { SituationType::NONE };
};

}





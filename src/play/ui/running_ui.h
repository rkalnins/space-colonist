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
#include "../situation/situation_type.h"
#include "pause_menu.h"
#include "../situation/situation_manager.h"
#include "../menu_options.h"


namespace sc::play {

using Random = effolkronium::random_static;

enum class RunningState {
    FLYING,
    PAUSED,
    SITUATION,
};


class RunningUI : public Task {

  public:

    RunningUI ( const std::string &name, TaskType taskType,
                shared_spaceship_handler_t spaceship_handler,
                shared_nav_manager_t nav_manager,
                shared_input_listener_t listener,
                WINDOW *main );

    void Init () override;

    GameState OnLoop ( GameState state ) override;

  private:

    void MoveSpaceship ();

    void ProcessInput ();

    void UpdateSpaceshipState ();

    void UpdateCrew ();

    void UpdateCrewFood ();

    void Pause ();

    bool UpdateVelocity ( Velocity new_velocity );

    void MoveFlyingObject ();

    void StandardLoopUpdate ();

    void Unpause ();

  private:

    logger_t logger_;

    std::shared_ptr< PauseMenu > pause_menu_ { nullptr };

    shared_nav_manager_t nav_manager_ { nullptr };

    shared_spaceship_handler_t spaceship_handler_ { nullptr };
    shared_spaceship_t         spaceship_ { nullptr };
    shared_input_listener_t    listener_ { nullptr };

    std::unique_ptr< FlyingDebris >     flying_debris_ { nullptr };
    std::unique_ptr< SituationManager > situation_manager_ { nullptr };

    WINDOW *main_;

    const double flying_object_prob_ { 0.1 };

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

    bool notified_no_food_ = false;

    GameState    ret_state { GameState::RUNNING };
    MenuOptions  menu_options_ { MenuOptions::MAIN };
    RunningState running_state_ { RunningState::FLYING };
};

}





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
    DEPARTING,
    FLYING,
    PAUSED,
    SITUATION,
    STORE,
    ARRIVING
};

using pause_menu_tasks_t = std::map< MenuOptions, std::map< int, std::function< void ()>> >;


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

    void ProcessInput ();

    void UpdateSpaceshipState ();

    void UpdateCrew ();

    void UpdateCrewFood ();

    void Pause ();

    void Store ();

    void ExitStore ();

    bool UpdateVelocity ( Velocity new_velocity );

    void MoveFlyingObject ();

    void StandardLoopUpdate ();

    void Unpause ();

  private:

    pause_menu_tasks_t pause_tasks_;

    logger_t logger_;

    std::shared_ptr< PauseMenu > pause_menu_ { nullptr };

    shared_nav_manager_t nav_manager_ { nullptr };

    shared_spaceship_handler_t spaceship_handler_ { nullptr };
    shared_spaceship_t         spaceship_ { nullptr };
    shared_input_listener_t    listener_ { nullptr };

    std::unique_ptr< FlyingDebris >     flying_debris_ { nullptr };
    std::unique_ptr< SituationManager > situation_manager_ { nullptr };

    WINDOW *main_;

    const std::string distance_remaining_ = "Distance remaining: ";
    const std::string velocity_name_      = "Velocity: ";

    double flying_object_prob_;

    int dist_disp_y_;
    int dist_disp_x_;

    int health_update_period_;

    double filling_ration_health_change_prob_;
    double normal_ration_health_change_prob_;
    double half_ration_health_change_prob_;

    int ss_food_usage_period_;
    int starve_period_;

    int ss_half_rations_;
    int ss_normal_rations_;
    int ss_filling_rations_;


    std::string rations_ = "Normal";

    int health_update_counter_ { 0 };
    int ss_food_usage_counter_ { 0 };
    int ss_mvmt_counter_ { 0 };

    bool notified_no_food_ = false;

    GameState    ret_state { GameState::RUNNING };
    MenuOptions  menu_options_ { MenuOptions::MAIN };
    RunningState running_state_ { RunningState::DEPARTING };
};

}





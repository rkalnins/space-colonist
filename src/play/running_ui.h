//
// Created by Roberts Kalnins on 20/12/2020.
//

#pragma once

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
    ENGINE_FAILURE,
    AIR_FILTER_FAILURE,
    GYROSCOPE_FAILURE,
    MINOR_HULL_BREACH,
    MAJOR_HULL_BREACH,
    APPROACHING_SHIP,
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

  private:

    logger_t logger_;

    std::shared_ptr< SpaceshipHandler > spaceship_handler_;

    std::shared_ptr< SpaceshipFactory > spaceship_factory_;

    std::shared_ptr< InputListener > listener_;

    WINDOW *main_;

    std::string appearance_code_ {};

    const std::string distance_remaining_ = "Distance remaining: ";
    const int dist_disp_y_ { 20 };
    const int dist_disp_x_ { 4 };

    const int ss_mvmt_counter_max_ { 80 };
    int       ss_mvmt_counter_ { 0 };
    double    ss_mvmt_x_prob_ { 0.75 };
    double    ss_mvmt_y_prob_ { 0.75 };

    double fuel_usage_        = 0.001;
    double distance_traveled_ = 0.001;

    const int ss_max_y_ { 28 };
    const int ss_min_y_ { 22 };
    const int ss_max_x_ { 55 };
    const int ss_min_x_ { 45 };

    int ss_pos_y_ { 25 };
    int ss_pos_x_ { 50 };

    RunningState  running_state_ { RunningState::FLYING };
    SituationType situation_type_ { SituationType::NONE };


};

}





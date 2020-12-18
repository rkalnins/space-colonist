//
// Created by Roberts Kalnins on 17/12/2020.
//

#pragma once

#include "../loop_control/task.h"
#include "objects/spaceship.h"
#include "../input_listener.h"
#include "../logging/logging.h"


namespace sc::play {

class SpaceshipHandler : public Task {
  public:
    SpaceshipHandler ( const std::string &name,
                       TaskType taskType,
                       std::shared_ptr< InputListener > listener,
                       WINDOW *main,
                       std::shared_ptr< Spaceship > spaceship );

  public:
    void Init () override;

    GameState OnLoop () override;

    bool IsFinished () override;

  private:
    void PrintCrew ();

    void PrintItems ();

    void PrintHUD ();

    void ProcessInput ();

  private:

    std::shared_ptr< play::Spaceship > spaceship_ { nullptr };

    std::shared_ptr< InputListener > listener_ { nullptr };

    WINDOW *main_;

    MEVENT   mouse_event_ {};
    logger_t logger_;

    bool show_crew_ { false };
    bool show_items_ { false };

    int item_page_id_ { 0 };
    int cat_per_page_ = 10;


    const int spaceship_display_x_ = 10;
    const int spaceship_display_y_ = 4;

    double fuel_use_ { 0.01 };

};

}





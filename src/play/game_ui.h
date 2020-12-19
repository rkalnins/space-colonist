//
// Created by Roberts Kalnins on 18/12/2020.
//

#pragma once

#include <ncurses.h>
#include <vector>
#include <queue>

#include "items/item.h"
#include "objects/crew_member.h"
#include "objects/spaceship.h"
#include "../input_listener.h"
#include "../logging/logging.h"
#include "../loop_control/task.h"
#include "objects/crew_factory.h"


namespace sc::play {

enum class UIState {
    DESTINATION_SELECTION,
    CREW_SELECTION,
    INVENTORY_SELECTION,
    SPACESHIP_SETUP,
    TRADING,
    FIGHTING,
    MECHANICAL_FAILURE,
    MAP
};


class GameUI : public Task {


  public:

    GameUI ( const std::string &name, TaskType taskType,
             std::shared_ptr< Spaceship > spaceship,
             std::shared_ptr< InputListener > listener,
             WINDOW *main );

    void Init () override;

    GameState OnLoop () override;

  private:

    void ProcessMouseCheckboxInput ( MousePosition &mpos );

    void DestinationSelection();

    void CrewSelection();



  private:
    logger_t logger_;

    CrewMemberFactory crew_member_factory_;

    std::vector< CrewMember > crew_choices_;

    std::shared_ptr< Spaceship > spaceship_ { nullptr };

    std::shared_ptr< InputListener > listener_ { nullptr };

    WINDOW *main_;
    MEVENT mouse_event_ {};

    struct Checkbox {
        int  y;
        int  left_x;
        int  right_x;
        bool checked;
    };

    std::vector< Checkbox > selected_crew_;
    std::queue<size_t>  select_order_;

    UIState state_ { UIState::DESTINATION_SELECTION };

    const int ui_init_y_          = 20;
    const int ui_init_x_          = 4;
    const int crew_choices_count_ = 5;
    const int crew_select_max_    = 3;
};

}



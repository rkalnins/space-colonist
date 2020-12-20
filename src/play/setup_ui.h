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
#include "spaceship_handler.h"
#include "../input_listener.h"
#include "../logging/logging.h"
#include "../loop_control/task.h"
#include "objects/crew_factory.h"
#include "spaceship_factory.h"
#include "map_generator.h"


namespace sc::play {

enum class SetupState {
    SPACESHIP_SETUP,
    DESTINATION_SELECTION,
    CREW_SELECTION,
    INVENTORY_SELECTION,
};


class SetupUI : public Task {


  public:

    SetupUI ( const std::string &name, TaskType taskType,
              std::shared_ptr< SpaceshipHandler > spaceship_handler,
              std::shared_ptr< InputListener > listener,
              WINDOW *main );

    void Init () override;

    GameState OnLoop () override;

  private:

    void ProcessMouseCheckboxInput ( MousePosition &mpos );

    void DestinationSelection ();

    void CrewSelection ();

    void SpaceshipSelection ();

    void InventorySelection ();

  private:
    logger_t logger_;

    CrewMemberFactory crew_member_factory_;
    SpaceshipFactory  spaceship_factory_;

    std::vector< CrewMember >                   crew_choices_;
    std::vector< std::shared_ptr< Spaceship > > spaceship_choices_;

    std::shared_ptr< SpaceshipHandler > spaceship_handler_ { nullptr };

    std::shared_ptr< InputListener > listener_ { nullptr };

    std::unique_ptr< MapGenerator > map_generator_ { nullptr };

    WINDOW *main_;

    std::vector< Checkbox > selected_crew_;
    std::queue< size_t >    select_order_;

    SetupState state_ { SetupState::SPACESHIP_SETUP };

    const int ui_init_y_          = 20;
    const int ui_init_x_          = 4;
    const int crew_choices_count_ = 5;


    const int spaceship_choice_count_ { 3 };
    int       selected_spaceship_ { 0 };

    void ProcessMousePlanetSelect ( MousePosition &mpos );
};

}



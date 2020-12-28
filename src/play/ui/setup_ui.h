//
// Created by Roberts Kalnins on 18/12/2020.
//

#pragma once

#include <ncurses.h>
#include <vector>
#include <map>
#include <queue>

#include "../item.h"
#include "../crew_member.h"
#include "../spaceship_handler.h"
#include "../../input_listener.h"
#include "../../logging/logging.h"
#include "../../loop_control/task.h"
#include "../crew_factory.h"
#include "../spaceship_factory.h"
#include "../space_map.h"
#include "../nav_control_manager.h"
#include "../../config/item_source.h"


namespace sc::play {

enum class SetupState {
    SPACESHIP_SETUP,
    DESTINATION_SELECTION,
    CREW_SELECTION,
    INVENTORY_SELECTION,
    DONE,
};

enum class TradingPostCategory {
    ALL,
    TOOLS,
    SPARE_PARTS,
    FUEL,
    WEAPONS,
    FOOD,
    SUPPLIES,
    INFRASTRUCTURE
};


class SetupUI : public Task {
  public:

    SetupUI ( const std::string &name, TaskType taskType,
              shared_spaceship_handler_t spaceship_handler,
              shared_nav_manager_t nav_manager_,
              shared_input_listener_t listener,
              shared_spaceship_factory_t spaceship_factory,
              WINDOW *main );

    void Init () override;

    GameState OnLoop ( GameState state ) override;

    bool IsFinished () override;

  private:

    void ProcessMouseCheckboxInput ( MousePosition &mpos );

    void DestinationSelection ();

    void CrewSelection ();

    void SpaceshipSelection ();

    void InventorySelection ();

    void ProcessMousePlanetSelect ( MousePosition &mpos );

    static std::string GetCategoryStr ( TradingPostCategory category );

  private:
    logger_t logger_;

    CrewMemberFactory          crew_member_factory_;
    shared_spaceship_factory_t spaceship_factory_;

    std::vector< CrewMember > crew_choices_;

    shared_nav_manager_t nav_manager_ {
            nullptr
    };

    std::vector< shared_spaceship_t > spaceship_choices_;

    shared_spaceship_handler_t  spaceship_handler_ { nullptr };
    shared_input_listener_t     listener_ { nullptr };
    std::unique_ptr< SpaceMap > map_generator_ { nullptr };

    WINDOW *main_;

    std::vector< Checkbox > selected_crew_;
    std::queue< size_t >    select_order_;

    SetupState state_ { SetupState::SPACESHIP_SETUP };

    TradingPostCategory trading_post_view_ { TradingPostCategory::ALL };

    std::string current_category_;
    int         current_selected_item_ {};

    std::map< std::string, items_vector_ptr_t > items_for_sale_;

    int ui_init_y_;
    int ui_init_x_;

    int crew_choices_count_;
    int spaceship_choice_count_;

    int selected_spaceship_ { 0 };
};

}



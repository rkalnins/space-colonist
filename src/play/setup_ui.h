//
// Created by Roberts Kalnins on 18/12/2020.
//

#pragma once

#include <ncurses.h>
#include <vector>
#include <map>
#include <queue>

#include "items/item.h"
#include "objects/crew_member.h"
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
              std::shared_ptr< SpaceshipHandler > spaceship_handler,
              std::shared_ptr< InputListener > listener,
              std::shared_ptr< SpaceshipFactory > spaceship_factory,
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

    CrewMemberFactory                   crew_member_factory_;
    std::shared_ptr< SpaceshipFactory > spaceship_factory_;

    std::vector< CrewMember > crew_choices_;

    std::vector< std::shared_ptr< Spaceship > > spaceship_choices_;

    std::shared_ptr< SpaceshipHandler > spaceship_handler_ { nullptr };
    std::shared_ptr< InputListener >    listener_ { nullptr };
    std::unique_ptr< MapGenerator >     map_generator_ { nullptr };

    WINDOW *main_;

    std::vector< Checkbox > selected_crew_;
    std::queue< size_t >    select_order_;

    SetupState state_ { SetupState::SPACESHIP_SETUP };

    TradingPostCategory trading_post_view_ { TradingPostCategory::ALL };

    std::string current_category_;
    int         current_selected_item_ {};

    // FIXME
    std::map< std::string, std::vector< std::shared_ptr< Item > > > items_for_sale_ = {
            {
                    "Tools",          {
                                              std::make_shared< Item >(
                                                      "Tools",
                                                      "Basic toolset", 2,
                                                      15, 20),
                                              std::make_shared< Item >(
                                                      "Tools",
                                                      "Standard toolset",
                                                      2,
                                                      20, 25),
                                              std::make_shared< Item >(
                                                      "Tools",
                                                      "Enhanced toolset",
                                                      2,
                                                      25, 35),
                                              std::make_shared< Item >(
                                                      "Tools",
                                                      "Full toolset", 1,
                                                      45, 50)
                                      }
            },
            {
                    "Spare parts",    {
                                              std::make_shared< Item >(
                                                      "Spare parts",
                                                      "Engine", 3,
                                                      30, 90),
                                              std::make_shared< Item >(
                                                      "Spare parts",
                                                      "Gyroscope", 1,
                                                      20, 120),
                                              std::make_shared< Item >(
                                                      "Spare parts",
                                                      "Cables", 20,
                                                      1, 1),
                                              std::make_shared< Item >(
                                                      "Spare parts",
                                                      "Communications", 3,
                                                      15, 75),
                                              std::make_shared< Item >(
                                                      "Spare parts",
                                                      "Electronic components",
                                                      50, 1, 2),
                                              std::make_shared< Item >(
                                                      "Spare parts",
                                                      "Hull plating", 5,
                                                      20, 50)
                                      }},
            {
                    "Fuel",           {
                                              std::make_shared< Item >(
                                                      "Fuel",
                                                      "Basic Fuel", 100,
                                                      10, 20),
                                              std::make_shared< Item >(
                                                      "Fuel",
                                                      "Purified fuel", 100,
                                                      10, 30),
                                              std::make_shared< Item >(
                                                      "Fuel",
                                                      "Enhanced fuel", 100,
                                                      10, 40)
                                      }},
            {
                    "Weapons",        {

                                              std::make_shared< Item >(
                                                      "Weapons",
                                                      "Rifle", 5, 4, 20),
                                              std::make_shared< Item >(
                                                      "Weapons",
                                                      "Sidearm", 5, 2, 10),
                                              std::make_shared< Item >(
                                                      "Weapons",
                                                      "Body Armor", 5, 8,
                                                      15),
                                              std::make_shared< Item >(
                                                      "Weapons",
                                                      "Torpedoes", 10, 30,
                                                      40),
                                              std::make_shared< Item >(
                                                      "Weapons",
                                                      "Defence Cannons", 2,
                                                      150, 200),
                                              std::make_shared< Item >(
                                                      "Weapons",
                                                      "Cannon ammo", 100,
                                                      3, 2),
                                              std::make_shared< Item >(
                                                      "Weapons",
                                                      "Rifle ammo", 100,
                                                      1, 1),
                                              std::make_shared< Item >(
                                                      "Weapons",
                                                      "Sidearm ammo", 100,
                                                      1, 1)

                                      }},
            {
                    "Food",           {

                                              std::make_shared< Item >(
                                                      "Food",
                                                      "Rations", 100, 2,
                                                      2),
                                              std::make_shared< Item >(
                                                      "Food",
                                                      "Fruits", 20, 2, 5),
                                              std::make_shared< Item >(
                                                      "Food",
                                                      "Vegetables", 20, 5,
                                                      3)
                                      }},
            {
                    "Infrastructure", {
                                              std::make_shared< Item >(
                                                      "Infrastructure",
                                                      "Workshop", 1,
                                                      600, 400),
                                              std::make_shared< Item >(
                                                      "Infrastructure",
                                                      "Medical Bay", 1,
                                                      600, 500)

                                      }
            },
            {
                    "Supplies",       {
                                              std::make_shared< Item >(
                                                      "Supplies",
                                                      "Medical supplies",
                                                      10, 3, 10)

                                      }
            }
    };

    const int ui_init_y_          = 17;
    const int ui_init_x_          = 4;
    const int crew_choices_count_ = 5;


    const int spaceship_choice_count_ { 3 };
    int       selected_spaceship_ { 0 };
};

}



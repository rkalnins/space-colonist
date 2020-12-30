//
// Created by Roberts Kalnins on 29/12/2020.
//

#pragma once

#include "../spaceship.h"
#include "../spaceship_handler.h"
#include "../../config/item_source.h"
#include "../../input_listener.h"
#include "../../logging/logging.h"


namespace sc::play {

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


class InventoryUI {

  public:
    InventoryUI ( WINDOW *main, const std::string &source,
                  shared_spaceship_t spaceship,
                  shared_input_listener_t listener,
                  shared_spaceship_handler_t spaceship_handler );

    void OnLoop ();

  private:
    static std::string GetCategoryStr ( TradingPostCategory category );

    void ProcessInput ();

    void ShowInventoryInterface ();

  private:

    logger_t logger_;

    shared_spaceship_t         spaceship_;
    shared_input_listener_t    listener_;
    shared_spaceship_handler_t spaceship_handler_;

    TradingPostCategory trading_post_view_ { TradingPostCategory::ALL };

    std::string current_category_;
    int         current_selected_item_ {};

    std::map< std::string, items_vector_ptr_t > items_for_sale_;

    WINDOW *main_;

    const std::string &source_;

    int ui_init_y_;
    int ui_init_x_;
};

}




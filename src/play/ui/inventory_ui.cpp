//
// Created by Roberts Kalnins on 29/12/2020.
//

#include "inventory_ui.h"

#include <utility>


namespace sc::play {

InventoryUI::InventoryUI ( WINDOW *main, const std::string &source,
                           shared_spaceship_t spaceship,
                           shared_input_listener_t listener,
                           shared_spaceship_handler_t spaceship_handler )
        : main_(main), source_(source),
          spaceship_(std::move(spaceship)),
          listener_(std::move(listener)),
          spaceship_handler_(std::move(spaceship_handler)),
          logger_(CreateLogger("inventory")) {

    Config &config = Config::GetInstance();

    ui_init_y_ = config.GetValue("setup-ui.init-y", 0);
    ui_init_x_ = config.GetValue("setup-ui.init-x", 0);

    logger_->debug("Getting categories");
    cat_vec_t categories = ItemSource::GetInstance().GetCategories(
            source_);
    logger_->debug("Got categories");

    for ( auto &c : *categories ) {
        items_for_sale_.insert(
                std::make_pair(c, ItemSource::GetInstance().GetItems(
                        source_, c)));
    }

    logger_->debug("Created inventory selection");
}

void InventoryUI::OnLoop () {

    if ( !spaceship_ ) {
        spaceship_ = spaceship_handler_->GetSpaceship();
        logger_->debug("Got spaceship");
        return;
    }

    ProcessInput();
    ShowInventoryInterface();
}

void InventoryUI::ProcessInput () {
    switch ( listener_->GetCh()) {
        case KEY_DOWN: {
            current_selected_item_ = std::min(
                    current_selected_item_ + 1,
                    static_cast<int>(
                            items_for_sale_[current_category_]->size() -
                            1));
            break;
        }
        case KEY_UP: {
            current_selected_item_ = std::max(
                    current_selected_item_ - 1, 0);
            break;
        }
        case 'a': {
            if ( trading_post_view_ ==
                 TradingPostCategory::ALL ) { break; }

            Item item = *( *items_for_sale_[current_category_] )[current_selected_item_];
            if ( item.GetQuantity() < 10 ) { break; }
            item.SetQuantity(10);
            bool success = spaceship_handler_->GetSpaceship()->AddItem(
                    item);
            if ( success ) {
                ( *items_for_sale_[current_category_] )[current_selected_item_]->HardUpdateQuantity(
                        -10);
            }
            break;
        }
        case KEY_RIGHT: {
            if ( trading_post_view_ ==
                 TradingPostCategory::ALL ) { break; }

            Item item = *( *items_for_sale_[current_category_] )[current_selected_item_];
            if ( item.GetQuantity() < 1 ) { break; }
            item.SetQuantity(1);
            bool success = spaceship_handler_->GetSpaceship()->AddItem(
                    item);
            if ( success ) {
                ( *items_for_sale_[current_category_] )[current_selected_item_]->HardUpdateQuantity(
                        -1);
            }
            break;
        }
        case 'b': {
            if ( trading_post_view_ ==
                 TradingPostCategory::ALL ) { break; }
            Item item = *( *items_for_sale_[current_category_] )[current_selected_item_];
            item.SetQuantity(10);
            bool success = spaceship_handler_->GetSpaceship()->RemoveItem(
                    item);
            if ( success ) {
                ( *items_for_sale_[current_category_] )[current_selected_item_]->HardUpdateQuantity(
                        10);
            }
            break;
        }
        case KEY_LEFT: {
            if ( trading_post_view_ ==
                 TradingPostCategory::ALL ) { break; }
            Item item = *( *items_for_sale_[current_category_] )[current_selected_item_];
            item.SetQuantity(1);
            bool success = spaceship_handler_->GetSpaceship()->RemoveItem(
                    item);
            if ( success ) {
                ( *items_for_sale_[current_category_] )[current_selected_item_]->HardUpdateQuantity(
                        1);
            }
            break;
        }
        case '0': {
            trading_post_view_     = TradingPostCategory::ALL;
            current_selected_item_ = 0;
            break;
        }
        case '1': {
            trading_post_view_     = TradingPostCategory::FOOD;
            current_category_      = GetCategoryStr(
                    trading_post_view_);
            current_selected_item_ = 0;
            break;
        }
        case '2': {
            trading_post_view_     = TradingPostCategory::FUEL;
            current_category_      = GetCategoryStr(
                    trading_post_view_);
            current_selected_item_ = 0;
            break;
        }
        case '3': {
            trading_post_view_     = TradingPostCategory::INFRASTRUCTURE;
            current_category_      = GetCategoryStr(
                    trading_post_view_);
            current_selected_item_ = 0;
            break;
        }
        case '4': {
            trading_post_view_     = TradingPostCategory::SPARE_PARTS;
            current_category_      = GetCategoryStr(
                    trading_post_view_);
            current_selected_item_ = 0;
            break;
        }
        case '5': {
            trading_post_view_     = TradingPostCategory::SUPPLIES;
            current_category_      = GetCategoryStr(
                    trading_post_view_);
            current_selected_item_ = 0;
            break;
        }
        case '6': {
            trading_post_view_     = TradingPostCategory::TOOLS;
            current_category_      = GetCategoryStr(
                    trading_post_view_);
            current_selected_item_ = 0;
            break;
        }
        case '7': {
            trading_post_view_     = TradingPostCategory::WEAPONS;
            current_category_      = GetCategoryStr(
                    trading_post_view_);
            current_selected_item_ = 0;
        }
            break;
    }
}


void InventoryUI::ShowInventoryInterface () {
    int y = ui_init_y_;
    int x = ui_init_x_;

    mvwaddstr(main_, y++, x,
              "Space station (Use $ to buy supplies, 0 to go to categories)");

    std::stringstream row;

    int i = 1;

    if ( trading_post_view_ == TradingPostCategory::ALL ) {
        mvwaddstr(main_, y++, x, "Categories:");

        for ( auto &c : items_for_sale_ ) {
            row << i << ": " << c.first.c_str();
            mvwaddstr(main_, y++, x, row.str().c_str());
            row.str("");

            ++i;
        }
        return;
    }

    i = 0;

    mvwaddstr(main_, y++, x, current_category_.c_str());
    mvwaddstr(main_, y, x + 4, "Item");
    mvwaddstr(main_, y, x + 28, "Available");
    mvwaddstr(main_, y, x + 39, "Weight");
    mvwaddstr(main_, y, x + 46, "Cost");

    ++y;

    for ( auto &item : *items_for_sale_[current_category_] ) {
        if ( current_selected_item_ == i ) {
            row << "> ";
        } else {
            row << "  ";
        }

        row << i << ":\t" << item->GetName();
        mvwaddstr(main_, y, x - 2, row.str().c_str());
        row.str("");

        row << item->GetQuantity();
        mvwaddstr(main_, y, x + 28, row.str().c_str());
        row.str("");

        row << item->GetWeight();
        mvwaddstr(main_, y, x + 39, row.str().c_str());
        row.str("");

        row << item->GetCost();
        mvwaddstr(main_, y, x + 46, row.str().c_str());
        row.str("");

        ++y;
        ++i;
    }

}


std::string InventoryUI::GetCategoryStr ( TradingPostCategory category ) {
    switch ( category ) {
        case TradingPostCategory::ALL:
            return "";
        case TradingPostCategory::TOOLS:
            return "Tools";
        case TradingPostCategory::SPARE_PARTS:
            return "Spare parts";
        case TradingPostCategory::FUEL:
            return "Fuel";
        case TradingPostCategory::WEAPONS:
            return "Weapons";
        case TradingPostCategory::FOOD:
            return "Food";
        case TradingPostCategory::SUPPLIES:
            return "Supplies";
        case TradingPostCategory::INFRASTRUCTURE:
            return "Infrastructure";
    }
}

}
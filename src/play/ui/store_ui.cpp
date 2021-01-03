//
// Created by Roberts Kalnins on 29/12/2020.
//

#include "store_ui.h"

#include <utility>


namespace sc::play {

StoreUI::StoreUI ( WINDOW *main, const std::string &source,
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

void StoreUI::OnLoop () {

    if ( !spaceship_ ) {
        spaceship_ = spaceship_handler_->GetSpaceship();
        logger_->debug("Got spaceship");
        return;
    }

    ProcessInput();
    ShowInventoryInterface();
}

void StoreUI::ProcessInput () {

    int ch = listener_->GetCh();
    switch ( ch ) {
        case KEY_DOWN: {
            if ( store_view_ == StoreCategory::ALL ) { break; }

            current_selected_item_ = std::min(
                    current_selected_item_ + 1,
                    static_cast<int>(
                            items_for_sale_[current_category_]->size() -
                            1));

            break;
        }
        case KEY_UP: {
            if ( store_view_ == StoreCategory::ALL ) { break; }

            current_selected_item_ = std::max(current_selected_item_ - 1,
                                              0);
            break;
        }
        case 'a': {
            if ( store_view_ == StoreCategory::ALL ) { break; }

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
            if ( store_view_ == StoreCategory::ALL ) { break; }

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
            if ( store_view_ == StoreCategory::ALL ) { break; }
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
            if ( store_view_ == StoreCategory::ALL ) { break; }
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
            store_view_            = StoreCategory::ALL;
            current_selected_item_ = 0;
            break;
        }
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7': {
            auto it = items_for_sale_.begin();
            std::advance(it, ch - '1');

            store_view_            = GetCategory(it->first);
            current_category_      = GetCategoryStr(store_view_);
            current_selected_item_ = 0;
            break;
        }
        default:
            break;
    }
}


void StoreUI::ShowInventoryInterface () {
    int y = ui_init_y_;
    int x = ui_init_x_;

    mvwaddstr(main_, y - 3, x,
              "Space station (Use $ to buy supplies, 0 to go to categories)");

    if ( store_view_ == StoreCategory::ALL ) {
        mvwaddstr(main_, y - 1, x,
                  "Use [1]-[9] to select category");
        y++;
    } else {
        mvwaddstr(main_, y - 1, x,
                  "[0] to view categories, [Up]/[Down] to select item.");
        mvwaddstr(main_, y++, x,
                  "[Right]/[Left] to add/remove 1x. [a]/[b] to add/remove 10x");
    }


    y++;

    std::stringstream row;

    int i = 1;

    if ( store_view_ == StoreCategory::ALL ) {
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


std::string StoreUI::GetCategoryStr ( StoreCategory category ) {
    switch ( category ) {
        case StoreCategory::ALL:
            return "";
        case StoreCategory::TOOLS:
            return "Tools";
        case StoreCategory::SPARE_PARTS:
            return "Spare parts";
        case StoreCategory::FUEL:
            return "Fuel";
        case StoreCategory::WEAPONS:
            return "Weapons";
        case StoreCategory::FOOD:
            return "Food";
        case StoreCategory::SUPPLIES:
            return "Supplies";
        case StoreCategory::INFRASTRUCTURE:
            return "Infrastructure";
    }
}

StoreCategory StoreUI::GetCategory ( const std::string &category ) {
    if ( category.empty()) {
        return StoreCategory::ALL;
    } else if ( category == "Tools" ) {
        return StoreCategory::TOOLS;
    } else if ( category == "Spare parts" ) {
        return StoreCategory::SPARE_PARTS;
    } else if ( category == "Fuel" ) {
        return StoreCategory::FUEL;
    } else if ( category == "Weapons" ) {
        return StoreCategory::WEAPONS;
    } else if ( category == "Food" ) {
        return StoreCategory::FOOD;
    } else if ( category == "Supplies" ) {
        return StoreCategory::SUPPLIES;
    } else if ( category == "Infrastructure" ) {
        return StoreCategory::INFRASTRUCTURE;
    } else {
        return StoreCategory::ALL;
    }
}

}
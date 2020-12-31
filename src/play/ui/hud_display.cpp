//
// Created by Roberts Kalnins on 22/12/2020.
//

#include "hud_display.h"

#include <ncurses.h>
#include <sstream>
#include <utility>


namespace sc::play {

HUDDisplay::HUDDisplay ( const std::string &name,
                         TaskType taskType,
                         shared_spaceship_handler_t spaceship_handler,
                         shared_nav_manager_t nav_manager,
                         shared_input_listener_t listener,
                         WINDOW *main )
        : Task(name, taskType),
          spaceship_handler_(std::move(spaceship_handler)),
          nav_manager_(std::move(nav_manager)),
          listener_(std::move(listener)),
          main_(main), logger_(CreateLogger(name)) {

    Config &config = Config::GetInstance();

    item_init_y_    = config.GetValue("hud.item-y", 0);
    item_init_x_    = config.GetValue("hud.item-x", 0);
    details_init_y_ = config.GetValue("hud.details-y", 0);
    details_init_x_ = config.GetValue("hud.details-x", 0);;
    spaceship_display_y_ = config.GetValue("hud.disp-y", 0);
    spaceship_display_x_ = config.GetValue("hud.disp-x", 0);;

    rows_per_details_pg_ = config.GetValue("hud.rows-per-details-pg", 0);
    cat_per_page_        = config.GetValue("hud.cat-per-pg", 0);

}


void HUDDisplay::Init () {
    logger_->debug("HUD init");
}

GameState HUDDisplay::OnLoop ( GameState state ) {

    if ( !spaceship_ ) {
        spaceship_ = spaceship_handler_->GetSpaceship();
    }

    ProcessInput();

    if ( spaceship_ ) {

        if ( state != GameState::SETUP &&
             spaceship_->IsDeparting()) { return state; }

        PrintHUD();

        if ( show_crew_ ) {
            PrintCrew();
        }

        if ( show_items_ ) {
            PrintItems();
        }

        if ( show_items_ && show_details_ ) {
            PrintCategoryDetails();
        }
    }

    return state;
}

void HUDDisplay::ProcessInput () {
    switch ( listener_->GetCh()) {
        case KEY_MOUSE: {
            MousePosition mpos = listener_->GetMousePos();

            wmouse_trafo(main_, &mpos.y, &mpos.x, false);

            if ( cat_details_.find(mpos.y) !=
                 cat_details_.end() &&
                 item_init_x_ < mpos.x ) {

                mpos_ = mpos;

                show_details_ = !show_details_;
            }

            break;
        }
        case 'i': {
            show_items_ = !show_items_;
            logger_->debug("toggle items to {}", show_items_);
            break;
        }
        case 'c': {
            show_crew_ = !show_crew_;
            logger_->debug("toggle crew to {}", show_crew_);
            break;
        }
        case 'n': {
            if ( show_details_ ) {

                std::string               category = cat_details_[mpos_.y];
                const std::vector< Item > &items   = spaceship_->GetItems()[category];

                details_page_id_ = std::min(details_page_id_ + 1,
                                            static_cast<int> (
                                                    items.size() /
                                                    ( rows_per_details_pg_ )));
            } else {
                cat_details_  = std::map< int, std::string >();
                item_page_id_ = std::min(item_page_id_ + 1,
                                         static_cast<int>(
                                                 spaceship_->GetItems().size() /
                                                 cat_per_page_));
            }
            break;
        }
        case 'b': {
            if ( show_details_ ) {
                details_page_id_ = std::max(details_page_id_ - 1, 0);
            } else {
                cat_details_  = std::map< int, std::string >();
                item_page_id_ = std::max(item_page_id_ - 1, 0);
            }
            break;
        }
        default: {
            break;
        }
    }
}

void HUDDisplay::PrintCrew () {
    int crew_y = spaceship_display_y_ + 3;

    mvwaddstr(main_, crew_y, spaceship_display_x_, "Crew");

    ++crew_y;

    for ( auto &c : spaceship_->GetCrew()) {
        if ( !c.IsDead()) {
            std::stringstream crew_member;
            crew_member << c.GetName() << " " << c.GetHealth() << "/"
                        << c.GetMaxHealth();
            mvwaddstr(main_, crew_y, spaceship_display_x_,
                      crew_member.str().c_str());
            ++crew_y;
        }
    }
}

void HUDDisplay::PrintItems () {
    int item_y = item_init_y_;
    int item_x = item_init_x_;


    std::stringstream items_disp;


    int start   = item_page_id_ * cat_per_page_;
    int end     = start + cat_per_page_;
    int counter = 0;
    int length  = 50;

    items_disp << "Items (" << ( 1 + item_page_id_ ) << "/"
               << ( spaceship_->GetItems().size() / cat_per_page_ + 1 )
               << ")";

    mvwaddstr(main_, item_y, item_x, items_disp.str().c_str());
    ++item_y;

    items_disp.str("");


    for ( auto &c : spaceship_->GetItems()) {
        if ( !c.second.empty()) {
            if ( start <= counter && counter < end ) {
                mvwaddstr(main_, item_y, item_x, c.first.c_str());

                items_disp.str("");

                for ( auto &i : c.second ) {
                    items_disp << i.GetName() << "(" << i.GetQuantity()
                               << ") ";
                }

                mvwaddnstr(main_, item_y, item_x + 16,
                           items_disp.str().c_str(), length);

                if ( items_disp.str().length() > length ) {
                    mvwaddstr(main_, item_y, item_x + 16 + length, " ...");
                }

                cat_details_[item_y] = c.first;

                ++item_y;
            }
            ++counter;
        }
    }
}


void HUDDisplay::PrintHUD () {
    std::stringstream display;
    display.precision(3);
    display << "Crew: " << spaceship_->GetCrew().size()
            << "\tFuel: "
            << std::max(
                    ( spaceship_->GetFuel() / spaceship_->GetFullFuel() *
                      100.0 ), 0.0)
            << "%\tHull: " << spaceship_->GetHull();

    mvwaddstr(main_, spaceship_display_y_, spaceship_display_x_,
              display.str().c_str());

    display.str("");

    display.precision(4);
    display << "Storage: "
            << ( spaceship_->GetMaxWeight() - spaceship_->GetWeight())
            << "\tMoney: " << spaceship_->GetMoney() << "\tFood: "
            << spaceship_->GetFood();

    mvwaddstr(main_, ( spaceship_display_y_ + 1 ), spaceship_display_x_,
              display.str().c_str());
}

void HUDDisplay::PrintCategoryDetails () {
    if ( cat_details_[mpos_.y].empty()) {
        logger_->debug("No category at {}", mpos_.y);
        return;
    }

    int y = details_init_y_;

    std::string               category = cat_details_[mpos_.y];
    const std::vector< Item > &items   = spaceship_->GetItems()[category];


    int index = details_page_id_ * ( rows_per_details_pg_ );
    int end   = index + ( rows_per_details_pg_ );

    std::stringstream row;

    row << "All items in selected category (" << ( details_page_id_ + 1 )
        << "/" <<
        ( items.size() /
          ( rows_per_details_pg_ ) + 1 ) << ")";

    mvwaddstr(main_, y, details_init_x_, row.str().c_str());
    y += 2;

    row.str("");
    for ( int r = index; r < end; ++r ) {

        if ( r >= items.size()) { return; }

        row << items[r].GetQuantity() << "x\t" << items[r].GetName();
        mvwaddstr(main_, y, details_init_x_, row.str().c_str());
        ++y;
        row.str("");
    }
}


}
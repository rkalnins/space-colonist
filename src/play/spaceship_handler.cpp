//
// Created by Roberts Kalnins on 17/12/2020.
//

#include <sstream>
#include <ncurses.h>
#include <cmath>
#include <ncurses.h>

#include "spaceship_handler.h"
#include "objects/crew_member.h"


namespace sc::play {

SpaceshipHandler::SpaceshipHandler ( const std::string &name,
                                     TaskType taskType,
                                     std::shared_ptr< InputListener > listener,
                                     WINDOW *main,
                                     std::shared_ptr< Spaceship > spaceship )
        : Task(name, taskType),
          spaceship_(std::move(spaceship)),
          listener_(std::move(listener)),
          main_(main) {

    logger_ = spdlog::basic_logger_mt("ss_handler",
                                      "logs/space-colonist-log.log");
    logger_->set_level(spdlog::level::debug);

    spaceship_->ResetFuel();

}

void SpaceshipHandler::Init () {
    logger_->debug("Spaceship init");
}

GameState SpaceshipHandler::OnLoop () {
    spaceship_->UseFuel(fuel_use_);

    ProcessInput();

    PrintHUD();

    if ( show_crew_ ) {
        PrintCrew();
    }

    if ( show_items_ ) {
        PrintItems();
    }

    if ( show_overflow_ ) {
        PrintItemOverflow(mouse_event_.y);
    }

    return GameState::RUNNING;
}

bool SpaceshipHandler::IsFinished () {
    return false;
}

void SpaceshipHandler::PrintCrew () {
    int crew_y = spaceship_display_y_ + 3;

    mvwaddstr(main_, crew_y, spaceship_display_x_, "Crew");

    ++crew_y;

    for ( auto &c : spaceship_->GetCrew()) {
        std::stringstream crew_member;
        crew_member << c.GetName() << " " << c.GetHealth() << "/"
                    << c.GetMaxHealth();
        mvwaddstr(main_, crew_y, spaceship_display_x_,
                  crew_member.str().c_str());
        ++crew_y;
    }
}

void SpaceshipHandler::PrintItems () {
    int item_y = item_init_y_;
    int item_x = item_init_x_;


    std::stringstream items_disp;


    int start   = item_page_id_ * cat_per_page_;
    int end     = start + cat_per_page_;
    int counter = 0;

    items_disp << "Items (" << ( 1 + item_page_id_ ) << "/"
               << ( spaceship_->GetItems().size() / cat_per_page_ + 1 )
               << ")";

    mvwaddstr(main_, item_y, item_x, items_disp.str().c_str());
    ++item_y;

    items_disp.str("");

    int length = 50;

    for ( auto &c : spaceship_->GetItems()) {
        if ( start <= counter && counter < end ) {
            mvwaddstr(main_, item_y, item_x, c.first.c_str());

            items_disp.str("");

            for ( auto &i : c.second ) {
                items_disp << i.GetName() << " ";
            }

            mvwaddnstr(main_, item_y, item_x + 10,
                       items_disp.str().c_str(), length);

            if ( items_disp.str().length() > length ) {
                mvwaddstr(main_, item_y, item_x + 10 + length, " ...");
                item_overflow_[item_y] = std::make_unique< std::vector< Item >>(
                        c.second);
            }

            ++item_y;
        }
        ++counter;
    }
}

void SpaceshipHandler::PrintHUD () {
    std::stringstream display;
    display << "Crew: " << spaceship_->GetCrew().size()
            << "\tFuel: "
            << std::round(
                    spaceship_->GetFuel() / spaceship_->GetFullFuel() *
                    100.0)
            << "\tHull: " << spaceship_->GetHull();

    mvwaddstr(main_, spaceship_display_y_, spaceship_display_x_,
              display.str().c_str());

    display.str("");

    display << "Storage: "
            << ( spaceship_->GetMaxWeight() - spaceship_->GetWeight())
            << "\tMoney: " << spaceship_->GetMoney();

    mvwaddstr(main_, ( spaceship_display_y_ + 1 ), spaceship_display_x_,
              display.str().c_str());
}

void SpaceshipHandler::ProcessInput () {
    switch ( listener_->GetCh()) {
        case KEY_MOUSE: {
            getmouse(&mouse_event_);
            wmouse_trafo(main_, &mouse_event_.y, &mouse_event_.x, false);

            logger_->debug("mouse: {} {}", mouse_event_.y, mouse_event_.x);

            if ( item_overflow_.find(mouse_event_.y) !=
                 item_overflow_.end() &&
                 item_init_x_ < mouse_event_.x ) {

                show_overflow_ = !show_overflow_;
            }

            break;
        }
        case 'r': {
            spaceship_->ResetFuel();
            logger_->debug("Reset fuel");
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
        case 'j': {
            CrewMember c("Joe", 10, 10, std::map< std::string, int >());
            spaceship_->AddCrewMember(c);
            break;
        }
        case 'h': {
            show_overflow_ = false;
            Item i("tools", "hammer", 10, 3);
            spaceship_->AddItem(i);
            break;
        }
        case 'f': {
            show_overflow_ = false;
            Item i("food", "potato", 1, 1);
            spaceship_->AddItem(i);
            break;
        }
        case 'a': {
            show_overflow_ = false;
            Item i("food", "apple", 1, 1);
            spaceship_->AddItem(i);
            break;
        }
        case 't': {
            show_overflow_ = false;
            Item i("transport", "bike", 1, 10);
            spaceship_->AddItem(i);
            break;
        }
        case 'n': {
            if ( show_overflow_ ) {
                overflow_page_id_ = std::min(overflow_page_id_ + 1,
                                             static_cast<int> (
                                                     item_overflow_[mouse_event_.y]->size() /
                                                     ( rows_per_overflow_pg_ *
                                                       items_per_overflow_row_ )));
            } else {
                item_overflow_ = std::map< int, std::unique_ptr< std::vector< Item >> >();
                item_page_id_  = std::min(item_page_id_ + 1,
                                          static_cast<int>(
                                                  spaceship_->GetItems().size() /
                                                  cat_per_page_));
            }
            break;
        }
        case 'b': {
            if ( show_overflow_ ) {
                overflow_page_id_ = std::max(overflow_page_id_ - 1, 0);
            } else {
                item_overflow_ = std::map< int, std::unique_ptr< std::vector< Item >> >();
                item_page_id_  = std::max(item_page_id_ - 1, 0);
            }
            break;
        }
        default: {
            break;
        }
    }

    listener_->ResetCh();
}

void SpaceshipHandler::PrintItemOverflow ( int id ) {

    if ( !item_overflow_[id] ) {
        logger_->debug("No vector at {}", id);
        return;
    }

    logger_->debug("Printing overflow for row {}", id);
    int y = overflow_init_y_;

    const std::vector< Item > &items = *item_overflow_[id];


    bool done  = false;
    int  index = overflow_page_id_ * ( rows_per_overflow_pg_ *
                                       items_per_overflow_row_ );
    int  end   = index + ( rows_per_overflow_pg_ *
                           items_per_overflow_row_ );

    std::stringstream row;

    row << "All items in selected category (" << ( overflow_page_id_ + 1 )
        << "/" <<
        ( item_overflow_[mouse_event_.y]->size() /
          ( rows_per_overflow_pg_ *
            items_per_overflow_row_ ) + 1 ) << ")";

    mvwaddstr(main_, y, overflow_init_x_, row.str().c_str());
    y += 2;

    row.str("");
    for ( int r = 0; r < rows_per_overflow_pg_; ++r ) {
        for ( int c = 0; c < items_per_overflow_row_; ++c ) {
            if ( index < items.size() && index < end ) {
                row << items[index].GetName() << " ";
            } else {
                done = true;
                break;
            }
            ++index;
        }

        if ( done ) {
            return;
        }

        mvwaddstr(main_, y, overflow_init_x_, row.str().c_str());
        ++y;
        row.str("");
    }
}

}
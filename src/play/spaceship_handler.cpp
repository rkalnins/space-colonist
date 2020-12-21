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
                                     WINDOW *main )
        : Task(name, taskType),
          listener_(std::move(listener)),
          main_(main) {

    logger_ = spdlog::basic_logger_mt("ss_handler",
                                      "logs/space-colonist-log.log");
    logger_->set_level(spdlog::level::debug);

}

void SpaceshipHandler::Init () {
    logger_->debug("Spaceship init");
}

GameState SpaceshipHandler::OnLoop ( GameState state ) {
    ProcessInput();

    if ( spaceship_ ) {

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

bool SpaceshipHandler::IsFinished () {
    return false;
}

void SpaceshipHandler::PrintCrew () {
    int crew_y = spaceship_display_y_ + 3;

    mvwaddstr(main_, crew_y, spaceship_display_x_, "Crew");

    ++crew_y;

    for ( auto &c : spaceship_->GetCrew()) {
        if (!c.IsDead()) {
            std::stringstream crew_member;
            crew_member << c.GetName() << " " << c.GetHealth() << "/"
                        << c.GetMaxHealth();
            mvwaddstr(main_, crew_y, spaceship_display_x_,
                      crew_member.str().c_str());
            ++crew_y;
        }
    }
}

void SpaceshipHandler::PrintItems () {
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
        if (!c.second.empty()) {
            if ( start <= counter && counter < end ) {
                mvwaddstr(main_, item_y, item_x, c.first.c_str());

                items_disp.str("");

                for ( auto &i : c.second ) {
                    items_disp << i.GetName() << "(" << i.GetValue()
                               << ") ";
                }

                mvwaddnstr(main_, item_y, item_x + 13,
                           items_disp.str().c_str(), length);

                if ( items_disp.str().length() > length ) {
                    mvwaddstr(main_, item_y, item_x + 13 + length, " ...");
                }

                cat_details_[item_y] = c.first;

                ++item_y;
            }
            ++counter;
        }
    }
}

void SpaceshipHandler::PrintHUD () {
    std::stringstream display;
    display.precision(2);
    display << "Crew: " << spaceship_->GetCrew().size()
            << "\tFuel: "
            << ( spaceship_->GetFuel() / spaceship_->GetFullFuel() *
                 100.0 )
            << "%\tHull: " << spaceship_->GetHull();

    mvwaddstr(main_, spaceship_display_y_, spaceship_display_x_,
              display.str().c_str());

    display.str("");

    display.precision(3);
    display << "Storage: "
            << ( spaceship_->GetMaxWeight() - spaceship_->GetWeight())
            << "\tMoney: " << spaceship_->GetMoney() << "\tFood: "
            << spaceship_->GetFood();

    mvwaddstr(main_, ( spaceship_display_y_ + 1 ), spaceship_display_x_,
              display.str().c_str());
}

void SpaceshipHandler::ProcessInput () {
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

void SpaceshipHandler::PrintCategoryDetails () {


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

        row << items[r].GetValue() << "x\t" << items[r].GetName();
        mvwaddstr(main_, y, details_init_x_, row.str().c_str());
        ++y;
        row.str("");
    }
}

const std::shared_ptr< play::Spaceship > &
SpaceshipHandler::GetSpaceship () const {
    return spaceship_;
}

void SpaceshipHandler::SetSpaceship (
        const std::shared_ptr< play::Spaceship > &spaceship ) {
    spaceship_ = spaceship;
    spaceship_->UpdateMoney(-1 * spaceship->GetCost());
}

void
SpaceshipHandler::SetCrew ( const std::vector< CrewMember > &crew_choices,
                            std::vector< Checkbox > &selected ) {
    for ( int i = 0; i < selected.size(); ++i ) {
        if ( selected[i].checked ) {
            spaceship_->AddCrewMember(crew_choices[i]);
        }
    }
}

double SpaceshipHandler::GetDistanceRemaining () const {
    return distance_remaining_;
}

void SpaceshipHandler::SetDistanceRemaining ( double distance_remaining ) {
    distance_remaining_ = distance_remaining;
}

double SpaceshipHandler::GetInitialDistance () const {
    return initial_distance_;
}

void SpaceshipHandler::SetInitialDistance ( double initial_distance ) {
    initial_distance_   = initial_distance;
    distance_remaining_ = initial_distance;
}

void SpaceshipHandler::UpdateDistanceRemaining ( double distance ) {
    distance_remaining_ += distance;
}

}
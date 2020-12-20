//
// Created by Roberts Kalnins on 18/12/2020.
//

#include "setup_ui.h"

#include <utility>
#include <sstream>

#include "objects/crew_factory.h"
#include "spaceship_factory.h"


namespace sc::play {


SetupUI::SetupUI ( const std::string &name, TaskType taskType,
                   std::shared_ptr< SpaceshipHandler > spaceship_handler,
                   std::shared_ptr< InputListener > listener,
                   WINDOW *main )
        : Task(name, taskType),
          spaceship_handler_(std::move(spaceship_handler)),
          listener_(std::move(listener)),
          main_(main) {
    logger_ = spdlog::basic_logger_mt("ui",
                                      "logs/space-colonist-log.log");
    logger_->set_level(spdlog::level::debug);

    map_generator_ = std::make_unique< MapGenerator >(main, ui_init_y_,
                                                      ui_init_x_);
}

void SetupUI::Init () {
    crew_choices_.reserve(crew_choices_count_);
    for ( int i = 0; i < crew_choices_count_; ++i ) {
        crew_choices_.push_back(crew_member_factory_.Create());
    }

    selected_crew_.resize(crew_choices_count_);

    int y       = ui_init_y_ + 15;
    int left_x  = ui_init_x_;
    int right_x = left_x + 3;

    for ( auto &i : selected_crew_ ) {
        i = { y, left_x, right_x, false };
        left_x += 20;
        right_x += 20;
    }

    logger_->debug("Created crew choices");

    map_generator_->SeedMap();
    logger_->debug("Created map");

    spaceship_choices_.reserve(2);

    for ( int i = 0; i < spaceship_choice_count_; ++i ) {
        spaceship_choices_.push_back(spaceship_factory_.CreateSpaceship());
    }

    logger_->debug("Created spaceship choices");
}


void SetupUI::ProcessMouseCheckboxInput ( MousePosition &mpos ) {
    int checked_count = 0;
    int index         = 0;

    for ( auto &box : selected_crew_ ) {
        if ( box.y == mpos.y &&
             box.left_x <= mpos.x &&
             mpos.x <= box.right_x ) {

            box.checked = !box.checked;

            if ( box.checked ) { select_order_.push(index); }

            logger_->debug("box {} toggled {} ", index,
                           box.checked);
        }

        if ( box.checked ) { ++checked_count; }

        while ( checked_count >
                spaceship_handler_->GetSpaceship()->GetMaxCrew()) {
            size_t top = select_order_.front();
            select_order_.pop();

            if ( selected_crew_[top].checked ) {
                selected_crew_[top].checked = false;
                --checked_count;
            }

        }

        ++index;
    }
}

void SetupUI::ProcessMousePlanetSelect ( MousePosition &mpos ) {
    int map_y = mpos.y - ui_init_y_;
    int map_x = mpos.x - ui_init_x_;

    map_generator_->ToggleSelection(map_y, map_x);
}

GameState SetupUI::OnLoop () {

    switch ( state_ ) {
        case SetupState::SPACESHIP_SETUP:
            SpaceshipSelection();
            break;
        case SetupState::DESTINATION_SELECTION:
            DestinationSelection();
            break;
        case SetupState::CREW_SELECTION:
            CrewSelection();
            break;
        case SetupState::INVENTORY_SELECTION:
            break;
    }

    switch ( listener_->GetCh()) {
        case KEY_MOUSE: {
            MousePosition mpos = listener_->GetMousePos();

            wmouse_trafo(main_, &mpos.y, &mpos.x, false);

            switch ( state_ ) {
                case SetupState::CREW_SELECTION:
                    ProcessMouseCheckboxInput(mpos);
                    break;
                case SetupState::DESTINATION_SELECTION:
                    ProcessMousePlanetSelect(mpos);
                default:
                    break;
            }
            break;
        }
        case KEY_DOWN:
            switch ( state_ ) {
                case SetupState::SPACESHIP_SETUP:
                    selected_spaceship_ = std::min(selected_spaceship_ + 1,
                                                   spaceship_choice_count_ -
                                                   1);
                    break;
                default:
                    break;
            }
            break;
        case KEY_UP:
            switch ( state_ ) {
                case SetupState::SPACESHIP_SETUP:
                    selected_spaceship_ = std::max(selected_spaceship_ - 1,
                                                   0);
                    break;
                default:
                    break;
            }
            break;
        case 10: {
            switch ( state_ ) {
                case SetupState::SPACESHIP_SETUP:
                    state_ = SetupState::DESTINATION_SELECTION;
                    spaceship_handler_->SetSpaceship(
                            spaceship_choices_[selected_spaceship_]);
                    break;
                case SetupState::DESTINATION_SELECTION:
                    state_ = SetupState::CREW_SELECTION;
                    break;
                case SetupState::CREW_SELECTION:
                    state_ = SetupState::INVENTORY_SELECTION;
                    spaceship_handler_->SetCrew(crew_choices_,
                                                selected_crew_);
                    break;
                case SetupState::INVENTORY_SELECTION:
                    logger_->debug("Finished inventory selection");
                    break;
                default:
                    break;
            }
        }
        default:
            break;
    }


    return GameState::RUNNING;
}

void SetupUI::SpaceshipSelection () {
    int y = ui_init_y_;
    int x = ui_init_x_;

    std::stringstream disp;

    int i = 1;

    disp << "Money available: " << ( SpaceshipFactory::GetInitialMoney() -
                                     spaceship_choices_[selected_spaceship_]->GetCost());
    mvwaddstr(main_, y - 2, x, disp.str().c_str());
    disp.str("");

    disp << "\tCost\tMax Fuel\tMax Cargo\tMax Crew";
    mvwaddstr(main_, y, x, disp.str().c_str());
    disp.str("");
    mvwaddstr(main_, y + 1, x,
              "-------------------------------------------------------------------------------");
    y += 4;

    for ( const auto &s : spaceship_choices_ ) {
        if ( selected_spaceship_ == i - 1 ) {
            disp << "[" << i << "]";
        } else {
            disp << " " << i;
        }
        disp << "\t" << s->GetCost() << "\t"
             << s->GetFullFuel() << "\t\t"
             << s->GetMaxWeight() << "\t\t" << s->GetMaxCrew();
        mvwaddstr(main_, y, x, disp.str().c_str());
        disp.str("");

        spaceship_factory_.PrintSpaceship(main_, y - 1, x + 55,
                                          s->GetAppearanceCode());

        mvwaddstr(main_, y + 3, x,
                  "-------------------------------------------------------------------------------");
        y += 6;
        ++i;
    }
}

void SetupUI::DestinationSelection () {
    map_generator_->PrintMap();
}


void SetupUI::CrewSelection () {
    int y = ui_init_y_;
    int x = ui_init_x_;

    int name_y        = ui_init_y_ + 8;
    int health_y      = name_y + 1;
    int skills_init_y = health_y + 1;
    int checkbox_y    = skills_init_y + 5;

    mvwaddstr(main_, y, x, "Choose your crew:");
    y += 2;

    std::stringstream stream;

    int        i = 0;
    for ( auto &crew_choice : crew_choices_ ) {
        CrewMemberFactory::PrintCharacter(main_, y, x,
                                          crew_choice.GetCode());


        mvwaddstr(main_, name_y, x,
                  crew_choice.GetName().c_str());

        stream << "HP: " << crew_choice.GetMaxHealth();

        mvwaddstr(main_, health_y, x, stream.str().c_str());
        stream.str("");

        int              skill_y = skills_init_y;
        for ( const auto &s : crew_choice.GetSkills()) {
            stream << s.first << " (" << s.second << ")";
            mvwaddstr(main_, skill_y++, x, stream.str().c_str());
            stream.str("");
        }

        if ( selected_crew_[i].checked ) {
            mvwaddstr(main_, checkbox_y, x, "[*]");
        } else {
            mvwaddstr(main_, checkbox_y, x, "[ ]");
        }

        ++i;
        x += 20;
    }
}

void SetupUI::InventorySelection () {

}

}

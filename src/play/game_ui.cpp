//
// Created by Roberts Kalnins on 18/12/2020.
//

#include "game_ui.h"

#include <utility>
#include <sstream>

#include "objects/crew_factory.h"


namespace sc::play {


GameUI::GameUI ( const std::string &name, TaskType taskType,
                 std::shared_ptr< Spaceship > spaceship,
                 std::shared_ptr< InputListener > listener,
                 WINDOW *main )
        : Task(name, taskType),
          spaceship_(std::move(spaceship)),
          listener_(std::move(listener)),
          main_(main) {
    logger_ = spdlog::basic_logger_mt("ui",
                                      "logs/space-colonist-log.log");
    logger_->set_level(spdlog::level::debug);

    map_generator_ = std::make_unique< MapGenerator >(main, ui_init_y_,
                                                      ui_init_x_);
}

void GameUI::Init () {
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
}


void GameUI::ProcessMouseCheckboxInput ( MousePosition &mpos ) {
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

        while ( checked_count > crew_select_max_ ) {
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

void GameUI::ProcessMousePlanetSelect ( MousePosition &mpos ) {
    int map_y = mpos.y - ui_init_y_;
    int map_x = mpos.x - ui_init_x_;

    map_generator_->ToggleSelection(map_y, map_x);
}

GameState GameUI::OnLoop () {

    switch ( listener_->GetCh()) {
        case KEY_MOUSE: {
            MousePosition mpos = listener_->GetMousePos();

            wmouse_trafo(main_, &mpos.y, &mpos.x, false);

            switch ( state_ ) {
                case UIState::CREW_SELECTION:
                    ProcessMouseCheckboxInput(mpos);
                    break;
                case UIState::DESTINATION_SELECTION:
                    ProcessMousePlanetSelect(mpos);
                default:
                    break;
            }
            break;
        }
        case 10: {
            switch ( state_ ) {
                case UIState::SPACESHIP_SETUP:
                    state_ = UIState::DESTINATION_SELECTION;
                    map_generator_->AddRouteLeg();
                    break;
                case UIState::DESTINATION_SELECTION:
                    state_ = UIState::CREW_SELECTION;
                    break;
                case UIState::CREW_SELECTION:
                    state_ = UIState::INVENTORY_SELECTION;
                    break;
                default:
                    break;
            }
        }
        default:
            break;
    }

    switch ( state_ ) {
        case UIState::SPACESHIP_SETUP:
            break;
        case UIState::DESTINATION_SELECTION:
            DestinationSelection();
            break;
        case UIState::CREW_SELECTION:
            CrewSelection();
            break;
        case UIState::INVENTORY_SELECTION:
            break;
        case UIState::TRADING:
            break;
        case UIState::FIGHTING:
            break;
        case UIState::MECHANICAL_FAILURE:
            break;
        case UIState::MAP:
            break;
    }

    return GameState::RUNNING;
}

void GameUI::CrewSelection () {
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

void GameUI::DestinationSelection () {
    map_generator_->PrintMap();
}

}







/*
        case 'j': {
            CrewMember c("Joe", 10, 10, std::map< std::string, int >());
            spaceship_->AddCrewMember(c);
            break;
        }
        case 'a': {
            show_overflow_ = false;
            Item i("tools", "hammer", 1, 3);
            spaceship_->AddItem(i);
            break;
        }
 */
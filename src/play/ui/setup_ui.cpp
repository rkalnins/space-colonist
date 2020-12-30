//
// Created by Roberts Kalnins on 18/12/2020.
//

#include "setup_ui.h"

#include <utility>
#include <sstream>


namespace sc::play {


SetupUI::SetupUI ( const std::string &name, TaskType taskType,
                   const shared_spaceship_handler_t &spaceship_handler,
                   shared_nav_manager_t nav_manager_,
                   const shared_input_listener_t &listener,
                   shared_spaceship_factory_t spaceship_factory,
                   WINDOW *main )
        : Task(name, taskType),
          spaceship_handler_(spaceship_handler),
          nav_manager_(std::move(nav_manager_)),
          listener_(listener),
          spaceship_factory_(std::move(spaceship_factory)),
          main_(main),
          logger_(CreateLogger("setup_ui")),
          inventory_ui_(main, "setup", nullptr, listener,
                        spaceship_handler) {
    logger_ = CreateLogger(name);


    Config &config = Config::GetInstance();

    ui_init_y_ = config.GetValue("setup-ui.init-y", 0);
    ui_init_x_ = config.GetValue("setup-ui.init-x", 0);

    crew_choices_count_     = config.GetValue("setup-ui.crew-choices", 0);
    spaceship_choice_count_ = config.GetValue("setup-ui.spaceship-choices",
                                              0);

    map_generator_ = std::make_unique< SpaceMap >(main, ui_init_y_,
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
        spaceship_choices_.push_back(
                spaceship_factory_->CreateSpaceship());
    }

    logger_->debug("Created spaceship choices");
}


void SetupUI::ProcessMouseCheckboxInput ( MousePosition &mpos ) {
    selected_crew_count_ = 0;
    int index = 0;

    for ( auto &box : selected_crew_ ) {
        if ( box.y == mpos.y &&
             box.left_x <= mpos.x &&
             mpos.x <= box.right_x ) {

            box.checked = !box.checked;

            if ( box.checked ) { select_order_.push(index); }

            logger_->debug("box {} toggled {} ", index,
                           box.checked);
        }

        if ( box.checked ) { ++selected_crew_count_; }

        while ( selected_crew_count_ >
                spaceship_handler_->GetSpaceship()->GetMaxCrew()) {
            size_t top = select_order_.front();
            select_order_.pop();

            if ( selected_crew_[top].checked ) {
                selected_crew_[top].checked = false;
                --selected_crew_count_;
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

GameState SetupUI::OnLoop ( GameState state ) {
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
            inventory_ui_.OnLoop();
            break;
        case SetupState::DONE:
            return GameState::RUNNING;
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
        case KEY_DOWN: {
            switch ( state_ ) {
                case SetupState::SPACESHIP_SETUP: {
                    selected_spaceship_ = std::min(selected_spaceship_ + 1,
                                                   spaceship_choice_count_ -
                                                   1);
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case KEY_UP: {
            switch ( state_ ) {
                case SetupState::SPACESHIP_SETUP:
                    selected_spaceship_ = std::max(selected_spaceship_ - 1,
                                                   0);
                    break;
                default:
                    break;
            }
            break;
        }
        case 10: {
            switch ( state_ ) {
                case SetupState::SPACESHIP_SETUP:
                    state_ = SetupState::DESTINATION_SELECTION;
                    spaceship_handler_->SetSpaceship(
                            spaceship_choices_[selected_spaceship_]);
                    spaceship_handler_->GetSpaceship()->LoggingInit();
                    break;
                case SetupState::DESTINATION_SELECTION:
                    if ( map_generator_->GetCost() < 1.0 ) { break; }

                    state_ = SetupState::CREW_SELECTION;
                    nav_manager_->SetInitialDistance(
                            map_generator_->GetCost());
                    break;
                case SetupState::CREW_SELECTION:
                    if ( selected_crew_count_ == 0 ) { break; }

                    state_ = SetupState::INVENTORY_SELECTION;
                    spaceship_handler_->SetCrew(crew_choices_,
                                                selected_crew_);
                    logger_->debug("Starting inventory selection");
                    break;
                case SetupState::INVENTORY_SELECTION:
                    state_ = SetupState::DONE;
                    logger_->debug("Finished inventory selection");
                    return GameState::RUNNING;
                default:
                    break;
            }
        }
        default:
            break;
    }


    return GameState::SETUP;
}

void SetupUI::SpaceshipSelection () {
    int y = ui_init_y_;
    int x = ui_init_x_;


    std::stringstream disp;

    int i = 1;

    disp << "Money available: "
         << ( spaceship_factory_->GetInitialMoney() -
              spaceship_choices_[selected_spaceship_]->GetCost());
    mvwaddstr(main_, y - 2, x, disp.str().c_str());
    disp.str("");

    disp << "\tCost\tMax Fuel\tMax Cargo\tMax Crew\tHull";
    mvwaddstr(main_, y, x, disp.str().c_str());
    disp.str("");
    mvwaddstr(main_, y + 1, x,
              "-----------------------------------------------------------------------------------------");
    y += 4;

    for ( const auto &s : spaceship_choices_ ) {
        if ( selected_spaceship_ == i - 1 ) {
            disp << "[" << i << "]";
        } else {
            disp << " " << i;
        }
        disp << "\t" << s->GetCost() << "\t" << s->GetFullFuel() << "\t\t"
             << s->GetMaxWeight() << "\t\t" << s->GetMaxCrew() << "\t\t"
             << s->GetFullHull();

        mvwaddstr(main_, y, x, disp.str().c_str());
        disp.str("");

        // print spaceship here

        mvwaddstr(main_, y + 3, x,
                  "-----------------------------------------------------------------------------------------");
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
            mvwaddstr(main_, selected_crew_[i].y, x, "[*]");
        } else {
            mvwaddstr(main_, selected_crew_[i].y, x, "[ ]");
        }

        ++i;
        x += 20;
    }
}

bool SetupUI::IsFinished () {
    return state_ == SetupState::DONE;
}

}

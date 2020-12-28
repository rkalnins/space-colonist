//
// Created by Roberts Kalnins on 18/12/2020.
//

#include "setup_ui.h"

#include <utility>
#include <sstream>


namespace sc::play {


SetupUI::SetupUI ( const std::string &name, TaskType taskType,
                   shared_spaceship_handler_t spaceship_handler,
                   shared_nav_manager_t nav_manager_,
                   shared_input_listener_t listener,
                   shared_spaceship_factory_t spaceship_factory,
                   WINDOW *main )
        : Task(name, taskType),
          spaceship_handler_(std::move(spaceship_handler)),
          nav_manager_(std::move(nav_manager_)),
          listener_(std::move(listener)),
          spaceship_factory_(std::move(spaceship_factory)),
          main_(main),
          logger_(CreateLogger("setup_ui")) {
    logger_ = CreateLogger(name);


    logger_->debug("Getting categories");
    cat_vec_t categories = ItemSource::GetInstance().GetCategories();
    logger_->debug("Got categories");

    for ( auto &c : *categories ) {
        items_for_sale_.insert(
                std::make_pair(c, ItemSource::GetInstance().GetItems(c)));
    }

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
            InventorySelection();
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
        case KEY_DOWN:
            switch ( state_ ) {
                case SetupState::SPACESHIP_SETUP:
                    selected_spaceship_ = std::min(selected_spaceship_ + 1,
                                                   spaceship_choice_count_ -
                                                   1);
                    break;
                case SetupState::INVENTORY_SELECTION: {
                    current_selected_item_ = std::min(
                            current_selected_item_ + 1,
                            static_cast<int>(
                                    items_for_sale_[current_category_]->size() -
                                    1));
                    break;
                }
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
                case SetupState::INVENTORY_SELECTION: {
                    current_selected_item_ = std::max(
                            current_selected_item_ - 1, 0);
                    break;
                }
                default:
                    break;
            }
            break;
        case 'a': {
            switch ( state_ ) {
                case SetupState::INVENTORY_SELECTION: {
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
                default:
                    break;
            }
            break;
        }
        case KEY_RIGHT: {
            switch ( state_ ) {
                case SetupState::INVENTORY_SELECTION: {
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
                default:
                    break;
            }
            break;
        }
        case 'b': {
            switch ( state_ ) {
                case SetupState::INVENTORY_SELECTION: {
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
                default:
                    break;
            }
            break;
        }
        case KEY_LEFT: {
            switch ( state_ ) {
                case SetupState::INVENTORY_SELECTION: {
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
                    state_ = SetupState::CREW_SELECTION;
                    nav_manager_->SetInitialDistance(
                            map_generator_->GetCost());
                    break;
                case SetupState::CREW_SELECTION:
                    state_ = SetupState::INVENTORY_SELECTION;
                    spaceship_handler_->SetCrew(crew_choices_,
                                                selected_crew_);
                    break;
                case SetupState::INVENTORY_SELECTION:
                    state_ = SetupState::DONE;
                    logger_->debug("Finished inventory selection");
                    return GameState::RUNNING;
                default:
                    break;
            }
        }
        case '0': {
            switch ( state_ ) {
                case SetupState::INVENTORY_SELECTION:
                    trading_post_view_     = TradingPostCategory::ALL;
                    current_selected_item_ = 0;
                    break;
                default:
                    break;
            }
            break;
        }
        case '1': {
            switch ( state_ ) {
                case SetupState::INVENTORY_SELECTION:
                    trading_post_view_     = TradingPostCategory::FOOD;
                    current_category_      = GetCategoryStr(
                            trading_post_view_);
                    current_selected_item_ = 0;
                    break;
                default:
                    break;
            }
            break;
        }
        case '2': {
            switch ( state_ ) {
                case SetupState::INVENTORY_SELECTION:
                    trading_post_view_     = TradingPostCategory::FUEL;
                    current_category_      = GetCategoryStr(
                            trading_post_view_);
                    current_selected_item_ = 0;
                    break;
                default:
                    break;
            }
            break;
        }
        case '3': {
            switch ( state_ ) {
                case SetupState::INVENTORY_SELECTION:
                    trading_post_view_     = TradingPostCategory::INFRASTRUCTURE;
                    current_category_      = GetCategoryStr(
                            trading_post_view_);
                    current_selected_item_ = 0;
                    break;
                default:
                    break;
            }
            break;
        }
        case '4': {
            switch ( state_ ) {
                case SetupState::INVENTORY_SELECTION:
                    trading_post_view_     = TradingPostCategory::SPARE_PARTS;
                    current_category_      = GetCategoryStr(
                            trading_post_view_);
                    current_selected_item_ = 0;
                    break;
                default:
                    break;
            }
            break;
        }
        case '5': {
            switch ( state_ ) {
                case SetupState::INVENTORY_SELECTION:
                    trading_post_view_     = TradingPostCategory::SUPPLIES;
                    current_category_      = GetCategoryStr(
                            trading_post_view_);
                    current_selected_item_ = 0;
                    break;
                default:
                    break;
            }
            break;
        }
        case '6': {
            switch ( state_ ) {
                case SetupState::INVENTORY_SELECTION:
                    trading_post_view_     = TradingPostCategory::TOOLS;
                    current_category_      = GetCategoryStr(
                            trading_post_view_);
                    current_selected_item_ = 0;
                    break;
                default:
                    break;
            }
            break;
        }
        case '7': {
            switch ( state_ ) {
                case SetupState::INVENTORY_SELECTION:
                    trading_post_view_     = TradingPostCategory::WEAPONS;
                    current_category_      = GetCategoryStr(
                            trading_post_view_);
                    current_selected_item_ = 0;
                    break;
                default:
                    break;
            }
            break;
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

        spaceship_factory_->PrintSpaceship(main_, y - 1, x + 70,
                                           s->GetAppearanceCode());

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

void SetupUI::InventorySelection () {
    int y = ui_init_y_;
    int x = ui_init_x_;

    mvwaddstr(main_, y++, x,
              "Trading Post (Use $ to buy supplies, 0 to go to categories)");

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

bool SetupUI::IsFinished () {
    return state_ == SetupState::DONE;
}

std::string SetupUI::GetCategoryStr ( TradingPostCategory category ) {
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

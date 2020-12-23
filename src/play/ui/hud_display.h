//
// Created by Roberts Kalnins on 22/12/2020.
//

#pragma once

#include "../../loop_control/task.h"
#include "../../input_listener.h"
#include "../../logging/logging.h"
#include "../spaceship.h"
#include "../spaceship_handler.h"
#include "../nav_control_manager.h"


namespace sc::play {

class HUDDisplay : public Task {
  public:

    HUDDisplay ( const std::string &name, TaskType task_type,
                 std::shared_ptr< SpaceshipHandler > spaceship_handler,
                 std::shared_ptr< play::NavigationControlManager > nav_manager,
                 std::shared_ptr< InputListener > listener, WINDOW *main );

    void Init () override;

    GameState OnLoop ( GameState state ) override;

  private:

    void ProcessInput ();

    void PrintCrew ();

    void PrintItems ();

    void PrintHUD ();

    void PrintCategoryDetails ();

  private:
    std::map< int, std::string > cat_details_;

    std::shared_ptr< SpaceshipHandler > spaceship_handler_ { nullptr };

    std::shared_ptr< NavigationControlManager > nav_manager_ { nullptr };

    std::shared_ptr< Spaceship >     spaceship_ { nullptr };
    std::shared_ptr< InputListener > listener_ { nullptr };

    logger_t logger_;

    WINDOW *main_;

    MousePosition mpos_ { 0, 0 };

    const int item_init_y_ { 7 };
    const int item_init_x_ { 30 };
    const int details_init_y_ { 17 };
    const int details_init_x_ { 70 };
    const int spaceship_display_x_ { 10 };
    const int spaceship_display_y_ { 4 };

    const int rows_per_details_pg_ { 10 };
    const int cat_per_page_ { 10 };
    int       details_page_id_ { 0 };
    int       item_page_id_ { 0 };

    bool show_crew_ { false };
    bool show_items_ { false };
    bool show_details_ { false };


};

}





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
                 shared_spaceship_handler_t spaceship_handler,
                 shared_nav_manager_t nav_manager,
                 shared_input_listener_t listener, WINDOW *main );

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

    shared_spaceship_handler_t spaceship_handler_ { nullptr };

    shared_nav_manager_t nav_manager_ { nullptr };

    shared_spaceship_t               spaceship_ { nullptr };
    std::shared_ptr< InputListener > listener_ { nullptr };

    logger_t logger_;

    WINDOW *main_;

    MousePosition mpos_ { 0, 0 };

    int item_init_y_;
    int item_init_x_;
    int details_init_y_;
    int details_init_x_;
    int spaceship_display_y_;
    int spaceship_display_x_;

    int rows_per_details_pg_;
    int cat_per_page_;

    int details_page_id_ { 0 };
    int item_page_id_ { 0 };

    bool show_crew_ { false };
    bool show_items_ { false };
    bool show_details_ { false };


};

}





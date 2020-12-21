//
// Created by Roberts Kalnins on 17/12/2020.
//

#pragma once

#include <queue>

#include "../loop_control/task.h"
#include "objects/spaceship.h"
#include "../input_listener.h"
#include "../logging/logging.h"


namespace sc::play {

struct Checkbox {
    int  y;
    int  left_x;
    int  right_x;
    bool checked;
};


class SpaceshipHandler : public Task {
  public:
    SpaceshipHandler ( const std::string &name,
                       TaskType taskType,
                       std::shared_ptr< InputListener > listener,
                       WINDOW *main );

  public:
    void Init () override;

    GameState OnLoop ( GameState state ) override;

    bool IsFinished () override;

    void
    SetSpaceship ( const std::shared_ptr< play::Spaceship > &spaceship );

    [[nodiscard]] const std::shared_ptr< play::Spaceship > &
    GetSpaceship () const;

    void SetCrew ( const std::vector< CrewMember > &crew_choices,
                   std::vector< Checkbox > &selected );

    [[nodiscard]] double GetDistanceRemaining () const;

    void SetDistanceRemaining ( double distance_remaining );

    [[nodiscard]] double GetInitialDistance () const;

    void UpdateDistanceRemaining (double distance);

    void SetInitialDistance ( double initial_distance );

  private:

    void PrintCrew ();

    void PrintItems ();

    void PrintHUD ();

    void ProcessInput ();

    void PrintCategoryDetails ();

  private:

    std::map< int, std::string > cat_details_;

    std::shared_ptr< play::Spaceship > spaceship_ { nullptr };

    std::shared_ptr< InputListener > listener_ { nullptr };

    WINDOW *main_;

    logger_t logger_;

    MousePosition mpos_ { 0, 0 };

    double distance_remaining_ { 0 };
    double initial_distance_ { 0 };

    bool show_crew_ { false };
    bool show_items_ { false };
    bool show_details_ { false };

    int       item_page_id_ { 0 };
    const int cat_per_page_ { 10 };

    int       details_page_id_ { 0 };
    const int rows_per_details_pg_ { 10 };

    const int spaceship_display_x_ { 10 };
    const int spaceship_display_y_ { 4 };
    const int item_init_y_ { 7 };
    const int item_init_x_ { 30 };

    const int details_init_y_ { 17 };
    const int details_init_x_ { 70 };

};

}





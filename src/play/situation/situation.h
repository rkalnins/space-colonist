//
// Created by Roberts Kalnins on 22/12/2020.
//

#pragma once

#include <string>
#include <utility>

#include "situation_type.h"
#include "../ui/pause_menu.h"
#include "../spaceship.h"
#include "../../config/situation_source.h"


namespace sc::play {

enum class SituationState {
    NEW,
    PROMPT_FOR_HELP,
    WAITING_FOR_HELP,
    FIXING,
    RESOLVED
};


struct MenuTask {
    MenuTask ( std::function< void () > task,
               std::string message,
               std::string current_action, int id )
            : task(std::move(task)),
              message(std::move(message)),
              current_action(std::move(current_action)),
              id(id) {}

    std::function< void () > task;
    std::string              message;
    std::string              current_action;
    int                      id;
};


using menu_tasks_t = std::vector< MenuTask >;


class Situation {
  public:
    Situation ( shared_spaceship_t spaceship,
                std::shared_ptr< PauseMenu > pause_menu,
                SituationType type );

    void SituationCycle ();

    [[nodiscard]] SituationType GetType () const;

    void StartWaitForHelp ();

    bool WaitForHelp ();

    void StartFix ();

    virtual void HealthUpdate ();

    [[nodiscard]] bool IsFixing () const;

    [[nodiscard]] bool PromptForHelp () const;

    [[nodiscard]] int GetRemainingResponseTime () const;

    [[nodiscard]] bool AttemptFix () const;

    [[nodiscard]] const std::unique_ptr< const std::string > &
    GetIssue () const;

    std::string GetSitrepText ();

    void UseMenuOption ( int option );

    void UseMenuOption ( const std::string &action );

    [[nodiscard]] menu_tasks_t &GetMenuTasks ();

  protected:

    template < typename Comparator >
    void UseMenuOptionCmp ( Comparator &cmp );

    bool UseGenericSpareParts ();

    [[nodiscard]] bool ResponseTimeExpired () const;

    virtual void SituationCycleOverride ();

    std::string KillRandomCrew ();


    class OptionIdComparator {
      public:
        explicit OptionIdComparator ( int option ) : option_(option) {
        }

        bool operator() ( const MenuTask &other ) const {
            return option_ == other.id;
        }

      private:
        int option_;
    };


    class OptionNameComparator {
      public:
        explicit OptionNameComparator ( const std::string &task ) : task_(
                task) {
        }

        bool operator() ( const MenuTask &other ) const {
            return task_ == other.current_action;
        }

      private:
        const std::string &task_;
    };


  protected:
    logger_t logger_;

    shared_spaceship_t           spaceship_ { nullptr };
    std::shared_ptr< PauseMenu > pause_menu_ { nullptr };

    menu_tasks_t menu_tasks_;

    std::unique_ptr< const std::string > issue_ { nullptr };

    SituationType  type_ { SituationType::NONE };
    SituationState state_ { SituationState::NEW };

    std::string timer_text_;
    std::string health_update_text_;

    double fix_prob_ { 0 };
    int    response_time_ { 0 };
    int    situation_time_ { 0 };

    int required_cables_ { 0 };
    int required_components_ { 0 };

    double successful_distress_;

  private:

    int max_situation_time_; // sec
    int cycles_per_second_ { 60 };

    int counter_ { 0 };
};


}




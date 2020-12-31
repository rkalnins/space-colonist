//
// Created by Roberts Kalnins on 22/12/2020.
//

#pragma once

#include <string>

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


class Situation {
  public:
    Situation ( shared_spaceship_t spaceship,
                std::shared_ptr< PauseMenu > pause_menu );

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

  protected:

    bool UseGenericSpareParts ();

    [[nodiscard]] bool ResponseTimeExpired () const;

    virtual void SituationCycleOverride ();

    std::string KillRandomCrew ();

  protected:
    logger_t logger_;

    shared_spaceship_t           spaceship_ { nullptr };
    std::shared_ptr< PauseMenu > pause_menu_ { nullptr };

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




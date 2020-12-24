//
// Created by Roberts Kalnins on 22/12/2020.
//

#pragma once

#include <string>

#include "situation_type.h"
#include "../ui/pause_menu.h"
#include "../spaceship.h"


namespace sc::play {

enum class SituationState {
    NEW,
    IGNORED,
    PROMPT_FOR_HELP,
    WAITING_FOR_HELP,
    FIXING,
    RESOLVED
};


class Situation {
  public:
    Situation ( std::shared_ptr< Spaceship > spaceship,
                std::shared_ptr< PauseMenu > pause_menu );

    void SituationCycle ();

    virtual bool IsResolved ();

    [[nodiscard]] SituationType GetType () const;

    void StartWaitForHelp ();

    bool WaitForHelp ();

    void StartFix ();

    virtual void HealthUpdate () = 0;

    [[nodiscard]] bool IsFixing () const;

    [[nodiscard]] bool IsWaitingForHelp () const;

    [[nodiscard]] bool PromptForHelp () const;

    [[nodiscard]] int GetRemainingResponseTime () const;

    [[nodiscard]] bool AttemptFix () const;

    [[nodiscard]] const std::unique_ptr< const std::string > &
    GetIssue () const;

  protected:

    bool UseGenericSpareParts ();

    [[nodiscard]] bool ResponseTimeExpired () const;

    virtual void SituationCycleOverride () = 0;

    std::shared_ptr< Spaceship > spaceship_ { nullptr };
    std::shared_ptr< PauseMenu > pause_menu_ { nullptr };

    std::unique_ptr< const std::string > issue_ { nullptr };

    SituationType type_ { SituationType::NONE };
    SituationState state_ { SituationState::NEW };

    double fix_prob_ { 0 };
    int    response_time_ { 0 };
    int    situation_time_ { 0 };

    int required_cables_ { 0 };
    int required_components_ { 0 };

    const double successful_distress_ { 0.008 };

  private:

    const int max_situation_time_ = 90; // sec
    const int cycles_per_second_  = 60;

    int counter_ { 0 };
};


}




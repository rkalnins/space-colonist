//
// Created by Roberts Kalnins on 30/12/2020.
//

#pragma once

#include "situation.h"
#include "major_situation.h"


namespace sc::play {

class MajorHullBreach : public MajorSituation {

  public:
    MajorHullBreach ( shared_spaceship_t spaceship,
                      std::shared_ptr< PauseMenu > pause_menu );

    void HealthUpdate () override;

    void SealBreach ();

    [[nodiscard]] bool IsCrewEscaped () const;

  protected:
    void SituationCycleOverride () override;

  private:

    double unsealed_fix_death_p_;

    int  crew_escape_time_;
    bool is_crew_escaped_;
    bool update_health_ { false };
    bool is_breach_open_ { true };

};

}




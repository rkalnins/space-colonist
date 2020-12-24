//
// Created by Roberts Kalnins on 23/12/2020.
//

#pragma once

#include "situation.h"


namespace sc::play {

class MinorSituation : public Situation {
  public:

    MinorSituation ( std::shared_ptr< Spaceship > spaceship,
                     std::shared_ptr< PauseMenu > pauseMenu );

  protected:
    void SituationCycleOverride () override;

  public:
    virtual void HealthUpdate () override;


  private:

    const std::vector< const std::string > issue_choices_ = {
            "Toilet control shorting out",
            "Bridge door control browning out",
            "Loose cabling below deck", "Minor coolant leak",
            "Dirty air filter", "Exception thrown by navigation software",
            "Flickering cargo hold power", "High amp draw by HVAC system",
            "Unstable voltage in emergency breach containment system"
    };

    const std::vector< int > components_used_choices_ = { 1, 2 };
    const std::vector< int > cabling_used_choices_    = { 1, 2 };

    bool ignored_ { false };
};


}




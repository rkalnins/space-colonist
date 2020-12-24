//
// Created by Roberts Kalnins on 23/12/2020.
//

#pragma once

#include "major_situation.h"


namespace sc::play {

class AirFilterFailure : public MajorSituation {
  public:
    AirFilterFailure ( std::shared_ptr< Spaceship > spaceship,
                       std::shared_ptr< PauseMenu > pauseMenu );

    void HealthUpdate () override;

  protected:
    void SituationCycleOverride () override;

  private:

    const std::vector< const std::string > issue_choices_ = {
            "Air filter pump failed. Cannot recycle Oxygen.",
            "Air filter clogged. Cannot recycle Oxygen",
            "Air filter unresponsive. Cannot recycle Oxygen.",
    };

    bool is_air_poisoned_ { false };

};


}


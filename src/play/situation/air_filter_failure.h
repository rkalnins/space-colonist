//
// Created by Roberts Kalnins on 23/12/2020.
//

#pragma once

#include "major_situation.h"


namespace sc::play {

class AirFilterFailure : public MajorSituation {
  public:
    AirFilterFailure ( shared_spaceship_t spaceship,
                       std::shared_ptr< PauseMenu > pauseMenu );

    void HealthUpdate () override;

  protected:
    void SituationCycleOverride () override;

  private:

    bool is_air_poisoned_ { false };

};


}


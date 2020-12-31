//
// Created by Roberts Kalnins on 22/12/2020.
//

#pragma once

#include "situation.h"
#include "situation_type.h"


namespace sc::play {

class MajorSituation : public Situation {

  public:
    MajorSituation ( shared_spaceship_t spaceship,
                     std::shared_ptr< PauseMenu > pause_menu,
                     SituationType type );

  protected:
    void SituationCycleOverride () override;
};

}





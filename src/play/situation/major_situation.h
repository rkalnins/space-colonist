//
// Created by Roberts Kalnins on 22/12/2020.
//

#pragma once

#include "situation.h"
#include "situation_type.h"


namespace sc::play {

class MajorSituation : public Situation {

  public:
    MajorSituation ( std::shared_ptr< Spaceship > spaceship,
                     std::shared_ptr< PauseMenu > pauseMenu );

  protected:
    void SituationCycleOverride () override;

  private:
    const std::vector< int > components_used_choices_ = { 2, 3, 5 };
    const std::vector< int > cabling_used_choices_    = { 2, 3, 5 };

};

}





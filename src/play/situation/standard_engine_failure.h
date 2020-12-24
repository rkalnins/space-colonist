//
// Created by Roberts Kalnins on 23/12/2020.
//

#pragma once

#include "major_situation.h"


namespace sc::play {

class StandardEngineFailure : public MajorSituation {
  public:
    StandardEngineFailure ( std::shared_ptr< Spaceship > spaceship,
                            std::shared_ptr< PauseMenu > pauseMenu );

    bool IsResolved () override;

    virtual void HealthUpdate () override;

  protected:
    void SituationCycleOverride () override;

    const std::vector< const std::string > issue_choices_ =
                                                   {
                                                           "Engine coolant system failed. Cannot fire engines.",
                                                           "Engine fuel leak. Cannot fire engines.",
                                                           "Engine control system failed. Cannot fire engines.",
                                                   };

};

}





//
// Created by Roberts Kalnins on 23/12/2020.
//

#pragma once

#include "major_situation.h"


namespace sc::play {

class StandardEngineFailure : public MajorSituation {
  public:
    StandardEngineFailure ( shared_spaceship_t spaceship,
                            std::shared_ptr< PauseMenu > pauseMenu );

  private:

    const std::vector< const std::string > issue_choices_ =
                                                   {
                                                           "Engine coolant system failed. Cannot fire engines.",
                                                           "Engine fuel leak. Cannot fire engines.",
                                                           "Engine control system failed. Cannot fire engines.",
                                                   };

};

}





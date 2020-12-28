//
// Created by Roberts Kalnins on 23/12/2020.
//

#pragma once

#include "major_situation.h"


namespace sc::play {

class StandardEngineFailure : public MajorSituation {
  public:
    StandardEngineFailure ( shared_spaceship_t spaceship,
                            std::shared_ptr< PauseMenu > pause_menu );
};

}





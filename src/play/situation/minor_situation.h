//
// Created by Roberts Kalnins on 23/12/2020.
//

#pragma once

#include "situation.h"


namespace sc::play {

class MinorSituation : public Situation {
  public:

    MinorSituation ( shared_spaceship_t spaceship,
                     std::shared_ptr< PauseMenu > pause_menu );

};


}




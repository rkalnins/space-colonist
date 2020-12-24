//
// Created by Roberts Kalnins on 23/12/2020.
//

#pragma once

#include <memory>

#include "situation_type.h"
#include "situation.h"
#include "major_situation.h"


namespace sc::play {

class SituationFactory {
  public:
    SituationFactory ( std::shared_ptr< PauseMenu > pauseMenu,
                       std::shared_ptr< Spaceship > spaceship );

    std::shared_ptr< Situation > GetSituation ();

    std::shared_ptr< Situation > GetSuddenEngineFailure ();


  private:

    logger_t logger_;

    std::shared_ptr< PauseMenu > pause_menu_;
    std::shared_ptr< Spaceship > spaceship_;

    const double minor_failure_prob_ { 0.001 };
    const double major_failure_prob_ { 0.0002 };
    const double unexpected_engine_prob_ { 0.0002 };

};

}




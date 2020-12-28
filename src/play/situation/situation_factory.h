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
    SituationFactory ( std::shared_ptr< PauseMenu > pause_menu,
                       shared_spaceship_t spaceship );

    std::unique_ptr< Situation > GetSituation ();

    std::unique_ptr< Situation > GetSuddenEngineFailure ();


  private:

    logger_t logger_;

    std::shared_ptr< PauseMenu > pause_menu_;
    shared_spaceship_t           spaceship_;

    double minor_failure_prob_;
    double major_failure_prob_;
    double unexpected_engine_prob_;

};

}




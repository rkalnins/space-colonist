//
// Created by Roberts Kalnins on 23/12/2020.
//

#include "situation_factory.h"

#include <effolkronium/random.hpp>
#include <utility>


#include "situation.h"
#include "minor_situation.h"
#include "standard_engine_failure.h"
#include "air_filter_failure.h"


namespace sc::play {

using Random = effolkronium::random_static;

SituationFactory::SituationFactory (
        std::shared_ptr< PauseMenu > pauseMenu,
        std::shared_ptr< Spaceship > spaceship ) : pause_menu_(std::move(
        pauseMenu)), spaceship_(std::move(spaceship)), logger_(CreateLogger("sit_factory")) {}

std::shared_ptr< Situation > SituationFactory::GetSituation () {

    // FIXME make major dependent on number of minor failures
    if ( Random::get< bool >(major_failure_prob_)) {
        logger_->debug("Creating major failure");
        char f = Random::get({ 'a', 'e' });
        switch ( f ) {
            case 'e':
                return std::make_shared< StandardEngineFailure >(
                        spaceship_, pause_menu_);
            case 'a':
                return std::make_shared< AirFilterFailure >(spaceship_,
                                                            pause_menu_);
            default:
                return nullptr;
        }
    }


    if ( Random::get< bool >(minor_failure_prob_)) {
        logger_->debug("Creating minor failure");
        return std::make_shared< MinorSituation >(spaceship_, pause_menu_);
    }


    return nullptr;
}

std::shared_ptr< Situation > SituationFactory::GetSuddenEngineFailure () {
    if ( Random::get< bool >(unexpected_engine_prob_)) {
        return std::make_shared< StandardEngineFailure >(spaceship_,
                                                         pause_menu_);
    }

    return nullptr;
}

}
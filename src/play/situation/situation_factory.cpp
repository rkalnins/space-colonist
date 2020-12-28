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
        std::shared_ptr< PauseMenu > pause_menu,
        shared_spaceship_t spaceship ) : pause_menu_(std::move(
        pause_menu)), spaceship_(std::move(spaceship)),
                                         logger_(CreateLogger(
                                                 "sit_factory")) {

    SituationSource &source = SituationSource::GetInstance();

    minor_failure_prob_     = source.GetValue("minor.failure-p", 0.0);
    major_failure_prob_     = source.GetValue("major.failure-p", 0.0);
    unexpected_engine_prob_ = source.GetValue(
            "major.unexpected-engine.failure-p", 0.0);

}

std::unique_ptr< Situation > SituationFactory::GetSituation () {

    // FIXME make major dependent on number of minor failures
    if ( Random::get< bool >(major_failure_prob_)) {
        logger_->debug("Creating major failure");
        char f = Random::get({ 'a', 'e' });
        switch ( f ) {
            case 'e':
                return std::make_unique< StandardEngineFailure >(
                        spaceship_, pause_menu_);
            case 'a':
                return std::make_unique< AirFilterFailure >(spaceship_,
                                                            pause_menu_);
            default:
                return nullptr;
        }
    }


    if ( Random::get< bool >(minor_failure_prob_)) {
        logger_->debug("Creating minor failure");
        return std::make_unique< MinorSituation >(spaceship_, pause_menu_);
    }


    return nullptr;
}

std::unique_ptr< Situation > SituationFactory::GetSuddenEngineFailure () {
    if ( Random::get< bool >(unexpected_engine_prob_)) {
        return std::make_unique< StandardEngineFailure >(spaceship_,
                                                         pause_menu_);
    }

    return nullptr;
}

}
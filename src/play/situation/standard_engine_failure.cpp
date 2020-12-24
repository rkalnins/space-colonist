//
// Created by Roberts Kalnins on 23/12/2020.
//

#include "standard_engine_failure.h"
#include <effolkronium/random.hpp>
#include <utility>


namespace sc::play {

using Random = effolkronium::random_static;

StandardEngineFailure::StandardEngineFailure (
        std::shared_ptr< Spaceship > spaceship,
        std::shared_ptr< PauseMenu > pauseMenu ) : MajorSituation(
        std::move(spaceship), std::move(pauseMenu)) {
    issue_ = std::make_unique< const std::string >(
            *Random::get(issue_choices_));

    type_ = SituationType::ENGINE_FAILURE;
}

bool StandardEngineFailure::IsResolved () {
    return MajorSituation::IsResolved();
}

void StandardEngineFailure::SituationCycleOverride () {
    MajorSituation::SituationCycleOverride();
}

void StandardEngineFailure::HealthUpdate () {

}
}
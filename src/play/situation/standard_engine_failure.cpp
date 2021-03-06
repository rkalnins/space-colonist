//
// Created by Roberts Kalnins on 23/12/2020.
//

#include "standard_engine_failure.h"
#include <effolkronium/random.hpp>
#include <utility>


namespace sc::play {

using Random = effolkronium::random_static;

StandardEngineFailure::StandardEngineFailure (
        shared_spaceship_t spaceship,
        std::shared_ptr< PauseMenu > pause_menu ) : MajorSituation(
        std::move(spaceship), std::move(pause_menu),
        SituationType::ENGINE_FAILURE) {

    static const std::vector< std::string > issue_choices = SituationSource::GetInstance().GetList< std::string >(
            "major.standard-engine.failures");

    issue_ = std::make_unique< const std::string >(
            *Random::get(issue_choices));

    std::vector< std::string > sitrep_options_ = SituationSource::GetInstance().GetList< std::string >(
            GetTypePath(type_) + ".options");

    menu_tasks_.emplace_back([this] { StartFix(); }, sitrep_options_[1],
                             "Fixing",
                             1);

}

}
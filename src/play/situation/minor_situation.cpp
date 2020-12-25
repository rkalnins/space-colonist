//
// Created by Roberts Kalnins on 23/12/2020.
//

#include "minor_situation.h"
#include <effolkronium/random.hpp>
#include <utility>


namespace sc::play {

using Random = effolkronium::random_static;

MinorSituation::MinorSituation (
        shared_spaceship_t spaceship,
        std::shared_ptr< PauseMenu > pauseMenu )
        : Situation(std::move(spaceship), std::move(pauseMenu)) {

    issue_ = std::make_unique< const std::string >(
            *Random::get(issue_choices_));

    type_                = SituationType::MINOR;
    fix_prob_            = 0.01;
    required_components_ = *Random::get(components_used_choices_);
    required_cables_     = *Random::get(cabling_used_choices_);
}

}
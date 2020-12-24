//
// Created by Roberts Kalnins on 22/12/2020.
//

#include "major_situation.h"
#include <effolkronium/random.hpp>
#include <utility>


namespace sc::play {

using Random = effolkronium::random_static;

MajorSituation::MajorSituation (
        std::shared_ptr< Spaceship > spaceship,
        std::shared_ptr< PauseMenu > pauseMenu )
        : Situation(std::move(spaceship),
                    std::move(pauseMenu)) {


    required_components_ = *Random::get(components_used_choices_);
    required_cables_     = *Random::get(cabling_used_choices_);

    fix_prob_ = 0.003;
}

void MajorSituation::SituationCycleOverride () {

}

bool MajorSituation::IsResolved () {
    return Situation::IsResolved();
}
}
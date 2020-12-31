//
// Created by Roberts Kalnins on 22/12/2020.
//

#include "major_situation.h"
#include <effolkronium/random.hpp>
#include <utility>


namespace sc::play {

using Random = effolkronium::random_static;

MajorSituation::MajorSituation (
        shared_spaceship_t spaceship,
        std::shared_ptr< PauseMenu > pause_menu, SituationType type )
        : Situation(std::move(spaceship),
                    std::move(pause_menu), type) {

    SituationSource &source = SituationSource::GetInstance();

    static const std::vector< int > component_choices = source.GetList< int >(
            "major.spares.components");
    static const std::vector< int > cable_choices     = source.GetList< int >(
            "major.spares.cables");

    required_components_ = *Random::get(component_choices);
    required_cables_     = *Random::get(cable_choices);

    fix_prob_ = source.GetValue("major.fix-p", 0.0);
}

void MajorSituation::SituationCycleOverride () {

}

}
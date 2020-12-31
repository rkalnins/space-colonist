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
        std::shared_ptr< PauseMenu > pause_menu )
        : Situation(std::move(spaceship), std::move(pause_menu),
                    SituationType::MINOR) {


    SituationSource &source = SituationSource::GetInstance();

    static const std::vector< std::string > issue_choices = source.GetList< std::string >(
            "minor.failures");

    issue_ = std::make_unique< const std::string >(
            *Random::get(issue_choices));

    fix_prob_ = source.GetValue("minor.fix-p", 0.0);

    static const std::vector< int > component_choices = source.GetList< int >(
            "minor.spares.components");
    static const std::vector< int > cable_choices     = source.GetList< int >(
            "minor.spares.cables");

    required_components_ = *Random::get(component_choices);
    required_cables_     = *Random::get(cable_choices);

    std::vector< std::string > sitrep_options_ = SituationSource::GetInstance().GetList< std::string >(
            GetTypePath(type_) + ".options");

    menu_tasks_.emplace_back(nullptr, sitrep_options_[0], "Ignoring", 1);
    menu_tasks_.emplace_back([this] { StartFix(); }, sitrep_options_[1],
                             "Fixing",
                             2);
}

}
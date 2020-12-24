//
// Created by Roberts Kalnins on 23/12/2020.
//

#include <initializer_list>

#include "air_filter_failure.h"
#include <effolkronium/random.hpp>
#include <utility>


namespace sc::play {

using Random = effolkronium::random_static;

AirFilterFailure::AirFilterFailure (
        std::shared_ptr< Spaceship > spaceship,
        std::shared_ptr< PauseMenu > pauseMenu ) : MajorSituation(
        std::move(spaceship), std::move(pauseMenu)) {

    issue_         = std::make_unique< std::string >(
            *Random::get(issue_choices_));
    type_          = SituationType::AIR_FILTER_FAILURE;
    response_time_ = 15; // sec
}

void AirFilterFailure::SituationCycleOverride () {
    if ( !is_air_poisoned_ && ResponseTimeExpired()) {
        is_air_poisoned_ = true;
    }
}

void AirFilterFailure::HealthUpdate () {
    std::vector< CrewMember > &crew = spaceship_->GetCrew();

    if ( is_air_poisoned_ ) {
        for ( auto &c : crew ) {
            c.UpdateHealth(-2);
        }
    }

    for ( auto &c : crew ) {
        if ( c.IsDead()) {
            pause_menu_->PushNotification(
                    c.GetName() +
                    " has suffocated to death from poisoned air :(");
        }
    }
}


}
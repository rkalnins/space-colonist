//
// Created by Roberts Kalnins on 23/12/2020.
//

#include <initializer_list>

#include "air_filter_failure.h"
#include <effolkronium/random.hpp>


namespace sc::play {

using Random = effolkronium::random_static;

AirFilterFailure::AirFilterFailure (
        shared_spaceship_t spaceship,
        std::shared_ptr< PauseMenu > pauseMenu ) : MajorSituation(
        std::move(spaceship), std::move(pauseMenu)) {

    static const std::vector< std::string > issue_choices = SituationSource::GetInstance().GetList< std::string >(
            "major.air-filter.failures");

    issue_         = std::make_unique< std::string >(
            *Random::get(issue_choices));
    type_          = SituationType::AIR_FILTER_FAILURE;
    response_time_ = SituationSource::GetInstance().GetValue(
            "major.air-filter.response-time", 0); // sec

    timer_text_ = SituationSource::GetInstance().GetValue<std::string>(
            "major.air-filter.timer-text", "");

    health_update_text_ = SituationSource::GetInstance().GetValue<std::string>(
            "major.air-filter.health-update-text", "");
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
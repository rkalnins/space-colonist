//
// Created by Roberts Kalnins on 30/12/2020.
//

#include "major_hull_breach.h"


#include <effolkronium/random.hpp>
#include <utility>


namespace sc::play {

using Random = effolkronium::random_static;


MajorHullBreach::MajorHullBreach ( shared_spaceship_t spaceship,
                                   std::shared_ptr< PauseMenu > pause_menu )
        : MajorSituation(std::move(spaceship), std::move(pause_menu)) {

    SituationSource &source = SituationSource::GetInstance();

    static const std::vector< std::string > issue_choices = source.GetList< std::string >(
            "major.hull.failures");

    issue_         = std::make_unique< std::string >(
            *Random::get(issue_choices));
    type_          = SituationType::MAJOR_HULL_BREACH;
    response_time_ = SituationSource::GetInstance().GetValue(
            "major.hull.response-time", 0); // sec

    double unlucky_death_p = source.GetValue(
            "major.hull.unlucky-crew-death",
            0.0);

    unsealed_fix_death_p_ = source.GetValue(
            "major.hull.death-if-not-sealed-p",
            0.0);

    Range escape_times = source.GetRange("major.hull.crew-escape-time");
    crew_escape_time_ = Random::get< int >(escape_times.min,
                                           escape_times.max);

    if ( Random::get< bool >(unlucky_death_p)) {
        pause_menu_->PushNotification(KillRandomCrew() +
                                      " died after rapid depressurization sucked them into space.");
        is_crew_escaped_ = true;
    } else {
        double stuck_p = source.GetValue("major.hull.crew-stuck-p", 0.0);
        is_crew_escaped_ = Random::get< bool >(1.0 - stuck_p);
    }

    if (is_crew_escaped_) {
        pause_menu_->PushNotification(
                "Hull breach: Zone is clear of crew members. Ready to seal");
    } else {
        pause_menu_->PushNotification(
                "Hull breach: Zone is not clear of crew members.");
    }

    timer_text_ = SituationSource::GetInstance().GetValue< std::string >(
            "major.hull.timer-text", "");

    health_update_text_ = SituationSource::GetInstance().GetValue< std::string >(
            "major.hull.health-update-text", "");
}

void MajorHullBreach::SituationCycleOverride () {
    if ( !update_health_ && ResponseTimeExpired() && is_breach_open_ ) {
        update_health_ = true;
    } else {
        update_health_ = false;
    }

    if ( is_breach_open_ && state_ == SituationState::FIXING ) {
        if ( Random::get< bool >(unsealed_fix_death_p_)) {
            fix_prob_ *= 10.0; // be nice to player. if dies then help fix

            pause_menu_->PushNotification(KillRandomCrew() +
                                          " died while fixing an unsealed and uncontrolled breach");
        }
    }

    if ( is_breach_open_ && !is_crew_escaped_ &&
         situation_time_ > crew_escape_time_ ) {
        pause_menu_->PushNotification("All crew has escaped breach area");
        is_crew_escaped_ = true;
    }
}

void MajorHullBreach::HealthUpdate () {
    std::vector< CrewMember > &crew = spaceship_->GetCrew();

    if ( update_health_ ) {
        for ( auto &c : crew ) {
            c.UpdateHealth(-2);
        }
    }

    for ( auto &c : crew ) {
        if ( c.IsDead()) {
            pause_menu_->PushNotification(
                    c.GetName() +
                    " has been killed by a hull breach :(");
        }
    }
}

void MajorHullBreach::SealBreach () {
    is_breach_open_     = false;
    timer_text_         = "";
    health_update_text_ = "";

    logger_->debug("Sealing breach, crew escaped {}", is_crew_escaped_);

    if ( !is_crew_escaped_ ) {
        pause_menu_->PushNotification(KillRandomCrew() +
                                      " died after getting sealed in the breach zone");
    }
}

bool MajorHullBreach::IsCrewEscaped () const {
    return is_crew_escaped_;
}


}




//
// Created by Roberts Kalnins on 22/12/2020.
//

#include "situation.h"

#include <utility>
#include <effolkronium/random.hpp>


namespace sc::play {

using Random = effolkronium::random_static;

static logger_t logger = CreateLogger("situation");

Situation::Situation ( std::shared_ptr< Spaceship > spaceship,
                       std::shared_ptr< PauseMenu > pause_menu )
        : spaceship_(std::move(spaceship)),
          pause_menu_(std::move(pause_menu)) {
    logger->set_level(spdlog::level::debug);
    logger->debug("Situation constructor");
}

void Situation::SituationCycle () {
    ++counter_;
    situation_time_ = counter_ / cycles_per_second_;
    SituationCycleOverride();
}

bool Situation::IsResolved () {
    return false;
}

SituationType Situation::GetType () const { return type_; }

bool Situation::UseGenericSpareParts () {
    logger->debug("Using spare parts");
    if ( !spaceship_->UseSpareParts(required_cables_,
                                    required_components_)) {
        logger->debug("Not enough spare parts");
        pause_menu_->PushNotification("Not enough spare parts");
        enough_spares_ = false;
        return false;
    }

    logger->debug("Enough spare parts");
    enough_spares_ = true;
    return true;
}

const std::unique_ptr< const std::string > &Situation::GetIssue () const {
    return issue_;
}

bool Situation::AttemptFix () const {

    if ( !fixing_ ) { return false; }

    if ( Random::get< bool >(fix_prob_)) {
        const std::string &fixed = "Fixed \"" + *issue_ +
                                   "\"";
        pause_menu_->PushNotification(fixed);

        return true;
    }
    return false;
}

bool Situation::ResponseTimeExpired () const {
    return response_time_ - situation_time_ <= 0;
}

void Situation::StartFix () {
    logger->debug("Attempting to start fix");
    if ( UseGenericSpareParts()) {
        logger->debug("Starting fix");
        fixing_ = true;
    }
}

void Situation::StartWaitForHelp () {
    if ( !enough_spares_ ) {
        waiting_for_help_ = true;
    }
}

bool Situation::IsFixing () const {
    return fixing_;
}

bool Situation::WaitForHelp () {
    if ( !waiting_for_help_ ) { return false; }

    if ( Random::get< bool >(successful_distress_)) {
        pause_menu_->PushNotification("Saved by friendly aliens.");
        waiting_for_help_ = false;
        return true;
    }

    return false;
}

bool Situation::IsWaitingForHelp () const {
    return waiting_for_help_;
}

bool Situation::PromptForHelp () const {
    return !enough_spares_ && !waiting_for_help_;
}

int Situation::GetRemainingResponseTime () const {
    return response_time_ - situation_time_;
}

}
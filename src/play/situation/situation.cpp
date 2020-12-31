//
// Created by Roberts Kalnins on 22/12/2020.
//

#include "situation.h"

#include <utility>
#include <sstream>
#include <effolkronium/random.hpp>


namespace sc::play {

using Random = effolkronium::random_static;


Situation::Situation ( shared_spaceship_t spaceship,
                       std::shared_ptr< PauseMenu > pause_menu,
                       SituationType type )
        : spaceship_(std::move(spaceship)),
          pause_menu_(std::move(pause_menu)),
          logger_(CreateLogger("situation")), type_(type) {

    SituationSource &source = SituationSource::GetInstance();

    successful_distress_ = source.GetValue("distress-success-p", 0.0);

    max_situation_time_ = source.GetValue("max-time", 0);
}

void Situation::SituationCycle () {
    ++counter_;
    situation_time_ = counter_ / cycles_per_second_;

    if ( situation_time_ > max_situation_time_ ) {
        state_ = SituationState::RESOLVED;
        pause_menu_->PushNotification("Situation resolved");
        return;
    }

    SituationCycleOverride();
}

SituationType Situation::GetType () const { return type_; }

bool Situation::UseGenericSpareParts () {
    logger_->debug("Using spare parts");
    if ( !spaceship_->UseSpareParts(required_cables_,
                                    required_components_)) {
        logger_->debug("Not enough spare parts");
        pause_menu_->PushNotification("Not enough spare parts");
        state_ = SituationState::PROMPT_FOR_HELP;
        return false;
    }

    logger_->debug("Enough spare parts");
    return true;
}

const std::unique_ptr< const std::string > &Situation::GetIssue () const {
    return issue_;
}

bool Situation::AttemptFix () const {

    if ( state_ != SituationState::FIXING ) { return false; }

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
    if ( state_ == SituationState::FIXING ) {
        logger_->debug("Already fixing");
        return;
    }

    logger_->debug("Attempting to start fix");
    if ( UseGenericSpareParts()) {
        logger_->debug("Starting fix");
        state_ = SituationState::FIXING;
    }
}

void Situation::StartWaitForHelp () {
    if ( state_ == SituationState::PROMPT_FOR_HELP ) {
        state_ = SituationState::WAITING_FOR_HELP;
    }
}

bool Situation::IsFixing () const {
    return state_ == SituationState::FIXING;
}

bool Situation::WaitForHelp () {
    if ( state_ != SituationState::WAITING_FOR_HELP ) { return false; }

    if ( Random::get< bool >(successful_distress_)) {
        pause_menu_->PushNotification("Saved by friendly aliens.");
        state_ = SituationState::RESOLVED;
        return true;
    }

    return false;
}

bool Situation::PromptForHelp () const {
    return state_ == SituationState::PROMPT_FOR_HELP;
}

int Situation::GetRemainingResponseTime () const {
    return response_time_ - situation_time_;
}

void Situation::SituationCycleOverride () {}

void Situation::HealthUpdate () {}

std::string Situation::GetSitrepText () {
    if ( timer_text_.empty() || health_update_text_.empty()) {
        return "";
    }

    std::stringstream disp;

    if ( GetRemainingResponseTime() >= 0 ) {
        disp << GetRemainingResponseTime() << timer_text_;
        return disp.str();
    }

    return health_update_text_;
}

std::string Situation::KillRandomCrew () {
    std::vector< CrewMember > &crew = spaceship_->GetCrew();

    auto random_crew = Random::get(crew);
    random_crew->UpdateHealth(-1 * random_crew->GetHealth());

    std::string name = random_crew->GetName();
    spaceship_->RemoveDeadCrew();

    return name;
}

void Situation::UseMenuOption ( int option ) {

    logger_->debug("Using menu option {}", option);
    if ( option >= menu_tasks_.size() || option < 0 ) { return; }

    OptionComparator cmp(option);

    menu_tasks_.erase(
            std::remove_if(menu_tasks_.begin(), menu_tasks_.end(), cmp),
            menu_tasks_.end());
    int       index = 0;
    for ( int i     = 1; i <= menu_tasks_.size(); ++i ) {
        logger_->debug("Option {} is now {}", menu_tasks_[index].id, i);
        menu_tasks_[index++].id = i;
    }
}

menu_tasks_t &Situation::GetMenuTasks () {
    return menu_tasks_;
}

}
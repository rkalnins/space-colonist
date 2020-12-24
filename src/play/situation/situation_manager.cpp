//
// Created by Roberts Kalnins on 22/12/2020.
//

#include "situation_manager.h"

#include <sstream>
#include <effolkronium/random.hpp>
#include <utility>


namespace sc::play {

using Random = effolkronium::random_static;

SituationManager::SituationManager ( WINDOW *main,
                                     const std::shared_ptr< Spaceship > &spaceship,
                                     const std::shared_ptr< PauseMenu > &pause_menu )
        : logger_(CreateLogger("sitmgr")), main_(main),
          spaceship_(spaceship),
          pause_menu_(pause_menu),
          situation_factory_(pause_menu,
                             spaceship) {}


void SituationManager::ShowSituationReport () {
    int y = pause_y_;
    int x = pause_x_;

    if ( situations_.empty()) { return; }

    mvwaddstr(main_, y++, x - 8, "Situation Report");
    mvwaddstr(main_, y++, x - 23,
              "---------------------------------------------");
    if ( !situations_.empty()) {
        mvwaddstr(main_, y++,
                  x - situations_.front()->GetIssue()->length() / 2,
                  situations_.front()->GetIssue()->c_str());
        mvwaddstr(main_, y++, x - 23,
                  "---------------------------------------------");
    }

    for ( auto &o : sitrep_options_[situations_.front()->GetType()] ) {
        mvwaddstr(main_, y++, x - 12, o.c_str());
    }

    y++;
    if ( situations_.front()->PromptForHelp()) {
        mvwaddstr(main_, y++, x - 12,
                  "Not enough spares. Issue distress signal (y/n)?");
    }

    if ( situations_.front()->IsFixing()) {
        mvwaddstr(main_, y++, x - 12, "Current Action: Fixing");
    }

    if ( situations_.front()->IsWaitingForHelp()) {
        mvwaddstr(main_, y++, x - 12, "Current Action: Waiting for help");
    }

    if ( situations_.front()->GetType() ==
         SituationType::AIR_FILTER_FAILURE ) {
        if ( situations_.front()->GetRemainingResponseTime() >= 0 ) {
            std::stringstream disp;
            disp << situations_.front()->GetRemainingResponseTime()
                 << " before poisoning begins";
            mvwaddstr(main_, y++, x - 12, disp.str().c_str());
        } else {
            mvwaddstr(main_, y++, x - 12,
                      "Crew is dying of poisoned air.");
        }
    }
}

bool SituationManager::CheckNewSituation () {
    if ( !situations_.empty()) {
        return false;
    }

    std::shared_ptr< Situation > tmp = situation_factory_.GetSituation();

    if ( tmp ) {
        logger_->debug("Got situation");
        situations_.push(tmp);
        return true;
    }

    return false;
}

void SituationManager::FixMinorIgnored () {
    if ( ignored_minor_issues_.empty()) { return; }

    if ( ignored_minor_issues_.front()->AttemptFix()) {
        logger_->debug("Minor ignored issue resolved");

        if ( !ignored_minor_issues_.empty()) {
            ignored_minor_issues_.pop();
        }
    }
}

SituationType SituationManager::GetSituationType () const {
    if ( situations_.empty()) { return SituationType::NONE; }

    return situations_.front()->GetType();
}

int SituationManager::GetIgnoredFailures () const {
    return ignored_minor_issues_.size();
}

bool SituationManager::SuddenEngineFailure () {
    std::shared_ptr< Situation > tmp = situation_factory_.GetSuddenEngineFailure();

    if ( tmp ) {
        situations_.push(tmp);
        return true;
    }

    return false;
}

bool SituationManager::IsSituation () {
    return !situations_.empty();
}

bool SituationManager::IsEngineFailure () {
    if ( situations_.empty()) { return false; }

    return situations_.front()->GetType() == SituationType::ENGINE_FAILURE;
}

bool SituationManager::UpdateSituation () {

    if ( situations_.empty()) { return false; }

    ShowSituationReport();

    situations_.front()->SituationCycle();

    // ending conditions for a situation, either help arrives or is fixed
    if ( situations_.front()->AttemptFix() ||
         situations_.front()->WaitForHelp()) {

        if ( !situations_.empty()) {
            situations_.pop();
        }

        return true;
    }

    return false;
}

bool SituationManager::ProcessInput ( int c ) {
    switch ( c ) {
        case 32:
            pause_menu_->PushNotification("Situation in Progress");
            break;
        case '1': {
            if ( situations_.front()->GetType() == SituationType::MINOR ) {
                logger_->debug("Ignoring minor");
                ignored_minor_issues_.push(situations_.front());

                if ( !situations_.empty()) {
                    situations_.pop();
                }
                return true;
            }

            logger_->debug("Starting to fix");
            situations_.front()->StartFix();
            break;
        }
        case '2': {
            situations_.front()->StartFix();
        }
        case 'y': {
            situations_.front()->StartWaitForHelp();
            break;
        }
        default:
            break;
    }

    return false;
}

bool SituationManager::CanFixMinorIgnoredIssue () {
    if ( ignored_minor_issues_.empty()) {
        return false;
    }

    pause_menu_->ClearLastNotification();
    logger_->debug("Trying to fix minor issue 1 of {}",
                   ignored_minor_issues_.size());
    ignored_minor_issues_.front()->StartFix();

    return true;
}

void SituationManager::UpdateHealth () {
    if ( situations_.empty()) { return; }

    situations_.front()->HealthUpdate();
}

}
//
// Created by Roberts Kalnins on 22/12/2020.
//

#include "situation_manager.h"

#include <sstream>
#include <effolkronium/random.hpp>
#include <utility>

#include "major_hull_breach.h"


namespace sc::play {

using Random = effolkronium::random_static;

SituationManager::SituationManager ( WINDOW *main,
                                     const shared_spaceship_t &spaceship,
                                     const std::shared_ptr< PauseMenu > &pause_menu )
        : logger_(CreateLogger("sitmgr")), main_(main),
          spaceship_(spaceship),
          pause_menu_(pause_menu),
          situation_factory_(pause_menu,
                             spaceship) {

    pause_y_ = Config::GetInstance().GetValue("pause.y", 0);
    pause_x_ = Config::GetInstance().GetValue("pause.x", 0);

}


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

    for ( auto &o : sitrep_options_ ) {
        mvwaddstr(main_, y++, x - 12, o.c_str());
    }

    y++;
    if ( situations_.front()->PromptForHelp()) {
        mvwaddstr(main_, y++, x - 12,
                  "Not enough spares. Issue distress signal (y/n)?");
    } else if ( !current_action_.empty()) {
        std::string output = "Current Action: " + current_action_;
        mvwaddstr(main_, y++, x - 12, output.c_str());
    }


    std::string message = situations_.front()->GetSitrepText();

    if ( !message.empty()) {
        mvwaddstr(main_, y++, x - 12, message.c_str());
    }

}

bool SituationManager::CheckNewSituation () {
    if ( !situations_.empty()) {
        return false;
    }

    std::shared_ptr< Situation > tmp = situation_factory_.GetSituation();

    if ( tmp ) {
        current_action_ = "";
        logger_->debug("Got situation");
        sitrep_options_ = SituationSource::GetInstance().GetList< std::string >(
                GetTypePath(tmp->GetType()) + ".options");
        sitrep_options_used_.clear();
        sitrep_options_used_.resize(sitrep_options_.size(), false);
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
            switch ( situations_.front()->GetType()) {
                case SituationType::MINOR: {
                    logger_->debug("Ignoring minor");
                    ignored_minor_issues_.push(situations_.front());

                    if ( !situations_.empty()) {
                        situations_.pop();
                    }
                    return true;
                }
                case SituationType::MAJOR_HULL_BREACH: {

                    if ( !sitrep_options_used_[0] ) {
                        std::shared_ptr< MajorHullBreach > breach = std::dynamic_pointer_cast< MajorHullBreach >(
                                situations_.front());

                        breach->SealBreach();
                        logger_->debug("Sealing breach");
                        current_action_ = "Sealed breach";

                        UseMenuOption(1);
                        UseMenuOption(2);
                    }
                    break;
                }
                default: {
                    logger_->debug("Starting to fix");
                    situations_.front()->StartFix();
                    current_action_ = "Fixing";
                    break;
                }
            }
            break;
        }
        case '2': {
            switch ( situations_.front()->GetType()) {
                case SituationType::MAJOR_HULL_BREACH: {
                    std::shared_ptr< MajorHullBreach > breach = std::dynamic_pointer_cast< MajorHullBreach >(
                            situations_.front());

                    UseMenuOption(2);

                    if ( !sitrep_options_used_[1] &&
                         !breach->IsCrewEscaped()) {
                        current_action_ = "Waiting for crew to escape";
                        logger_->debug("Start waiting for crew to escape");
                    }
                    break;
                }
                default:
                    situations_.front()->StartFix();
                    current_action_ = "Fixing";
                    break;
            }
            break;
        }
        case '3': {
            switch ( situations_.front()->GetType()) {
                case SituationType::MAJOR_HULL_BREACH: {
                    logger_->debug("Starting to fix");
                    situations_.front()->StartFix();
                    current_action_ = "Fixing";
                    UseMenuOption(1);
                    UseMenuOption(2);
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case 'y': {
            situations_.front()->StartWaitForHelp();
            current_action_ = "Waiting for help";
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

void SituationManager::UseMenuOption ( int option ) {

    logger_->debug("Using menu option {}", option);
    if ( option > 8 || option < 0 ) { return; }

    OptionComparator cmp(option);

    logger_->debug("Searching for {}", cmp.GetOption());

    auto opt_at = std::find_if(sitrep_options_.begin(),
                               sitrep_options_.end(), cmp);


    if ( opt_at != sitrep_options_.end()) {
        int i = std::distance(sitrep_options_.begin(), opt_at);
        logger_->debug("Option at index {}", i);
        sitrep_options_used_[i] = true;

        // fix numbering
        sitrep_options_[i]    = " ";
        sitrep_options_[i][0] = ( '1' + i );
        sitrep_options_[i].append(". n/a");
    }

}

}
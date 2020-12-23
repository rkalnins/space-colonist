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
                                     std::shared_ptr< Spaceship > spaceship,
                                     std::shared_ptr< PauseMenu > pause_menu )
        : logger_(
        CreateLogger("sitmgr")), main_(main),
          spaceship_(std::move(spaceship)),
          pause_menu_(std::move(pause_menu)) {}


void SituationManager::ShowSituationReport () {
    int y = pause_y_;
    int x = pause_x_;

    mvwaddstr(main_, y++, x - 8, "Situation Report");
    mvwaddstr(main_, y++, x - 23,
              "---------------------------------------------");
    if ( !situations_.empty()) {
        mvwaddstr(main_, y++, x - situations_.front().length() / 2,
                  situations_.front().c_str());
        mvwaddstr(main_, y++, x - 23,
                  "---------------------------------------------");
    }

    for ( auto &o : sitrep_options_[situation_type_] ) {
        mvwaddstr(main_, y++, x - 12, o.c_str());
    }

    y++;
    if ( !waiting_for_help_ && !enough_spares_ ) {
        mvwaddstr(main_, y++, x - 12,
                  "Not enough spares. Issue distress signal (y/n)?");
    }

    if ( enough_spares_ && fixing_ ) {
        mvwaddstr(main_, y++, x - 12, "Current Action: Fixing");
    }

    if ( waiting_for_help_ ) {
        mvwaddstr(main_, y++, x - 12, "Current Action: Waiting for help");
    }

    if ( situation_type_ == SituationType::AIR_FILTER_FAILURE ) {
        int seconds = air_response_time_ -
                      ( situation_counter_ / second_count_period_ );
        if ( seconds >= 0 ) {
            std::stringstream disp;
            disp << seconds << " before poisoning begins";
            mvwaddstr(main_, y++, x - 12, disp.str().c_str());
        } else {
            mvwaddstr(main_, y++, x - 12,
                      "Crew is dying of poisoned air.");
        }
    }
}

bool SituationManager::CheckNewSituation () {
    if ( situation_type_ != SituationType::NONE ) {
        return false;
    }

    double major_p_ = std::min(1.0,
                               major_failure_prob_ *
                               ( ignored_minor_mech_failures_ + 1 ));

    if ( Random::get< bool >(major_p_)) {
        char f = Random::get({ 'a', 'e' });
        switch ( f ) {
            case 'e':
                situation_type_ = SituationType::ENGINE_FAILURE;
                situations_.push(*Random::get(engine_failures_));
                break;
            case 'a':
                situation_type_ = SituationType::AIR_FILTER_FAILURE;
                situations_.push(*Random::get(air_filter_failure_));
                break;
            default:
                situation_type_ = SituationType::NONE;
        }

        req_cabling_    = *Random::get(cabling_used_major_);
        req_components_ = *Random::get(components_used_major_);

        logger_->debug("Req cabling/comp: {} {}", req_cabling_,
                       req_components_);

        logger_->debug("Major failure");
        return true;
    }


    if ( Random::get< bool >(minor_failure_prob_)) {
        situation_type_ = SituationType::MINOR;
        situations_.push(*Random::get(minor_failures_));


        req_cabling_    = *Random::get(cabling_used_minor_);
        req_components_ = *Random::get(components_used_minor_);

        logger_->debug("Req cabling/comp: {} {}", req_cabling_,
                       req_components_);
        logger_->debug("Minor failure");
        return true;
    }

    return false;
}

bool SituationManager::WaitForHelp () {
    if ( !waiting_for_help_ ) { return false; }

    if ( Random::get< bool >(successful_distress_)) {
        pause_menu_->PushNotification("Saved by friendly aliens.");
        situation_type_    = SituationType::NONE;
        waiting_for_help_  = false;
        is_air_poisoned_   = false;
        enough_spares_     = true;
        situation_counter_ = 0;

        if ( !situations_.empty()) {
            situations_.pop();
        }

        return true;
    }

    return false;
}

void SituationManager::FixMinorIgnored () {
    if ( !fixing_minor_ ) { return; }

    if ( Random::get< bool >(minor_fix_prob_)) {
        logger_->debug("Fixed ignored issue");
        std::string fixed = "Fixed ignored minor issue";
        pause_menu_->PushNotification(fixed);
        fixing_minor_  = false;
        enough_spares_ = true;

        --ignored_minor_mech_failures_;

        if ( ignored_minor_mech_failures_ <= 0 ) {
            ignored_minor_mech_failures_ = 0;
        }
    }
}

bool SituationManager::FixMinor () {
    if ( Random::get< bool >(minor_fix_prob_)) {
        std::string fixed = "Fixed \"" + situations_.front() +
                            "\"";
        pause_menu_->PushNotification(fixed);
        situation_type_ = SituationType::NONE;
        fixing_         = false;

        if ( !situations_.empty()) {
            situations_.pop();
        }

        return true;
    }

    return false;
}

bool SituationManager::AttemptFix () {

    if ( !fixing_ ) {
        return false;
    }

    switch ( situation_type_ ) {
        case SituationType::MINOR:
            return FixMinor();
        case SituationType::ENGINE_FAILURE: {
            if ( Random::get< bool >(major_fix_prob_)) {
                pause_menu_->PushNotification(
                        "Fixed engine. Engine nominal.");
                situation_type_    = SituationType::NONE;
                situation_counter_ = 0;
                fixing_            = false;
                enough_spares_     = true;

                if ( !situations_.empty()) {
                    situations_.pop();
                }

                return true;
            }
            break;
        }
        case SituationType::AIR_FILTER_FAILURE: {
            if ( Random::get< bool >(major_fix_prob_)) {
                pause_menu_->PushNotification("Fixed air filter.");
                situation_type_    = SituationType::NONE;
                is_air_poisoned_   = false;
                enough_spares_     = true;
                situation_counter_ = 0;
                fixing_            = false;

                if ( !situations_.empty()) {
                    situations_.pop();
                }

                return true;
            }
            break;
        }
        default:
            break;
    }

    return false;
}

SituationType SituationManager::GetSituationType () const {
    return situation_type_;
}

int SituationManager::GetIgnoredFailures () const {
    return ignored_minor_mech_failures_;
}

bool SituationManager::SuddenEngineFailure () {
    if ( Random::get< bool >(unresponsive_engines_)) {
        situations_.push("Unresponsive engines. Cause unknown.");
        situation_type_ = SituationType::ENGINE_FAILURE;
        return true;
    }

    return false;
}

bool SituationManager::IsSituation () {
    return situation_type_ != SituationType::NONE;
}

bool SituationManager::IsEngineFailure () {
    return situation_type_ == SituationType::ENGINE_FAILURE;
}

bool SituationManager::Update () {
    if ( situation_type_ == SituationType::AIR_FILTER_FAILURE &&
         !is_air_poisoned_ && air_response_time_ -
                              ( situation_counter_++ /
                                second_count_period_ ) <= 0 ) {
        logger_->debug("Air is poisoned");
        is_air_poisoned_ = true;
    }

    if ( AttemptFix()) {
        return true;
    }

    if ( WaitForHelp()) {
        return true;
    }

    return false;
}

void SituationManager::StartWaitingForHelp () {
    if ( !enough_spares_ ) {
        waiting_for_help_ = true;
    }
}

void SituationManager::StartFixingMinor () {
    fixing_minor_ = true;

    pause_menu_->ClearLastNotification();

    logger_->debug("Trying to fix minor issue");

}

void SituationManager::StartImmediateFixing () {
    fixing_ = true;
}

void SituationManager::IgnoreMinorFailure () {
    if ( !situations_.empty()) {
        situations_.pop();
    }
    ++ignored_minor_mech_failures_;
    situation_type_ = SituationType::NONE;
}

bool SituationManager::IsAirPoisoned () const {
    return is_air_poisoned_;
}

bool SituationManager::ProcessInput ( int c ) {
    switch ( c ) {
        case 32:
            pause_menu_->PushNotification("Situation in Progress");
            break;
        case '1': {
            switch ( situation_type_ ) {
                case SituationType::MINOR:
                    IgnoreMinorFailure();
                    return true;
                case SituationType::ENGINE_FAILURE:
                case SituationType::AIR_FILTER_FAILURE:
                    if ( !UseGenericSpareParts()) { break; }
                    StartImmediateFixing();
                    break;
                default:
                    break;
            }
            break;
        }
        case '2': {
            switch ( situation_type_ ) {
                case SituationType::MINOR:
                    StartImmediateFixing();
                    break;
                default:
                    break;
            }
            break;
        }
        case 'y': {
            StartWaitingForHelp();
            break;
        }
        default:
            break;
    }

    return false;
}

bool SituationManager::UseGenericSpareParts () {
    if ( !spaceship_->UseSpareParts(req_cabling_,
                                    req_components_)) {
        pause_menu_->PushNotification("Not enough spare parts");
        enough_spares_ = false;
        return false;
    } else {
        logger_->debug("Enough spare parts");
        enough_spares_ = true;
        return true;
    }
}

bool SituationManager::CanFixMinorIgnoredIssue () {
    if ( ignored_minor_mech_failures_ > 0 &&
         !fixing_minor_ && !UseGenericSpareParts()) {
        return false;
    }

    StartFixingMinor();
    return true;
}

}
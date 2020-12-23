//
// Created by Roberts Kalnins on 22/12/2020.
//

#pragma once

#include <vector>
#include <map>
#include <queue>
#include <ncurses.h>

#include "../../logging/logging.h"
#include "situation_type.h"
#include "situation.h"
#include "major_situation.h"
#include "../pause_menu.h"


namespace sc::play {

class SituationManager {
  public:

    SituationManager ( WINDOW *main,
                       std::shared_ptr< Spaceship > spaceship,
                       std::shared_ptr< PauseMenu > pause_menu );

    void ShowSituationReport ();

    bool CheckNewSituation ();

    [[nodiscard]] SituationType GetSituationType () const;

    [[nodiscard]] int GetIgnoredFailures () const;

    bool SuddenEngineFailure ();

    bool IsSituation ();

    bool IsEngineFailure ();

    [[nodiscard]] bool IsAirPoisoned () const;

    bool Update ();

    bool ProcessInput ( int c );

    bool CanFixMinorIgnoredIssue ();

    void FixMinorIgnored ();

  private:

    bool FixMinor ();

    bool UseGenericSpareParts ();

    void IgnoreMinorFailure ();

    void StartImmediateFixing ();

    bool AttemptFix ();

    bool WaitForHelp ();

    void StartFixingMinor ();

    void StartWaitingForHelp ();

  private:

    logger_t logger_;

    std::shared_ptr< PauseMenu > pause_menu_ { nullptr };

    std::shared_ptr< Spaceship > spaceship_ { nullptr };


    std::map< SituationType, std::vector< std::string>> sitrep_options_ {
            { SituationType::MINOR,              { "1. Ignore", "2. Attempt fix" }},
            { SituationType::ENGINE_FAILURE,     { "1. Attempt fix" }},
            { SituationType::AIR_FILTER_FAILURE, { "1. Attempt fix" }}
    };

    std::queue< const std::string > situations_;

    std::vector< const std::string > minor_failures_ = {
            "Toilet control shorting out",
            "Bridge door control browning out",
            "Loose cabling below deck", "Minor coolant leak",
            "Dirty air filter", "Exception thrown by navigation software",
            "Flickering cargo hold power", "High amp draw by HVAC system",
            "Unstable voltage in emergency breach containment system"
    };

    std::vector< const std::string > engine_failures_ = {
            "Engine coolant system failed. Cannot fire engines.",
            "Engine fuel leak. Cannot fire engines.",
            "Engine control system failed. Cannot fire engines.",
    };

    std::vector< const std::string > air_filter_failure_ = {
            "Air filter pump failed. Cannot recycle Oxygen.",
            "Air filter clogged. Cannot recycle Oxygen",
            "Air filter unresponsive. Cannot recycle Oxygen.",
    };

    const std::vector< int > components_used_minor_ = { 1, 2 };
    const std::vector< int > cabling_used_minor_    = { 1, 2 };
    const std::vector< int > components_used_major_ = { 2, 3, 5 };
    const std::vector< int > cabling_used_major_    = { 2, 3, 5 };

    std::unique_ptr< Situation > situation_ { nullptr };

    WINDOW *main_;

    SituationType situation_type_ { SituationType::NONE };

    const double minor_failure_prob_ { 0.001 };
    double       major_failure_prob_ { 0.0002 };
    const double unresponsive_engines_ { 0.0001 };

    const double successful_distress_ { 0.008 };
    bool         waiting_for_help_ { false };

    int    ignored_minor_mech_failures_ { 0 };
    double minor_fix_prob_ { 0.01 };
    double major_fix_prob_ { 0.003 };

    bool enough_spares_ { true };

    int req_components_ { 0 };
    int req_cabling_ { 0 };

    bool fixing_ { false };
    bool fixing_minor_ { false };

    const int second_count_period_ { 60 };
    int       situation_counter_ { 0 };

    const int air_response_time_ = 8;

    bool is_air_poisoned_ { false };

    const int pause_y_ { 32 };
    const int pause_x_ { 30 };
};

}





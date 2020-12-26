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
#include "../ui/pause_menu.h"
#include "situation_factory.h"
#include "minor_situation.h"


namespace sc::play {

class SituationManager {
  public:

    SituationManager ( WINDOW *main,
                       const shared_spaceship_t &spaceship,
                       const std::shared_ptr< PauseMenu > &pause_menu );

    bool CheckNewSituation ();

    [[nodiscard]] SituationType GetSituationType () const;

    [[nodiscard]] int GetIgnoredFailures () const;

    bool SuddenEngineFailure ();

    bool IsSituation ();

    bool IsEngineFailure ();

    bool UpdateSituation ();

    bool ProcessInput ( int c );

    bool CanFixMinorIgnoredIssue ();

    void FixMinorIgnored ();

    void UpdateHealth ();

  private:

    void ShowSituationReport ();

  private:

    logger_t logger_;

    std::shared_ptr< PauseMenu > pause_menu_ { nullptr };
    shared_spaceship_t           spaceship_ { nullptr };

    SituationFactory situation_factory_;

    std::map< SituationType, std::vector< std::string>> sitrep_options_ {
            { SituationType::MINOR,              { "1. Ignore", "2. Attempt fix" }},
            { SituationType::ENGINE_FAILURE,     { "1. Attempt fix" }},
            { SituationType::AIR_FILTER_FAILURE, { "1. Attempt fix" }}
    };

    std::queue< std::shared_ptr< Situation > > situations_;

    std::queue< std::shared_ptr< Situation >> ignored_minor_issues_;

    WINDOW *main_;

    const int pause_y_ { 32 };
    const int pause_x_ { 30 };
};

}





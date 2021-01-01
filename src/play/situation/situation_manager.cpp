//
// Created by Roberts Kalnins on 22/12/2020.
//

#include "situation_manager.h"

#include <sstream>
#include <effolkronium/random.hpp>

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

    std::stringstream disp;

    for ( auto &t : situations_.front()->GetMenuTasks()) {
        disp << t.id << ". " << t.message;
        mvwaddstr(main_, y++, x - 12, disp.str().c_str());
        disp.str("");
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

    if ( c == 32 ) {
        pause_menu_->PushNotification("Situation in Progress");
        return false;
    }

    menu_tasks_t &menu_tasks = situations_.front()->GetMenuTasks();

    int index = c - '1';

    if ( 0 <= index && index < menu_tasks.size()) {

        if ( menu_tasks[index].task ) {
            logger_->debug(
                    "Executing task {} at tasks index {} and menu index {}",
                    menu_tasks[index].current_action, index,
                    menu_tasks[index].id);
            menu_tasks[index].task();

            current_action_ = menu_tasks[index].current_action;
            situations_.front()->UseMenuOption(menu_tasks[index].id);
        } else {
            logger_->debug("Ignoring minor");

            ignored_minor_issues_.push(situations_.front());

            if ( !situations_.empty()) {
                situations_.pop();
            }
            return true;
        }

    }

    if ( c == 'y' && situations_.front()->PromptForHelp()) {
        situations_.front()->StartWaitForHelp();
        current_action_ = "Waiting for help";
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
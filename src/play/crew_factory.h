//
// Created by Roberts Kalnins on 18/12/2020.
//

#pragma once

#include <ncurses.h>
#include <string>
#include <map>

#include "../logging/logging.h"
#include "crew_member.h"


namespace sc::play {

class CrewMemberFactory {
  public:

    CrewMemberFactory ();

    CrewMember Create ();

    /**
     *
     * People are drawn in a box of height 5 and width 3
     * @param x Upper left x coordinate of person bounding box
     * @param y Upper left y coordinate of person bounding box
     * @param code Appearance code
     */
    static void PrintCharacter ( WINDOW *window, int y, int x,
                                 const std::string &code );

  protected:

    static char GetFaceChar ( char c );

  protected:
    std::map< std::string, bool > names_ = {
            { "Dex",      false },
            { "Maia",     false },
            { "Margo",    false },
            { "Tejal",    false },
            { "Kierstan", false },
            { "Annica",   false },
            { "Roberts",  false },
            { "Max",      false }};


    std::map< std::string, bool > skills_ = {
            { "Fix",        false },
            { "Fight",      false },
            { "Speed",      false },
            { "Stomach",    false },
            { "Plan",       false },
            { "Creativity", false }};

    int max_health_ { 10 };

    int min_health_ { 5 };
    int min_skill_count_ { 2 };
    int min_skill_range_ { 1 };

    int max_skill_range_ { 10 };
    int skill_count_ { 3 };

  private:
    logger_t logger_;

};

}


/*

\|/ \\   // |||  ___
(@) [%] {#} <$> |&|
-|- /^\ —|_ —[\ /!\
 |   |   |   ]   |
/ \  |\ /|

*/



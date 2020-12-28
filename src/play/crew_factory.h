//
// Created by Roberts Kalnins on 18/12/2020.
//

#pragma once

#include <ncurses.h>
#include <string>
#include <map>

#include "../logging/logging.h"
#include "crew_member.h"
#include "../config/config.h"
#include "../config/item_source.h"


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
    std::map< std::string, bool > names_;
    std::map< std::string, bool > skills_;

    Range health_range_;
    Range skill_count_range_;
    Range skill_strength_range_;

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



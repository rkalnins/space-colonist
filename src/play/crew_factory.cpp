//
// Created by Roberts Kalnins on 19/12/2020.
//


#include "crew_factory.h"

#include <string>
#include <effolkronium/random.hpp>

#include "crew_member.h"


using Random = effolkronium::random_static;

namespace sc::play {

CrewMemberFactory::CrewMemberFactory () : logger_(
        CreateLogger("crew_factory")) {
}


CrewMember CrewMemberFactory::Create () {
    auto name = names_.begin();

    do {
        name = Random::get(names_);
    } while ( name->second );

    name->second = true;

    int  health         = Random::get(min_health_, max_health_);
    auto possible_skill = skills_.begin();

    std::map< std::string, int > skills;

    int skill_count = Random::get(min_skill_count_, skill_count_);

    for ( int i = 0; i < skill_count; ++i ) {
        do {
            possible_skill = Random::get(skills_);
        } while ( possible_skill->second );

        possible_skill->second = true;
        int skill_strength = Random::get(min_skill_range_,
                                         max_skill_range_);

        skills[possible_skill->first] = skill_strength;
    }

    for ( auto &s : skills_ ) {
        s.second = false;
    }

    logger_->debug("Created crew {}", name->first);

    return CrewMember(name->first, health, skills);

}

void CrewMemberFactory::PrintCharacter ( WINDOW *window, int y, int x,
                                         const std::string &code ) {
    char hair = code[0];
    char head = code[1];
    char face = code[2];
    char body = code[3];
    char legs = code[4];

    switch ( hair ) {
        case 'a': {
            mvwaddstr(window, y++, x, "\\|/");
            break;
        }
        case 'b': {
            mvwaddstr(window, y++, x, "\\\\ ");
            break;
        }
        case 'c': {
            mvwaddstr(window, y++, x, " //");
            break;
        }
        case 'd': {
            mvwaddstr(window, y++, x, "|||");
            break;
        }
        case 'e': {
            mvwaddstr(window, y++, x, "___");
            break;
        }
        default:
            break;
    }

    std::string h;
    switch ( head ) {
        case 'a': {
            h.append(1, '(').append(1, GetFaceChar(face)).append(1, ')');
            mvwaddstr(window, y++, x, h.c_str());
            break;
        }
        case 'b': {
            h.append(1, '[').append(1, GetFaceChar(face)).append(1, ']');
            mvwaddstr(window, y++, x, h.c_str());
            break;
        }
        case 'c': {
            h.append(1, '{').append(1, GetFaceChar(face)).append(1, '}');
            mvwaddstr(window, y++, x, h.c_str());
            break;
        }
        case 'd': {
            h.append(1, '<').append(1, GetFaceChar(face)).append(1, '>');
            mvwaddstr(window, y++, x, h.c_str());
            break;
        }
        case 'e': {
            h.append(1, '|').append(1, GetFaceChar(face)).append(1, '|');
            mvwaddstr(window, y++, x, h.c_str());
            break;
        }
        default:
            break;
    }

    switch ( body ) {
        case 'a': {
            mvwaddstr(window, y++, x, "-|-");
            mvwaddstr(window, y++, x, " | ");
            break;
        }
        case 'b': {
            mvwaddstr(window, y++, x, "/^\\");
            mvwaddstr(window, y++, x, " | ");
            break;
        }
        case 'c': {
            mvwaddstr(window, y++, x, "-|_");
            mvwaddstr(window, y++, x, " | ");
            break;
        }
        case 'd': {
            mvwaddstr(window, y++, x, "-[\\");
            mvwaddstr(window, y++, x, " ] ");
            break;
        }
        case 'e': {
            mvwaddstr(window, y++, x, "/!\\");
            mvwaddstr(window, y++, x, " | ");
            break;
        }
        default:
            break;
    }

    switch ( legs ) {
        case 'a': {
            mvwaddstr(window, y++, x, "/ \\");
            break;
        }
        case 'd':
        case 'b': {
            mvwaddstr(window, y++, x, " |\\");
            break;
        }
        case 'e':
        case 'c': {
            mvwaddstr(window, y++, x, "/| ");
            break;
        }
        default:
            break;
    }

}

char CrewMemberFactory::GetFaceChar ( char c ) {
    switch ( c ) {
        case 'a':
            return '@';
        case 'b':
            return '%';
        case 'c':
            return '#';
        case 'd':
            return '$';
        case 'e':
            return '&';
        default:
            return ' ';
    }
}

}
//
// Created by Roberts Kalnins on 16/12/2020.
//

#include "crew_member.h"

#include <utility>
#include <effolkronium/random.hpp>


using Random = effolkronium::random_static;

namespace sc::play {

CrewMember::CrewMember ( std::string name, int max_health,
                         std::map< std::string, int > skills )
        : name_(std::move(name)), health_(max_health),
          max_health_(max_health),
          skills_(std::move(skills)) {

    for ( int i = 0; i < 5; ++i ) {
        appearance_code_ += Random::get('a', 'c');
    }

}

const std::string &CrewMember::GetName () const {
    return name_;
}

int sc::play::CrewMember::GetHealth () const {
    return health_;
}

const std::map< std::string, int > &CrewMember::GetSkills () const {
    return skills_;
}

int CrewMember::GetMaxHealth () const {
    return max_health_;
}

const std::string &CrewMember::GetCode () {
    return appearance_code_;
}

void CrewMember::UpdateHealth ( int change ) {
    if ( health_ + change > max_health_ ) {
        health_ = max_health_;
        return;
    }

    if ( health_ + change < 0 ) {
        health_ = 0;
    }

    health_ += change;
}

bool CrewMember::IsDead () const {
    return health_ <= 0;
}

}

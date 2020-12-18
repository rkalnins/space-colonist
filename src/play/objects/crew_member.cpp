//
// Created by Roberts Kalnins on 16/12/2020.
//

#include "crew_member.h"

#include <utility>


sc::play::CrewMember::CrewMember ( std::string name, int health,
                                   int max_health,
                                   std::map< std::string, int > skills )
        : name_(std::move(name)), health_(health), max_health_(max_health),
          skills_(std::move(skills)) {}

const std::string &sc::play::CrewMember::GetName () const {
    return name_;
}

int sc::play::CrewMember::GetHealth () const {
    return health_;
}

const std::map< std::string, int > &
sc::play::CrewMember::GetSkills () const {
    return skills_;
}

int sc::play::CrewMember::GetMaxHealth () const {
    return max_health_;
}

//
// Created by Roberts Kalnins on 16/12/2020.
//

#include "spaceship.h"


namespace sc::play {

void
Spaceship::AddCrewMember ( const CrewMember &crew_member ) {
    crew_.push_back(crew_member);
}

void Spaceship::AddItem ( std::string &category,
                          Item &item ) {
    items_[category].push_back(item);

}

int Spaceship::GetHull () const {
    return hull_;
}

double Spaceship::GetFuel () const {
    return fuel_;
}

void Spaceship::UseFuel ( double usage ) {
    if ( state_ == SpaceshipState::MOVING ) {
        fuel_ -= usage;
    }
}

const std::vector< CrewMember > &
Spaceship::GetCrew () const {
    return crew_;
}

void Spaceship::ResetFuel () {
    fuel_ = full_fuel_;
}

double Spaceship::GetFullFuel () const {
    return full_fuel_;
}

}
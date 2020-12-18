//
// Created by Roberts Kalnins on 16/12/2020.
//

#include "spaceship.h"


namespace sc::play {

void
Spaceship::AddCrewMember ( const CrewMember &crew_member ) {
    crew_.push_back(crew_member);
}

void Spaceship::AddItem ( Item &item ) {
    items_[item.GetCategory()].push_back(item);

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

const std::map< std::string, std::vector< Item>> &
Spaceship::GetItems () const {
    return items_;
}

}
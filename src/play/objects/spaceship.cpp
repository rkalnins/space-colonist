//
// Created by Roberts Kalnins on 16/12/2020.
//

#include "spaceship.h"


namespace sc::play {

void
Spaceship::AddCrewMember ( const CrewMember &crew_member ) {
    crew_.insert(crew_member);
}

void Spaceship::AddItem ( Item &item ) {
    weight_ += item.GetWeight();
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

const std::set< CrewMember > &
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

int Spaceship::GetWeight () const {
    return weight_;
}

void Spaceship::SetWeight ( int weight ) {
    weight_ = weight;
}

void Spaceship::UpdateWeight ( int change ) {
    weight_ += change;
}

int Spaceship::GetMaxWeight () const {
    return max_weight;
}

void Spaceship::SetMaxWeight ( int maxWeight ) {
    max_weight = maxWeight;
}

int Spaceship::GetMoney () const {
    return money_;
}

void Spaceship::UpdateMoney ( int change ) {
    money_ += change;
}

void Spaceship::SetMoney ( int money ) {
    money_ = money;
}

}
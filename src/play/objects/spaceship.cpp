//
// Created by Roberts Kalnins on 16/12/2020.
//

#include "spaceship.h"
#include "../items/item.h"

namespace sc::play {

void
Spaceship::AddCrewMember ( const CrewMember &crew_member ) {
    crew_.insert(crew_member);
}

void Spaceship::AddItem ( Item &item ) {
    weight_ += item.GetWeight();

    std::vector<Item> &items = items_[item.GetCategory()];

    Item::NameComparator cmp(item.GetName());

    auto item_it = std::find_if(items.begin(), items.end(), cmp);

    if (item_it != items.end()) {
        item_it->UpdateValue(item.GetValue());
        item_it->UpdateWeight(item.GetWeight());
    } else {
        items.push_back(item);
    }
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
    return max_weight_;
}

void Spaceship::SetMaxWeight ( int maxWeight ) {
    max_weight_ = maxWeight;
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

Spaceship::Spaceship () {
    money_      = 1000;
    weight_     = 0;
    max_weight_ = 0;
    fuel_       = 0;
    full_fuel_  = 0;
    hull_       = 0;
    full_hull_  = 0;
    max_crew_   = 0;
}

}
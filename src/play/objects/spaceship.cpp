//
// Created by Roberts Kalnins on 16/12/2020.
//

#include "spaceship.h"

#include <effolkronium/random.hpp>

#include "../items/item.h"


namespace sc::play {

Spaceship::Spaceship ( std::string &appearance_code ) {
    appearance_code_ = appearance_code;
}

void
Spaceship::AddCrewMember ( const CrewMember &crew_member ) {
    crew_.insert(crew_member);
}

bool Spaceship::AddItem ( Item &item ) {
    if ( weight_ + item.GetWeight() > max_weight_ ||
         money_ - item.GetCost() < 0 ) {
        return false;
    }

    weight_ += item.GetWeight();
    money_ -= item.GetCost();

    std::vector< Item > &items = items_[item.GetCategory()];

    Item::NameComparator cmp(item.GetName());

    auto item_it = std::find_if(items.begin(), items.end(), cmp);

    if ( item_it != items.end()) {
        item_it->UpdateWeight(item.GetWeight());
        item_it->UpdateValue(item.GetValue());
    } else {
        items.push_back(item);
    }

    return true;
}

bool Spaceship::RemoveItem ( Item &item ) {

    std::vector< Item > &items = items_[item.GetCategory()];

    Item::NameComparator cmp(item.GetName());

    auto item_it = std::find_if(items.begin(), items.end(), cmp);

    if ( item_it != items.end()) {
        if ( item_it->UpdateValue(-1)) {
            weight_ -= item.GetWeight();
            money_ += item.GetCost();

            if ( item_it->GetValue() == 0 ) {
                items.erase(item_it);
            }

            return true;
        }
    }

    return false;
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

std::map< std::string, std::vector< Item>> &Spaceship::GetItems () {
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

void Spaceship::SetMaxWeight ( int max_weight ) {
    max_weight_ = max_weight;
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

const std::string &Spaceship::GetAppearanceCode () const {
    return appearance_code_;
}

void Spaceship::SetFullHull ( int hull ) {
    full_hull_ = hull;
    hull_ = hull;
}

void Spaceship::SetFullFuel ( double fuel ) {
    full_fuel_ = fuel;
}

void Spaceship::SetMaxCrew ( int max_crew ) {
    max_crew_ = max_crew;
}

int Spaceship::GetCost () const {
    return cost_;
}

void Spaceship::SetCost ( int cost ) {
    cost_ = cost;
}

int Spaceship::GetFullHull () const {
    return full_hull_;
}

int Spaceship::GetMaxCrew () const {
    return max_crew_;
}

}
//
// Created by Roberts Kalnins on 16/12/2020.
//

#include "spaceship.h"

#include <effolkronium/random.hpp>
#include <algorithm>

#include "../items/item.h"


using Random = effolkronium::random_static;

namespace sc::play {

Spaceship::Spaceship ( std::string &appearance_code ) {
    appearance_code_ = appearance_code;
}

void Spaceship::LoggingInit () {
    logger_ = spdlog::basic_logger_mt("ss", "logs/space-colonist-log.log");
    logger_->set_level(spdlog::level::debug);
}

void
Spaceship::AddCrewMember ( const CrewMember &crew_member ) {
    crew_.push_back(crew_member);
}

void Spaceship::RemoveCrewMember ( const CrewMember &crew_member ) {
    CrewMember::NameComparator cmp(crew_member.GetName());

    auto crew_it = std::find_if(crew_.begin(),
                                crew_.end(), cmp);

    if ( crew_it != crew_.end()) {
        crew_.erase(crew_it);
        logger_->debug("Removed {}", crew_member.GetName());
    } else {
        logger_->debug("Unable to remove {}", crew_member.GetName());
    }
}

bool Spaceship::AddItem ( Item &item ) {

    logger_->debug("Adding {}", item.GetName());

    if ( money_ - item.GetCost() < 0 ) { return false; }

    if ( item.GetCategory() == "Fuel" ) {
        if ( item.GetValue() + fuel_ > full_fuel_ ) {
            return false;
        }

        if ( !UpdateWeight(item.GetWeight())) {
            logger_->debug("Unable to add item because of weight");
            return false;
        }

        money_ -= item.GetCost();
        fuel_ += item.GetValue();
        return true;
    }

    if ( !UpdateWeight(item.GetWeight())) {
        logger_->debug("Unable to add item because of weight");
        return false;
    }


    money_ -= item.GetCost();

    if ( item.GetCategory() == "Food" ) {
        food_ += item.GetValue();
    }

    std::vector< Item > &items = items_[item.GetCategory()];

    Item::NameComparator cmp(item.GetName());

    auto item_it = std::find_if(items.begin(), items.end(), cmp);

    if ( item_it != items.end()) {
        item_it->UpdateValue(item.GetValue());
    } else {
        items.push_back(item);
    }

    return true;
}

bool Spaceship::RemoveItem ( Item &item ) {

    logger_->debug("Removing {}", item.GetName());

    if ( item.GetCategory() == "Fuel" ) {

        if ( fuel_ - item.GetValue() < 0 ) {
            return false;
        }

        UpdateWeight(-1 * item.GetWeight());
        money_ += item.GetCost();
        fuel_ -= item.GetValue();
        return true;
    }

    std::vector< Item > &items = items_[item.GetCategory()];

    Item::NameComparator cmp(item.GetName());

    auto item_it = std::find_if(items.begin(), items.end(), cmp);

    if ( item_it != items.end()) {
        if ( item_it->UpdateValue(-1)) {
            UpdateWeight(-1 * item.GetWeight());
            money_ += item.GetCost();

            if ( item.GetCategory() == "Food" ) {
                food_ -= item.GetValue();
            }

            if ( item_it->GetValue() == 0 ) {
                items.erase(item_it);
            }

            return true;
        }
    }

    return false;
}

void Spaceship::UseFood () {
    if ( food_ <= 0 ) {
        food_ = 0;
        return;
    }

    food_ = std::max(food_ - static_cast<int>(crew_.size()), 0);

    auto food = Random::get(items_["Food"]);

    // TODO bug when crew size > remaining value

    if ( food->UpdateValue(-1 * static_cast<int>(crew_.size()))) {
        UpdateWeight(
                -1 * static_cast<int>(crew_.size()) * food->GetWeight());
        if ( food->GetValue() <= 0 ) {
            items_["Food"].erase(food);
        }
    }

}


int Spaceship::GetHull () const {
    return hull_;
}

double Spaceship::GetFuel () const {
    return fuel_;
}

void Spaceship::UseFuel ( double usage ) {
    if ( fuel_ - usage > 0 ) {

        fuel_ -= usage;
        UpdateWeight(-10 * usage); // FIXME 10 is weight of fuel
    }
}

std::vector< CrewMember > &Spaceship::GetCrew () {
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

double Spaceship::GetWeight () const {
    return weight_;
}

bool Spaceship::UpdateWeight ( double change ) {
    if (( change < 0 && weight_ + change < 0 ) ||
        ( change > 0 && weight_ + change > max_weight_ )) {
        logger_->debug("unable to change weight");
        return false;
    }
    weight_ += change;
    return true;
}

double Spaceship::GetMaxWeight () const {
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
    hull_      = hull;
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

int Spaceship::GetFood () const {
    return food_;
}

void Spaceship::SetFood ( int food ) {
    food_ = food;
}

void Spaceship::RemoveDeadCrew () {
    crew_.erase(
            std::remove_if(crew_.begin(), crew_.end(), RemoveDeadComp()),
            crew_.end());
}

}
//
// Created by Roberts Kalnins on 16/12/2020.
//

#include "spaceship.h"

#include <effolkronium/random.hpp>
#include <algorithm>

#include "../config/config.h"
#include "item.h"


using Random = effolkronium::random_static;

namespace sc::play {

Spaceship::Spaceship ( std::string &appearance_code ) {
    appearance_code_ = appearance_code;

    Config &config = Config::GetInstance();

    workshop_spare_part_scale_ = config.GetValue(
            "play.spaceship.workshop-scale", 0.0);
}

void Spaceship::LoggingInit () {
    logger_ = CreateLogger("ss");
}

void Spaceship::SpaceshipInitConfig ( double max_weight, double full_fuel,
                                      int full_hull, int max_crew,
                                      int init_money, int cost ) {
    max_weight_ = max_weight;
    full_fuel_  = full_fuel;
    full_hull_  = full_hull;
    hull_       = full_hull;
    max_crew_   = max_crew;
    money_      = init_money;
    cost_       = cost;

}

void
Spaceship::AddCrewMember ( const CrewMember &crew_member ) {
    crew_.push_back(crew_member);
}

bool Spaceship::CanAddItem ( Item &item ) {
    if ( money_ - ( item.GetCost() * item.GetQuantity()) < 0 ) {
        logger_->debug("Not enough money to purchase {} for {}",
                       item.GetName(), item.GetCost());

        return false;
    }

    if ( weight_ + ( item.GetQuantity() * item.GetWeight()) >
         max_weight_ ) {
        logger_->debug("Unable to add item because of weight");
        return false;
    }

    return true;
}

bool Spaceship::AddFuel ( Item &item ) {
    if ( item.GetQuantity() + fuel_ > full_fuel_ ) {
        logger_->debug("Not enough fuel storage to add fuel");
        return false;
    }

    fuel_ += item.GetQuantity();
    money_ -= item.GetCost() * item.GetQuantity();
    weight_ += item.GetWeight() * item.GetQuantity();

    return true;
}

bool Spaceship::AddItem ( Item &item ) {

    logger_->debug("Adding {} at {} cost and {} weight", item.GetName(),
                   item.GetCost(), item.GetWeight());

    if ( !CanAddItem(item)) { return false; }

    if ( item.GetCategory() == "Fuel" ) {
        return AddFuel(item);
    }

    if ( item.GetCategory() == "Food" ) {
        food_ += item.GetQuantity();
    }

    std::vector< Item > &items = items_[item.GetCategory()];

    Item::NameComparator cmp(item.GetName());

    auto item_it = std::find_if(items.begin(), items.end(), cmp);

    if ( item_it != items.end()) {
        item_it->HardUpdateQuantity(item.GetQuantity());
    } else {
        items.push_back(item);
    }

    money_ -= item.GetCost() * item.GetQuantity();
    weight_ += item.GetWeight() * item.GetQuantity();

    return true;
}

bool Spaceship::RemoveFuel ( Item &item ) {
    if ( fuel_ - item.GetQuantity() < 0 ) {
        return false;
    }

    UpdateWeight(-1 * item.GetQuantity() * item.GetWeight());
    money_ += item.GetCost() * item.GetQuantity();
    fuel_ -= item.GetQuantity();

    return true;
}


bool Spaceship::RemoveItem ( Item &item ) {

    logger_->debug("Removing {}", item.GetName());

    if ( item.GetCategory() == "Fuel" ) {
        return RemoveFuel(item);
    }

    std::vector< Item > &items = items_[item.GetCategory()];

    Item::NameComparator cmp(item.GetName());

    auto item_it = std::find_if(items.begin(), items.end(), cmp);

    if ( item_it != items.end()) {
        if ( item_it->SoftUpdateQuantity(-1 * item.GetQuantity())) {
            UpdateWeight(-1 * item.GetQuantity() * item.GetWeight());
            money_ += item.GetCost() * item.GetQuantity();

            if ( item.GetCategory() == "Food" ) {
                food_ -= item.GetQuantity();
            }

            if ( item_it->GetQuantity() == 0 ) {
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

    if ( food->HardUpdateQuantity(-1 * static_cast<int>(crew_.size()))) {
        UpdateWeight(
                -1 * static_cast<int>(crew_.size()) * food->GetWeight());
        if ( food->GetQuantity() <= 0 ) {
            items_["Food"].erase(food);
        }
    }

}

void Spaceship::RemoveDeadCrew () {
    crew_.erase(
            std::remove_if(crew_.begin(), crew_.end(), RemoveDeadComp()),
            crew_.end());
}

void Spaceship::UseFuel ( double usage ) {
    if ( fuel_ - usage > 0 ) {

        fuel_ -= usage;
        UpdateWeight(-10 * usage); // FIXME 10 is weight of fuel
    } else {
        UpdateWeight(-10 * ( fuel_ ));
        fuel_ = 0;
    }
}

bool Spaceship::UpdateWeight ( double change ) {
    if (( change < 0 && weight_ + change < 0 ) ||
        ( change > 0 && weight_ + change > max_weight_ )) {
        logger_->debug("Unable to change weight");
        return false;
    }
    weight_ += change;
    return true;
}

bool Spaceship::UseSpareParts ( int cables, int components ) {
    auto spares = items_.find("Spare parts");

    if ( spares == items_.end()) {
        logger_->debug("No spares available");
        return false;
    }

    auto &spares_v = spares->second;

    if ( HasWorkshop()) {
        cables     = (int) (static_cast<double>(cables) * workshop_spare_part_scale_);
        components = (int) (static_cast<double>(components) * workshop_spare_part_scale_);
        logger_->debug(
                "Reducing spare parts cost to {} cables and {} components",
                cables, components);
    }

    Item::NameComparator cables_cmp("Cables");
    Item::NameComparator comp_cmp("Electronic components");

    auto cables_it = std::find_if(spares_v.begin(), spares_v.end(),
                                  cables_cmp);
    auto comp_it   = std::find_if(spares_v.begin(), spares_v.end(),
                                  comp_cmp);

    if ( cables_it == spares_v.end() || comp_it == spares_v.end()) {
        logger_->debug("Required spares not available");
        return false;
    }

    if ( cables_it->GetQuantity() < cables ||
         comp_it->GetQuantity() < components ) {
        logger_->debug("Not enough spare parts");
        return false;
    }


    cables_it->HardUpdateQuantity(-1 * cables);
    comp_it->HardUpdateQuantity(-1 * components);

    logger_->debug("Used spare parts");

    return true;

}

int Spaceship::GetHull () const { return hull_; }

double Spaceship::GetFuel () const { return fuel_; }

std::vector< CrewMember > &Spaceship::GetCrew () { return crew_; }

double Spaceship::GetFullFuel () const { return full_fuel_; }

std::map< std::string, std::vector< Item>> &Spaceship::GetItems () {
    return items_;
}

double Spaceship::GetWeight () const { return weight_; }

double Spaceship::GetMaxWeight () const { return max_weight_; }

int Spaceship::GetMoney () const { return money_; }

const std::string &Spaceship::GetAppearanceCode () const {
    return appearance_code_;
}

bool Spaceship::HasWorkshop () {
    if ( items_.find("Infrastructure") == items_.end()) { return false; }

    Item::NameComparator cmp("Workshop");

    auto workshop_it = std::find_if(items_["Infrastructure"].begin(),
                                    items_["Infrastructure"].end(), cmp);

    return workshop_it != items_["Infrastructure"].end();
}

double Spaceship::GetToolsetMultiplier () {

    auto tools = items_.find("Tools");

    if (tools == items_.end()) { return 1.0; }

    Item::NameComparator basic("Basic toolset");
    Item::NameComparator standard("Standard toolset");
    Item::NameComparator enhanced("Enhanced toolset");
    Item::NameComparator full("Full toolset");

    static Config &config = Config::GetInstance();

    auto basic_it = std::find_if(tools->second.begin(), tools->second.end(), basic);
    auto std_it = std::find_if(tools->second.begin(), tools->second.end(), standard);
    auto enhanced_it = std::find_if(tools->second.begin(), tools->second.end(), enhanced);
    auto full_it = std::find_if(tools->second.begin(), tools->second.end(), full);

    if (basic_it != tools->second.end()) { return config.GetValue("play.spaceship.toolset-scale.basic", 1.0); }
    if (std_it != tools->second.end()) { return config.GetValue("play.spaceship.toolset-scale.basic", 1.0); }
    if (enhanced_it != tools->second.end()) { return config.GetValue("play.spaceship.toolset-scale.basic", 1.0); }
    if (full_it != tools->second.end()) { return config.GetValue("play.spaceship.toolset-scale.basic", 1.0); }

    return 0;
}

int Spaceship::GetCost () const { return cost_; }

int Spaceship::GetFullHull () const { return full_hull_; }

int Spaceship::GetMaxCrew () const { return max_crew_; }

int Spaceship::GetFood () const { return food_; }

bool Spaceship::HasFuel () const { return fuel_ > 0; }

bool Spaceship::HasFood () const { return food_ > 0; }

void Spaceship::Pause () { state_ = SpaceshipState::PAUSED; }

void Spaceship::Unpause () { state_ = SpaceshipState::OTHER; }

bool Spaceship::IsPaused () { return state_ == SpaceshipState::PAUSED; }

bool Spaceship::IsDeparting () const {
    return state_ == SpaceshipState::DEPARTING;
}

}
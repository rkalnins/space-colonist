//
// Created by Roberts Kalnins on 16/12/2020.
//

#include "item.h"

#include <utility>


namespace sc::play {

Item::Item ( std::string category, std::string name, int quantity, int weight,
             int cost )
        : category_(std::move(category)),
          name_(std::move(name)),
          quantity_(quantity), weight_(weight), cost_(cost) {}

const std::string &Item::GetCategory () const {
    return category_;
}

const std::string &Item::GetName () const {
    return name_;
}

int Item::GetQuantity () const {
    return quantity_;
}

double Item::GetWeight () const {
    return weight_;
}

bool Item::HardUpdateQuantity ( int quantity ) {
    quantity_ += quantity;

    if ( quantity_ < 0 ) {
        quantity_ = 0;
        return false;
    }

    return true;
}

bool Item::SoftUpdateQuantity ( int quantity ) {
    if ( quantity_ + quantity < 0 ) {
        return false;
    }

    quantity_ += quantity;
    return true;
}

int Item::PartialUpdateQuantity ( int quantity ) {
    quantity_ += quantity;

    if ( quantity_ < 0 ) {
        int tmp = quantity_;
        quantity_ = 0;
        return std::abs(tmp);
    }

    return 0;
}

int Item::GetCost () const {
    return cost_;
}

void Item::SetQuantity ( int quantity ) {
    quantity_ = quantity;
}


}
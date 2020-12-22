//
// Created by Roberts Kalnins on 16/12/2020.
//

#include "item.h"

#include <utility>


namespace sc::play {

Item::Item ( std::string category, std::string name, int value, int weight,
             int cost )
        : category_(std::move(category)),
          name_(std::move(name)),
          value_(value), weight_(weight), cost_(cost) {}

const std::string &Item::GetCategory () const {
    return category_;
}

const std::string &Item::GetName () const {
    return name_;
}

int Item::GetValue () const {
    return value_;
}

double Item::GetWeight () const {
    return weight_;
}

bool Item::UpdateValue ( int value ) {
    value_ += value;

    if ( value_ < 0 ) {
        value_ = 0;
        return false;
    } else {
        return true;
    }
}

void Item::UpdateWeight ( double weight ) {
    weight_ += weight;
}

int Item::GetCost () const {
    return cost_;
}

void Item::SetValue ( int value ) {
    value_ = value;
}


}
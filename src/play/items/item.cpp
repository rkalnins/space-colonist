//
// Created by Roberts Kalnins on 16/12/2020.
//

#include "item.h"

#include <utility>


namespace sc::play {

Item::Item ( std::string category, std::string name,
             int value ) : category_(std::move(category)),
                           name_(std::move(name)),
                           value_(value) {}

const std::string &Item::GetCategory () const {
    return category_;
}

const std::string &Item::GetName () const {
    return name_;
}

int Item::GetValue () const {
    return value_;
}
}
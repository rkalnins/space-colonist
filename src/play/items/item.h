//
// Created by Roberts Kalnins on 16/12/2020.
//

#pragma once

#include <string>


namespace sc::play {


class Item {
  public:

    Item ( std::string category, std::string name, int value, int weight );

    [[nodiscard]] const std::string &GetCategory () const;

    [[nodiscard]] const std::string &GetName () const;

    [[nodiscard]] int GetValue () const;

    [[nodiscard]] int GetWeight () const;

  private:
    std::string category_;
    std::string name_;
    int         value_;
    int         weight_;


};

}





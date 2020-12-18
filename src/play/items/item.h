//
// Created by Roberts Kalnins on 16/12/2020.
//

#pragma once

#include <string>


namespace sc::play {


class Item {
  public:

    Item ( std::string category, std::string name,
           int value );

    const std::string &GetCategory () const;

    const std::string &GetName () const;

    int GetValue () const;


  private:
    std::string category_;
    std::string name_;
    int         value_;


};

}





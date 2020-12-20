//
// Created by Roberts Kalnins on 16/12/2020.
//

#pragma once

#include <string>


namespace sc::play {

class Item {
  public:

    Item ( std::string category, std::string name, int value, int weight,
           int cost );

    [[nodiscard]] const std::string &GetCategory () const;

    [[nodiscard]] const std::string &GetName () const;

    [[nodiscard]] int GetValue () const;

    [[nodiscard]] int GetWeight () const;

    bool UpdateValue ( int value );

    void UpdateWeight ( int weight );

    [[nodiscard]] int GetCost () const;

    void SetValue ( int value );


    class NameComparator {
      public:
        explicit NameComparator ( const std::string &compTo ) : comp_to_(
                compTo) {}

        bool operator() ( const Item &rhs ) const {
            return rhs.name_ == comp_to_;
        }

      private:
        const std::string &comp_to_;
    };


  private:

    std::string category_;
    std::string name_;
    int         value_;
    int         weight_;
    int cost_;
};

}





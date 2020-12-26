//
// Created by Roberts Kalnins on 16/12/2020.
//

#pragma once

#include <string>


namespace sc::play {

class Item {
  public:

    Item ( std::string category, std::string name, int quantity,
           int weight,
           int cost );

    [[nodiscard]] const std::string &GetCategory () const;

    [[nodiscard]] const std::string &GetName () const;

    [[nodiscard]] double GetWeight () const;

    [[nodiscard]] int GetCost () const;

    [[nodiscard]] int GetQuantity () const;

    void SetQuantity ( int quantity );

    /**
     * Updates value, if subtracting and result is less than 0, new value is 0
     * @param quantity
     * @return
     */
    bool HardUpdateQuantity ( int quantity );

    /**
     * Updates value, if subtracting and result is less than 0 then abort
     * @param quantity
     * @return
     */
    bool SoftUpdateQuantity ( int quantity );


    /**
     * Updates value, if subtracting, updates as much as possible, returns value not used, if any
     * @param weight
     * @return
     */
    int PartialUpdateQuantity ( int quantity );


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

    double weight_;

    int cost_;
    int quantity_;
};

}





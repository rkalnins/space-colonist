//
// Created by Roberts Kalnins on 16/12/2020.
//

#pragma once

#include <set>
#include <map>
#include <vector>

#include "crew_member.h"
#include "../items/item.h"


namespace sc::play {

enum class SpaceshipState { MOVING, OTHER };


class Spaceship {
  public:

    Spaceship ();

    void AddCrewMember ( const CrewMember &crew_member );

    void AddItem ( Item &item );

    [[nodiscard]] int GetHull () const;

    [[nodiscard]] double GetFuel () const;

    void ResetFuel ();

    void UseFuel ( double usage );

    [[nodiscard]] const std::set< CrewMember > &
    GetCrew () const;

    [[nodiscard]] double GetFullFuel () const;

    [[nodiscard]] const std::map< std::string, std::vector< Item>> &
    GetItems () const;

    int GetWeight () const;

    void SetWeight ( int weight );

    void UpdateWeight ( int change );

    int GetMaxWeight () const;

    void SetMaxWeight ( int maxWeight );

    int GetMoney () const;

    void UpdateMoney ( int change );

    void SetMoney ( int money );

  private:


    std::map< std::string, std::vector< Item > > items_;

    std::set< CrewMember > crew_;

    int    full_hull_ { 100 };
    double full_fuel_ { 1000.0 };
    int    hull_ { 0 };
    double fuel_ { 0 };
    int    weight_ { 0 };
    int    max_weight_ { 0 };
    int    money_ { 0 };
    int    max_crew_ { 0 };

    SpaceshipState state_ { SpaceshipState::MOVING };

};

}



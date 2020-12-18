//
// Created by Roberts Kalnins on 16/12/2020.
//

#pragma once

#include <vector>
#include <map>

#include "crew_member.h"
#include "../items/item.h"


namespace sc::play {

enum class SpaceshipState { MOVING, OTHER };

class Spaceship {
  public:

    void AddCrewMember (  const CrewMember& crew_member );

    void AddItem ( Item &item );

    [[nodiscard]] int GetHull () const;

    [[nodiscard]] double GetFuel () const;

    void ResetFuel();

    void UseFuel ( double usage );

    [[nodiscard]] const std::vector< CrewMember> &
    GetCrew () const;

    [[nodiscard]] double GetFullFuel () const;

    const std::map< std::string, std::vector< Item>> &GetItems () const;

  private:


    std::map< std::string, std::vector<  Item > > items_;

    std::vector< CrewMember> crew_;

    int    full_hull_ { 100 };
    double full_fuel_ { 1000.0 };
    int    hull_ { 0 };
    double fuel_ { 0 };

    SpaceshipState state_ { SpaceshipState::MOVING };

};

}



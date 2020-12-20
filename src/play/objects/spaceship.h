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

    explicit Spaceship (std::string &appearance_code);

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

    [[nodiscard]] int GetWeight () const;

    void SetWeight ( int weight );

    void UpdateWeight ( int change );

    [[nodiscard]] int GetMaxWeight () const;

    void SetMaxWeight ( int maxWeight );

    [[nodiscard]] int GetMoney () const;

    void UpdateMoney ( int change );

    void SetMoney ( int money );

    void StartMoving () { state_ = SpaceshipState::MOVING; }

    void StopMoving () { state_ = SpaceshipState::OTHER; }

    [[nodiscard]] const std::string &GetAppearanceCode () const;

    void SetFullHull ( int fullHull );

    void SetFullFuel ( double fullFuel );

    void SetMaxCrew ( int maxCrew );

    [[nodiscard]] int GetCost () const;

    void SetCost ( int cost );

    [[nodiscard]] int GetFullHull () const;

    int GetMaxCrew () const;

  private:

    std::map< std::string, std::vector< Item > > items_;

    std::set< CrewMember > crew_;
    std::string            appearance_code_;

    int    full_hull_ { 0 };
    double full_fuel_ { 0 };
    int    hull_ { 0 };
    double fuel_ { 0 };
    int    weight_ { 0 };
    int    max_weight_ { 0 };
    int    money_ { 0 };
    int    max_crew_ { 0 };
    int    cost_ { 0 };


    SpaceshipState state_ { SpaceshipState::OTHER };

};

}



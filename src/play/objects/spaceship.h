//
// Created by Roberts Kalnins on 16/12/2020.
//

#pragma once

#include <set>
#include <map>
#include <vector>

#include "crew_member.h"
#include "../items/item.h"
#include "../../logging/logging.h"


namespace sc::play {

enum class SpaceshipState { MOVING, OTHER };


class Spaceship {
  public:

    explicit Spaceship ( std::string &appearance_code );

    void LoggingInit ();

    void AddCrewMember ( const CrewMember &crew_member );

    void RemoveCrewMember ( const CrewMember &crew_member );

    void RemoveDeadCrew();

    bool AddItem ( Item &item );

    bool RemoveItem ( Item &item );

    [[nodiscard]] int GetHull () const;

    [[nodiscard]] double GetFuel () const;

    void ResetFuel ();

    void UseFuel ( double usage );

    [[nodiscard]] std::vector< CrewMember > & GetCrew ();

    [[nodiscard]] double GetFullFuel () const;

    [[nodiscard]] std::map< std::string, std::vector< Item>> &
    GetItems ();

    [[nodiscard]] double GetWeight () const;

    bool UpdateWeight ( double change );

    [[nodiscard]] double GetMaxWeight () const;

    void SetMaxWeight ( int max_weight );

    [[nodiscard]] int GetMoney () const;

    void UpdateMoney ( int change );

    void SetMoney ( int money );

    void StartMoving () { state_ = SpaceshipState::MOVING; }

    void StopMoving () { state_ = SpaceshipState::OTHER; }

    [[nodiscard]] const std::string &GetAppearanceCode () const;

    void SetFullHull ( int hull );

    void SetFullFuel ( double fuel );

    void SetMaxCrew ( int max_crew );

    [[nodiscard]] int GetCost () const;

    void SetCost ( int cost );

    [[nodiscard]] int GetFullHull () const;

    [[nodiscard]] int GetMaxCrew () const;

    void UseFood ();

    [[nodiscard]] int GetFood () const;

    void SetFood ( int food );

  private:

    class RemoveDeadComp {
      public:
        bool operator() (CrewMember &c) const {
            return c.IsDead();
        }
    };

  private:

    std::map< std::string, std::vector< Item > > items_;

    std::vector< CrewMember > crew_;
    std::string            appearance_code_;

    logger_t logger_;

    int    full_hull_ { 0 };
    double full_fuel_ { 0 };
    int    hull_ { 0 };
    double fuel_ { 0 };
    double weight_ { 0 };
    double max_weight_ { 0 };
    int    money_ { 0 };
    int    max_crew_ { 0 };
    int    cost_ { 0 };
    int    food_ { 0 };


    SpaceshipState state_ { SpaceshipState::OTHER };

};

}



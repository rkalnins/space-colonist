//
// Created by Roberts Kalnins on 16/12/2020.
//

#pragma once

#include <set>
#include <map>
#include <vector>

#include "crew_member.h"
#include "item.h"
#include "../logging/logging.h"


namespace sc::play {

enum class SpaceshipState { NONE, PAUSED, MOVING, FIXING, OTHER };


class Spaceship {
  public:

    explicit Spaceship ( std::string &appearance_code );

    /**
     * Logger must be initialized late
     */
    void LoggingInit ();

    void SpaceshipInitConfig ( double max_weight, double full_fuel,
                               int full_hull, int max_crew, int init_money,
                               int cost );

    /**
     * If fuel, adds quantity to fuel, if food, adds to quantity to food and item to inventory
     * @param item
     * @return
     */
    bool AddItem ( Item &item );

    /**
     * Checks weight and cost requirements of item
     * @param item
     * @return
     */
    bool RemoveItem ( Item &item );

    void AddCrewMember ( const CrewMember &crew_member );

    /**
     * Removes crew member if IsDead()
     */
    void RemoveDeadCrew ();

    /**
     * Consumes the requested number of cables and components
     * @param cables
     * @param components
     * @return true if requested components are available
     */
    bool UseSpareParts ( int cables, int components );

    bool UpdateWeight ( double change );

    void UseFuel ( double usage );

    void UpdateMoney ( int change );

    [[nodiscard]] std::map< std::string, std::vector< Item>> &
    GetItems ();

    [[nodiscard]] std::vector< CrewMember > &GetCrew ();

    [[nodiscard]] const std::string &GetAppearanceCode () const;

    [[nodiscard]] double GetMaxWeight () const;

    [[nodiscard]] double GetWeight () const;

    [[nodiscard]] double GetFullFuel () const;

    [[nodiscard]] double GetFuel () const;

    [[nodiscard]] int GetFullHull () const;

    [[nodiscard]] int GetHull () const;

    [[nodiscard]] int GetMoney () const;

    [[nodiscard]] int GetCost () const;

    [[nodiscard]] int GetMaxCrew () const;

    [[nodiscard]] int GetFood () const;

    void UseFood ();

    bool IsPaused ();

    void Unpause ();

    void Pause ();

    [[nodiscard]] bool HasFuel () const;

    [[nodiscard]] bool HasFood () const;

  private:

    /**
     * Attempts to add fuel. Returns false if over max fuel
     * @param item
     * @return
     */
    bool AddFuel ( Item &item );

    /**
     * Attempts to remove fuel. Returns false if result less than 0
     * @param item
     * @return
     */
    bool RemoveFuel ( Item &item );

    bool CanAddItem ( Item &item );


    class RemoveDeadComp {
      public:
        bool operator() ( CrewMember &c ) const {
            return c.IsDead();
        }
    };


  private:

    std::map< std::string, std::vector< Item > > items_;

    std::vector< CrewMember > crew_;

    logger_t logger_;

    std::string appearance_code_;

    SpaceshipState state_ { SpaceshipState::NONE };

    double full_fuel_ { 0 };
    double fuel_ { 0 };
    double max_weight_ { 0 };
    double weight_ { 0 };

    int full_hull_ { 0 };
    int hull_ { 0 };
    int money_ { 0 };
    int cost_ { 0 };
    int food_ { 0 };
    int max_crew_ { 0 };
};

using shared_spaceship_t = std::shared_ptr< Spaceship >;

}



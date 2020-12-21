//
// Created by Roberts Kalnins on 16/12/2020.
//

#pragma once

#include <string>
#include <map>


namespace sc::play {

class CrewMember {

  public:
    CrewMember ( std::string name, int health, int max_health,
                 std::map< std::string, int > skills );

    [[nodiscard]] const std::string &GetName () const;

    [[nodiscard]] int GetHealth () const;

    [[nodiscard]] const std::map< std::string, int > &GetSkills () const;

    [[nodiscard]] int GetMaxHealth () const;

    bool operator< ( const CrewMember &rhs ) const {
        return name_ < rhs.name_;
    }

    bool operator== ( const CrewMember &rhs ) const {
        return name_ == rhs.name_;
    }

    const std::string &GetCode ();

    void UpdateHealth ( int change );

    [[nodiscard]] bool IsDead () const { return health_ == 0; };

    class NameComparator {
      public:
        explicit NameComparator ( const std::string &compTo ) : comp_to_(
                compTo) {}

        bool operator() ( const CrewMember &rhs ) const {
            return rhs.name_ == comp_to_;
        }

      private:
        const std::string &comp_to_;
    };

  private:
    std::string name_;
    std::string appearance_code_;
    int         health_;
    int         max_health_;

  private:

    std::map< std::string, int > skills_;

};

}





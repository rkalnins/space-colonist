//
// Created by Roberts Kalnins on 16/12/2020.
//

#pragma once

#include <string>
#include <map>


namespace sc::play {

class CrewMember {

  public:
    CrewMember ( std::string name, int max_health,
                 std::map< std::string, int > skills );

    [[nodiscard]] const std::map< std::string, int > &GetSkills () const;

    [[nodiscard]] const std::string &GetName () const;

    const std::string &GetCode ();

    [[nodiscard]] int GetMaxHealth () const;

    [[nodiscard]] int GetHealth () const;

    void UpdateHealth ( int change );

    [[nodiscard]] bool IsDead () const;


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


    bool operator< ( const CrewMember &rhs ) const {
        return name_ < rhs.name_;
    }

    bool operator== ( const CrewMember &rhs ) const {
        return name_ == rhs.name_;
    }

  private:
    std::map< std::string, int > skills_;

    std::string name_;
    std::string appearance_code_;

    int max_health_;
    int health_;


};

}





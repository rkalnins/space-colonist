//
// Created by Roberts Kalnins on 16/12/2020.
//

#pragma once

#include <string>
#include <map>


namespace sc::play {

class CrewMember {

  public:
    CrewMember ( std::string name, int health,
                 std::map< std::string, int > skills );

    const std::string &GetName () const;

    int GetHealth () const;

    const std::map< std::string, int > &GetSkills () const;

  private:

    std::string name_;
    int         health_;

    std::map< std::string, int > skills_;

};

}





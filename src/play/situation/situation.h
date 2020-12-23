//
// Created by Roberts Kalnins on 22/12/2020.
//

#pragma once

#include <string>

#include "situation_type.h"


namespace sc::play {

class Situation {
  public:
    Situation ( const std::string &issue, SituationType type,
                double fix_prob, int required_cables,
                int required_components );

    [[nodiscard]] SituationType GetType () const;

    [[nodiscard]] double GetFixProb () const;

    [[nodiscard]] int GetRequiredCables () const;

    [[nodiscard]] int GetRequiredComponents () const;

  private:

    const std::string &issue_;

    const SituationType type_;

    const double fix_prob_;
    const int    required_cables_;
    const int    required_components_;
};


}




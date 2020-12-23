//
// Created by Roberts Kalnins on 22/12/2020.
//

#include "situation.h"


namespace sc::play {

Situation::Situation ( const std::string &issue, SituationType type,
                       double fix_prob,
                       int required_cables, int required_components )
        : issue_(issue), type_(type), fix_prob_(fix_prob),
          required_cables_(required_cables),
          required_components_(required_components) {}

SituationType Situation::GetType () const { return type_; }

double Situation::GetFixProb () const { return fix_prob_; }

int Situation::GetRequiredCables () const { return required_cables_; }

int
Situation::GetRequiredComponents () const { return required_components_; }


}
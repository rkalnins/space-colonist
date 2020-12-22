//
// Created by Roberts Kalnins on 17/12/2020.
//

#include "spaceship_handler.h"
#include "crew_member.h"


namespace sc::play {

SpaceshipHandler::SpaceshipHandler () : logger_(
        CreateLogger("ss_handler")) {}

const std::shared_ptr< play::Spaceship > &
SpaceshipHandler::GetSpaceship () const {
    return spaceship_;
}

void SpaceshipHandler::SetSpaceship (
        const std::shared_ptr< play::Spaceship > &spaceship ) {
    spaceship_ = spaceship;
}

void
SpaceshipHandler::SetCrew ( const std::vector< CrewMember > &crew_choices,
                            std::vector< Checkbox > &selected ) {
    for ( int i = 0; i < selected.size(); ++i ) {
        if ( selected[i].checked ) {
            spaceship_->AddCrewMember(crew_choices[i]);
        }
    }
}

}
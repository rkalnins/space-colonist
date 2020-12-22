//
// Created by Roberts Kalnins on 17/12/2020.
//

#include "spaceship_handler.h"

#include <utility>
#include "crew_member.h"


namespace sc::play {

SpaceshipHandler::SpaceshipHandler (
        std::shared_ptr< SpaceshipFactory > spaceship_factory )
        : spaceship_factory_(std::move(spaceship_factory)), logger_(
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

void SpaceshipHandler::PrintSpaceship ( WINDOW *main, int y, int x ) {
    spaceship_factory_->PrintSpaceship(main, y, x,
                                       spaceship_->GetAppearanceCode());
}

}
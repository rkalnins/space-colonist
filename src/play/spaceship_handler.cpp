//
// Created by Roberts Kalnins on 17/12/2020.
//

#include "spaceship_handler.h"

#include <utility>
#include <effolkronium/random.hpp>

#include "crew_member.h"


namespace sc::play {

using Random = effolkronium::random_static;

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

void SpaceshipHandler::PrintSpaceship ( WINDOW *main ) {
    spaceship_factory_->PrintSpaceship(main, ss_pos_y_, ss_pos_x_,
                                       spaceship_->GetAppearanceCode());
}

void SpaceshipHandler::BoundSpaceshipPosition () {
    if ( ss_pos_x_ > ss_max_x_ ) {
        ss_pos_x_ = ss_max_x_;
    }

    if ( ss_pos_x_ < ss_min_x_ ) {
        ss_pos_x_ = ss_min_x_;
    }

    if ( ss_pos_y_ > ss_max_y_ ) {
        ss_pos_y_ = ss_max_y_;
    }

    if ( ss_pos_y_ < ss_min_y_ ) {
        ss_pos_y_ = ss_min_y_;
    }
}

void SpaceshipHandler::MoveSpaceship () {
    bool move_x = Random::get< bool >(ss_mvmt_x_prob_);
    bool move_y = Random::get< bool >(ss_mvmt_y_prob_);

    if ( move_x ) {
        ss_pos_x_ += Random::get({ -1, 1 });
    }

    if ( move_y ) {
        ss_pos_y_ += Random::get({ -1, 1 });
    }

    BoundSpaceshipPosition();

}

}
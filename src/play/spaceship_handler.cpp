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
        shared_spaceship_factory_t spaceship_factory )
        : spaceship_factory_(std::move(spaceship_factory)), logger_(
        CreateLogger("ss_handler")) {

    Config &config = Config::GetInstance();

    ss_pos_y_ = config.GetValue< int >("play.spaceship-handler.init-y",
                                       0);
    ss_pos_x_ = config.GetValue< int >("play.spaceship-handler.init-x",
                                       0);

}

const std::shared_ptr< play::Spaceship > &
SpaceshipHandler::GetSpaceship () const {
    return spaceship_;
}

void
SpaceshipHandler::SetSpaceship ( const shared_spaceship_t &spaceship ) {
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

void SpaceshipHandler::StartFlip () {
    start_flip_ = true;
}

void SpaceshipHandler::Burn () {
    burn_ = true;
}

void SpaceshipHandler::PrintSpaceship ( WINDOW *main ) {

    spaceship_factory_->PrintSpaceship(main, ss_pos_y_,
                                       ss_pos_x_,
                                       start_flip_,
                                       !spaceship_->IsPaused(),
                                       burn_);

    burn_       = false;
    start_flip_ = false;
}

bool SpaceshipHandler::PrintDeparture ( WINDOW *main ) {
    bool ret = spaceship_factory_->PrintStationDeparture(main,
                                                         static_cast<int>(
                                                                 departure_cycle_++ *
                                                                 departure_speed_));

    spaceship_factory_->PrintSpaceship(main, ss_pos_y_, ss_pos_x_,
                                       false, false, false);

    return ret;
}

bool SpaceshipHandler::PrintArrival ( WINDOW *main ) {
    bool ret = spaceship_factory_->PrintStationArrival(main,
                                                         static_cast<int>(
                                                                 arrival_cycle_++ *
                                                                 arrival_speed_));

    spaceship_factory_->PrintSpaceship(main, ss_pos_y_, ss_pos_x_,
                                       false, false, false);

    return ret;
}

}
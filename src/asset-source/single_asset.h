//
// Created by Roberts Kalnins on 29/12/2020.
//

#ifndef SPACE_COLONIST_SINGLE_ASSET_H
#define SPACE_COLONIST_SINGLE_ASSET_H

#include <fstream>
#include <vector>
#include <string>

#include "../logging/logging.h"
#include "../assets/departure_station.h"
#include "../assets/arrival_station.h"
#include "../assets/ship_a.h"


namespace sc {

enum class Asset { DEPARTURE_STATION, ARRIVAL_STATION, SHIP_A };

using SingleAsset = std::vector< const std::string >;
using AnimationFrames = std::vector< const std::vector< const std::string >>;

static std::unique_ptr< SingleAsset > GetAsset ( Asset asset ) {

    static logger_t logger = CreateLogger("asset");

    switch ( asset ) {

        case Asset::DEPARTURE_STATION: {
            assets::DepartureStation s;
            return std::make_unique< SingleAsset >(s.station_dep_);
        }
        case Asset::ARRIVAL_STATION: {
            assets::ArrivalStation s;
            return std::make_unique< SingleAsset >(s.station_arrival_);
        }
        default:
            return nullptr;
    }
}

static std::unique_ptr< AnimationFrames > GetFrames ( Asset asset ) {

    static logger_t logger = CreateLogger("asset");

    switch ( asset ) {

        case Asset::SHIP_A: {
            assets::ShipA sa;
            return std::make_unique< AnimationFrames >(sa.ship);
        }
        default:
            return nullptr;
    }
}

}


#endif //SPACE_COLONIST_SINGLE_ASSET_H

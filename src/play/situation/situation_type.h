//
// Created by Roberts Kalnins on 22/12/2020.
//

#ifndef SC_SITUATION_TYPE_H
#define SC_SITUATION_TYPE_H

#include <string>


enum class SituationType {
    NONE,
    MINOR,
    ENGINE_FAILURE,
    AIR_FILTER_FAILURE,
    GYROSCOPE_FAILURE,
    MINOR_HULL_BREACH,
    MAJOR_HULL_BREACH,
    APPROACHING_SHIP,
};


static const std::string GetTypePath ( SituationType type ) {
    switch ( type ) {

        case SituationType::NONE:
            return "none";
        case SituationType::MINOR:
            return "minor";
        case SituationType::ENGINE_FAILURE:
            return "major.standard-engine";
        case SituationType::AIR_FILTER_FAILURE:
            return "major.air-filter";
        case SituationType::GYROSCOPE_FAILURE:
            return "major.gyroscope";
        case SituationType::MINOR_HULL_BREACH:
            return "major.minor-hull";
        case SituationType::MAJOR_HULL_BREACH:
            return "major.hull";
        case SituationType::APPROACHING_SHIP:
            return "major.approaching-ship";
    }
}

#endif //SC_SITUATION_TYPE_H

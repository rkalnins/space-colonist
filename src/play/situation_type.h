//
// Created by Roberts Kalnins on 22/12/2020.
//

#ifndef SC_SITUATION_TYPE_H
#define SC_SITUATION_TYPE_H

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

#endif //SC_SITUATION_TYPE_H

//
// Created by Roberts Kalnins on 25/12/2020.
//

#include <catch2/catch.hpp>
#include <vector>
#include <string>

#include <iostream>

#include "../play/space_map.h"


using sc::play::SpaceMap;


class SpaceMapTest : public SpaceMap {
  public:
    SpaceMapTest ( WINDOW *main, int map_init_y, int map_init_x )
            : SpaceMap(
            main, map_init_y, map_init_x) {

        map_height_ = 4;
        map_width_  = 6;

        planets_.resize(map_height_,
                        std::vector< bool >(map_width_, false));
        planets_str_.resize(map_height_, std::string(map_width_, ' '));

        planets_ = {
                { false, true, false, false, true,  false },
                { false, true, false, false, true,  false },
                { false, true, false, false, false, false },
                { false, true, false, false, true,  false }
        };

        for ( int y = 0; y < map_height_; ++y ) {
            for ( int x = 0; x < map_width_; ++x ) {
                if ( planets_[y][x] ) {
                    planets_str_[y][x] = '*';
                }
            }
        }

    };

    [[nodiscard]] const std::vector< std::string > &GetMap () const {
        return planets_str_;
    }

    [[maybe_unused]] [[nodiscard]] int
    GetMaxHeight () const { return map_height_; }

    [[maybe_unused]] [[nodiscard]] int
    GetMaxWidth () const { return map_width_; }

    [[maybe_unused]] [[nodiscard]] int
    GetMaxRouteLength () const { return max_route_length_; }
};


TEST_CASE("7.1 Space Map", "[multi-file:7]") {
    SpaceMapTest spacemap(nullptr, 0, 0);

    REQUIRE(spacemap.GetCost() == 0);

    std::vector< SpaceMapTest::LocationSelected > locations;
    locations.emplace_back(0, 1);
    locations.emplace_back(1, 4);
    locations.emplace_back(3, 4);
    locations.emplace_back(0, 4);
    locations.emplace_back(2, 1);
    locations.emplace_back(1, 1);

    SECTION("7.2 Simple route selection") {

        for ( int i = 0; i < spacemap.GetMaxRouteLength(); ++i ) {
            spacemap.ToggleSelection(locations[i].y, locations[i].x);

            REQUIRE(spacemap.GetMap()[locations[i].y][locations[i].x -
                                                      1] == '(');
            REQUIRE(spacemap.GetMap()[locations[i].y][locations[i].x] ==
                    '1' + i);
            REQUIRE(spacemap.GetMap()[locations[i].y][locations[i].x +
                                                      1] == ')');
        }

        double cost = spacemap.GetCost();
        spacemap.ToggleSelection(locations[5].y, locations[5].x);
        REQUIRE(spacemap.GetCost() == cost);
    }

    SECTION("7.3 Planet toggling") {
        spacemap.ToggleSelection(locations[0].y, locations[0].x);
        double cost_1 = spacemap.GetCost();
        spacemap.ToggleSelection(locations[1].y, locations[1].x);
        spacemap.ToggleSelection(locations[1].y, locations[1].x);
        REQUIRE(spacemap.GetCost() == cost_1);
        spacemap.ToggleSelection(locations[0].y, locations[0].x);
        REQUIRE(spacemap.GetCost() == 0);
    }
}

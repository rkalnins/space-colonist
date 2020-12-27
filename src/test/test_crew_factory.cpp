//
// Created by Roberts Kalnins on 26/12/2020.
//

#include <catch2/catch.hpp>

#include "../play/crew_factory.h"


using sc::play::CrewMemberFactory;
using sc::play::CrewMember;


class CrewMemberFactoryTest : public CrewMemberFactory {

  public:
    [[nodiscard]] std::map< std::string, bool > &GetNames () {
        return names_;
    }
};


TEST_CASE("8.1 Crew Factory") {
    CrewMemberFactoryTest factory;


    SECTION("8.1.1 Creating crew") {
        CrewMember c1 = factory.Create();

        for ( auto &c : factory.GetNames()) {
            if ( c1.GetName() == c.first ) {
                REQUIRE(c.second);
            } else {
                REQUIRE_FALSE(c.second);
            }
        }

        CrewMember c2 = factory.Create();

        for ( auto &c : factory.GetNames()) {
            if ( c1.GetName() == c.first || c2.GetName() == c.first ) {
                REQUIRE(c.second);
            } else {
                REQUIRE_FALSE(c.second);
            }
        }

        CrewMember c3 = factory.Create();

        for ( auto &c : factory.GetNames()) {
            if ( c1.GetName() == c.first || c2.GetName() == c.first ||
                 c3.GetName() == c.first ) {
                REQUIRE(c.second);
            } else {
                REQUIRE_FALSE(c.second);
            }
        }

        factory.Create();
        factory.Create();
        factory.Create();
        factory.Create();
        factory.Create();

        for ( auto &c : factory.GetNames()) {
            REQUIRE(c.second);
        }
    }

    // TODO: add checks for skills toggling and quantity, health range

}
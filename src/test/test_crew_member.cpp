//
// Created by Roberts Kalnins on 24/12/2020.
//

#include <catch2/catch.hpp>

#include "../play/crew_member.h"


using sc::play::CrewMember;

TEST_CASE("4.1 Crew member basic tests", "[multi-file:4]") {

    const std::string                  &name  = "test";
    const int                          health = 10;
    const std::map< std::string, int > skills = {{ "s1", 1 },
                                                 { "s2", 5 }};


    CrewMember c(name, health, skills);

    SECTION("4.1.1 Getters") {
        REQUIRE(c.GetName() == name);
        REQUIRE(c.GetMaxHealth() == health);
        REQUIRE(c.GetHealth() == health);
        REQUIRE(c.GetSkills() == skills);
    }

    SECTION("4.1.2 Health changes") {
        int change_1 = -1;
        int change_2 = 5;
        int change_3 = -1 * health;

        REQUIRE(c.GetMaxHealth() == health);
        REQUIRE(c.GetHealth() == c.GetMaxHealth());
        c.UpdateHealth(change_1);
        REQUIRE(c.GetHealth() == c.GetMaxHealth() + change_1);

        c.UpdateHealth(change_2);
        REQUIRE(c.GetHealth() == c.GetMaxHealth());

        c.UpdateHealth(change_3);
        REQUIRE(c.GetHealth() == c.GetMaxHealth() + change_3);
    }

    SECTION("4.1.3 IsDead") {
        REQUIRE_FALSE(c.IsDead());
        c.UpdateHealth(-1 * c.GetMaxHealth() + 1);
        REQUIRE_FALSE(c.IsDead());
        c.UpdateHealth(-1);
        REQUIRE(c.IsDead());
        c.UpdateHealth(-1);
        REQUIRE(c.IsDead());
    }
}
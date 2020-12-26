//
// Created by Roberts Kalnins on 24/12/2020.
//

#include <catch2/catch.hpp>
#include <iostream>

#include "../play/spaceship.h"
#include "../play/spaceship_factory.h"
#include "../play/spaceship_handler.h"
#include "../play/crew_factory.h"


using namespace sc::play;


TEST_CASE("6.1 Spaceship Handler", "[multi-file:6]") {
    shared_spaceship_factory_t spaceship_factory = std::make_shared< SpaceshipFactory >();

    CrewMemberFactory crew_factory;
    SpaceshipHandler  handler(spaceship_factory);


    SECTION("6.1.1 Setting spaceship") {
        shared_spaceship_t s = spaceship_factory->CreateSpaceship();

        handler.SetSpaceship(s);

        REQUIRE(handler.GetSpaceship() == s);
        REQUIRE(handler.GetSpaceship()->GetAppearanceCode() ==
                s->GetAppearanceCode());
    }

    SECTION("6.1.2 Setting crew") {
        const int crew_choices_size = 5;
        const int selected          = 2;

        std::vector< CrewMember > crew;
        std::vector< Checkbox >   selected_crew(crew_choices_size);

        crew.reserve(crew_choices_size);

        for ( int i = 0; i < crew_choices_size; ++i ) {
            crew.push_back(crew_factory.Create());
        }

        selected_crew[0].checked = true;
        selected_crew[3].checked = true;

        shared_spaceship_t s = spaceship_factory->CreateSpaceship();

        handler.SetSpaceship(s);
        handler.SetCrew(crew, selected_crew);

        REQUIRE(s->GetCrew().size() == selected);
        REQUIRE(s->GetCrew()[0].GetName() == crew[0].GetName());
        REQUIRE(s->GetCrew()[1].GetName() == crew[3].GetName());
    }
}
//
// Created by Roberts Kalnins on 24/12/2020.
//

#include <catch2/catch.hpp>
#include "../play/item.h"


using sc::play::Item;

TEST_CASE("2.1 Item basic tests", "[multi-file:2]") {
    const std::string &category = "test_cat";
    const std::string &name     = "test_item";
    const int         quantity  = GENERATE(0, 10, 15);
    const int weight = GENERATE(0, 11, 16);
    const int cost = GENERATE(0, 12, 17);

    Item item(category, name, quantity, weight, cost);

    SECTION("2.1.1 Getters") {
        REQUIRE(item.GetCategory() == category);
        REQUIRE(item.GetName() == name);
        REQUIRE(item.GetQuantity() == quantity);
        REQUIRE(item.GetWeight() == weight);
        REQUIRE(item.GetCost() == cost);
    }
}

TEST_CASE("2.2 Item quantity updates", "[multi-file:2") {

    const std::string &category = "test_cat";
    const std::string &name     = "test_item";
    const int         quantity  = 1;
    const int         weight    = GENERATE(0, 11);
    const int cost = GENERATE(0, 12);

    Item item(category, name, quantity, weight, cost);


    SECTION("2.2.2 Hard Quantity update positive") {
        const int update_pos   = 2;
        const int new_quantity = 3;
        REQUIRE(item.HardUpdateQuantity(update_pos));
        REQUIRE(item.GetQuantity() == new_quantity);

    }

    SECTION("2.2.3 Hard Quantity update negative") {
        const int update_neg   = -5;
        const int new_quantity = 0;
        REQUIRE_FALSE(item.HardUpdateQuantity(update_neg));
        REQUIRE(item.GetQuantity() == new_quantity);
    }

    SECTION("2.2.4 Soft Quantity update positive") {
        const int update_pos = -5;
        REQUIRE_FALSE(item.SoftUpdateQuantity(update_pos));
        REQUIRE(item.GetQuantity() == quantity);
    }

    SECTION("2.2.5 Soft Quantity update negative") {
        const int update_neg = -5;
        REQUIRE_FALSE(item.SoftUpdateQuantity(update_neg));
        REQUIRE(item.GetQuantity() == quantity);
    }

    SECTION("2.2.6 Soft Quantity update negative to 0") {
        const int update_neg   = -1;
        const int new_quantity = 0;
        REQUIRE(item.SoftUpdateQuantity(update_neg));
        REQUIRE(item.GetQuantity() == new_quantity);
    }

    SECTION("2.2.7 Partial Quantity update negative no remains") {
        const int update_neg   = -1;
        const int remaining    = 0;
        const int new_quantity = 0;

        REQUIRE(item.PartialUpdateQuantity(update_neg) == remaining);
        REQUIRE(item.GetQuantity() == new_quantity);
    }

    SECTION("2.2.8 Partial Quantity update negative with remains") {
        const int update_neg   = -5;
        const int remaining    = 4;
        const int new_quantity = 0;

        REQUIRE(item.PartialUpdateQuantity(update_neg) == remaining);
        REQUIRE(item.GetQuantity() == new_quantity);
    }

    SECTION("2.2.9 Partial Quantity update positive") {
        const int update_pos   = 5;
        const int remaining    = 0;
        const int new_quantity = 6;

        REQUIRE(item.PartialUpdateQuantity(update_pos) == remaining);
        REQUIRE(item.GetQuantity() == new_quantity);
    }
}
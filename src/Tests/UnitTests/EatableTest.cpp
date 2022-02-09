/*
* prjsv 2019
* 2016-19
* Marco Antognini & Jamila Sam
*/

#include <Application.hpp>
#include <Animal/Gerbil.hpp>
#include <Animal/Scorpion.hpp>
#include <Environment/OrganicEntity.hpp>
#include <Environment/Food.hpp>
#include <Config.hpp>
#include <catch.hpp>
#include <iostream>

SCENARIO("Gerbil vs OrganicEntity", "[Gerbil]")
{

    OrganicEntity* gerbil1(new Gerbil({0,0}));
    OrganicEntity* gerbil2(new Gerbil({0,0}));
    OrganicEntity* scorpion1(new Scorpion({0,0}));
    OrganicEntity* food1(new Food({0,0}));

    GIVEN("a gerbil") {
        THEN("no autophagy") {
            CHECK_FALSE(gerbil1->eatable(gerbil1));
        }
    }
    GIVEN("two gerbils") {
        THEN("no cannibalism") {
            CHECK_FALSE(gerbil1->eatable(gerbil2));
            CHECK_FALSE(gerbil2->eatable(gerbil1));
        }
    }

    GIVEN("a gerbil and a food") {

        THEN("the food is eatable by the gerbil") {
            CHECK(gerbil1->eatable(food1));
        }
        THEN("the gerbil is not eatable by the food") {
            CHECK_FALSE(food1->eatable(gerbil1));
        }
    }

    GIVEN("a gerbil and a scorpion") {

        THEN("the gerbil is eatable by the scorpion") {
            CHECK(scorpion1->eatable(gerbil1));
        }
        THEN("the scorpion is not eatable by the gerbil") {
            CHECK_FALSE(gerbil1->eatable(scorpion1));
        }
    }
    delete gerbil1;
    delete gerbil2;
    delete scorpion1;
    delete food1;
}

SCENARIO("Food vs OrganicEntity", "[Food]")
{

    OrganicEntity* gerbil1(new Gerbil({0,0}));
    OrganicEntity* scorpion1(new Scorpion({0,0}));
    OrganicEntity* food1(new Food({0,0}));
    OrganicEntity* food2(new Food({0,0}));
    GIVEN("a food") {
        THEN("no autophagy") {
            CHECK_FALSE(food1->eatable(food1));
        }
    }
    GIVEN("two foods") {
        THEN("no cannibalism") {
            CHECK_FALSE(food1->eatable(food2));
            CHECK_FALSE(food2->eatable(food1));
        }
    }

    GIVEN("a food and a scorpion") {

        THEN("the food is not eatable by the scorpion") {
            CHECK_FALSE(scorpion1->eatable(food1));
        }
        THEN("the scorpion is not eatable by the food") {
            CHECK_FALSE(food1->eatable(scorpion1));
        }
    }

    GIVEN("a food and a gerbil") {

        THEN("the food is eatable by the gerbil") {
            CHECK(gerbil1->eatable(food1));
        }
        THEN("the gerbil is not eatable by the food") {
            CHECK_FALSE(food1->eatable(gerbil1));
        }
    }
    delete gerbil1;
    delete scorpion1;
    delete food1;
    delete food2;
}

SCENARIO("Scorpion vs OrganicEntity", "[Scorpion]")
{

    OrganicEntity* gerbil1(new Gerbil({0,0}));
    OrganicEntity* scorpion1(new Scorpion({0,0}));
    OrganicEntity* scorpion2(new Scorpion({0,0}));
    OrganicEntity* food1(new Food({0,0}));

    GIVEN("a scorpion") {
        THEN("no autophagy") {
            CHECK_FALSE(scorpion1->eatable(scorpion1));
        }
    }
    GIVEN("two wolves") {
        THEN("no cannibalism") {
            CHECK_FALSE(scorpion1->eatable(scorpion2));
            CHECK_FALSE(scorpion2->eatable(scorpion1));
        }
    }

    GIVEN("a Scorpion and a food") {

        THEN("the food is not eatable by the scorpion") {
            CHECK_FALSE(scorpion1->eatable(food1));
        }
        THEN("the scorpion is not eatable by the food") {
            CHECK_FALSE(food1->eatable(scorpion1));
        }
    }

    GIVEN("a scorpion and a gerbil") {

        THEN("the gerbil is eatable by the scorpion") {
            CHECK(scorpion1->eatable(gerbil1));
        }
        THEN("the scorpion is not eatable by the gerbil") {
            CHECK(scorpion1->eatable(gerbil1));
        }
    }
    delete gerbil1;
    delete scorpion1;
    delete scorpion2;
    delete food1;
}

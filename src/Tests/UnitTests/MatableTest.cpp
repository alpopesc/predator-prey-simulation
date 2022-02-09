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

SCENARIO("Gerbils' reproduction", "[Gerbil]")
{

    OrganicEntity* male_gerbil1(new Gerbil({0,0}, 0, false));
    OrganicEntity* male_gerbil2(new Gerbil({0,0}, getAppConfig().gerbil_energy_min_mating_male, false));
    OrganicEntity* female_gerbil1(new Gerbil({0,0}, 0, true));
    OrganicEntity* female_gerbil2(new Gerbil({0,0}, getAppConfig().gerbil_energy_min_mating_female, true));
    OrganicEntity* female_scorpion(new Scorpion({0,0}, getAppConfig().scorpion_energy_min_mating_female, true));
    OrganicEntity* male_scorpion(new Scorpion({0,0}, getAppConfig().scorpion_energy_min_mating_male, false));
    OrganicEntity*food(new Food({0,0}));

    // make some gerbils sufficiently mature
    auto age(sf::seconds(getAppConfig().gerbil_min_age_mating));
    male_gerbil2->OrganicEntity::update(age);
    female_gerbil2->OrganicEntity::update(age);

    GIVEN("two gerbils of same gender") {
        THEN("no mating") {
            CHECK_FALSE(male_gerbil1->matable(male_gerbil1));
            CHECK_FALSE(male_gerbil1->matable(male_gerbil2));
            CHECK_FALSE(male_gerbil2->matable(male_gerbil1));
            CHECK_FALSE(female_gerbil1->matable(female_gerbil1));
            CHECK_FALSE(female_gerbil1->matable(female_gerbil2));
            CHECK_FALSE(female_gerbil2->matable(female_gerbil1));
        }
    }
    GIVEN("two gerbils of different genders, but too weak") {
        THEN("no mating") {
            CHECK_FALSE(male_gerbil1->matable(female_gerbil1));
            CHECK_FALSE((male_gerbil2->matable(female_gerbil1)
                         and female_gerbil1->matable(male_gerbil2)));
            CHECK_FALSE((female_gerbil2->matable(male_gerbil1)
                         and male_gerbil1->matable(female_gerbil2)));
        }
    }

    GIVEN("a gerbil and a scorpion") {

        THEN("no mating") {
            CHECK_FALSE(male_gerbil2->matable(female_scorpion));
            CHECK_FALSE(female_scorpion->matable(male_gerbil2));
            CHECK_FALSE(female_gerbil2->matable(male_scorpion));
            CHECK_FALSE(male_scorpion->matable(female_gerbil2));
        }
    }

    GIVEN("a gerbil and a food") {

        THEN("no mating") {
            CHECK_FALSE(male_gerbil2->matable(food));
            CHECK_FALSE(female_gerbil2->matable(food));
            CHECK_FALSE(food->matable(female_gerbil2));
            CHECK_FALSE(food->matable(male_gerbil2));
        }
    }

    GIVEN("two sufficiently old gerbils of different genders and enough energy") {

        THEN("reproduction is possible") {
            CHECK(male_gerbil2->matable(female_gerbil2));
            CHECK(female_gerbil2->matable(male_gerbil2));
        }
    }

    delete male_gerbil1;
    delete male_gerbil2;
    delete female_gerbil1;
    delete female_gerbil2;
    delete food;
    delete male_scorpion;
    delete female_scorpion;
}

SCENARIO("Scorpions' reproduction", "[Scorpion]")
{

    OrganicEntity* male_scorpion1(new Scorpion({0,0}, 0, false));
    OrganicEntity* male_scorpion2(new Scorpion({0,0}, getAppConfig().scorpion_energy_min_mating_male, false));
    OrganicEntity* female_scorpion1(new Scorpion({0,0}, 0, true));
    OrganicEntity* female_scorpion2(new Scorpion({0,0}, getAppConfig().scorpion_energy_min_mating_female, true));
    OrganicEntity* female_gerbil(new Gerbil({0,0}, getAppConfig().gerbil_energy_min_mating_female, true));
    OrganicEntity* male_gerbil(new Gerbil({0,0}, getAppConfig().gerbil_energy_min_mating_male, false));
    OrganicEntity*food(new Food({0,0}));
    // make some scorpions sufficiently mature
    auto age(sf::seconds(getAppConfig().scorpion_min_age_mating));
    male_scorpion2->OrganicEntity::update(age);
    female_scorpion2->OrganicEntity::update(age);

    GIVEN("two scorpions of same gender") {
        THEN("no mating") {
            CHECK_FALSE(male_scorpion1->matable(male_scorpion1));
            CHECK_FALSE(male_scorpion1->matable(male_scorpion2));
            CHECK_FALSE(male_scorpion2->matable(male_scorpion1));
            CHECK_FALSE(female_scorpion1->matable(female_scorpion1));
            CHECK_FALSE(female_scorpion1->matable(female_scorpion2));
            CHECK_FALSE(female_scorpion2->matable(female_scorpion1));
        }
    }
    GIVEN("two scorpions of different genders, but too weak") {
        THEN("no mating") {
            CHECK_FALSE(male_scorpion1->matable(female_scorpion1));
            CHECK_FALSE((male_scorpion2->matable(female_scorpion1)
                         and female_scorpion1->matable(male_scorpion2)));
            CHECK_FALSE((female_scorpion2->matable(male_scorpion1)
                         and male_scorpion1->matable(female_scorpion2)));
        }
    }

    GIVEN("a scorpion and a gerbil") {

        THEN("no mating") {
            CHECK_FALSE(male_scorpion2->matable(female_gerbil));
            CHECK_FALSE(female_gerbil->matable(male_scorpion2));
            CHECK_FALSE(female_scorpion2->matable(male_gerbil));
            CHECK_FALSE(male_gerbil->matable(female_scorpion2));
        }
    }

    GIVEN("a scorpion and a food") {

        THEN("no mating") {
            CHECK_FALSE(male_scorpion2->matable(food));
            CHECK_FALSE(female_scorpion2->matable(food));
            CHECK_FALSE(food->matable(female_scorpion2));
            CHECK_FALSE(food->matable(male_scorpion2));
        }
    }

    GIVEN("two scorpions of different genders and enough energy") {

        THEN("reproduction is possible") {
            CHECK(male_scorpion2->matable(female_scorpion2));
            CHECK(female_scorpion2->matable(male_scorpion2));
        }
    }

    delete male_scorpion1;
    delete male_scorpion2;
    delete female_scorpion1;
    delete female_scorpion2;
    delete food;
    delete male_gerbil;
    delete female_gerbil;
}

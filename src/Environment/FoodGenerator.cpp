#include "FoodGenerator.hpp"
#include <Environment/Environment.hpp>
#include <Environment/Food.hpp>
#include <Random/Normal.hpp>
#include <Utility/Vec2d.hpp>

FoodGenerator::FoodGenerator()
    : temps(sf::Time::Zero)
{} //

void FoodGenerator::addRandomTarget()
{

    //Creations des coordonée aleatoire
    double rdm_coord1(0.0);
    double rdm_coord2(0.0);
    double w_size(getAppConfig().simulation_world_size);
    rdm_coord1 = normal(w_size/2, (w_size/4)*(w_size/4));
    rdm_coord2 = normal(w_size/2, (w_size/4)*(w_size/4));

    //Initialisation l'entite
    Food* ptr_random_food = new Food(Vec2d(rdm_coord1, rdm_coord2));
    getAppEnv().addEntity(ptr_random_food);
}


void FoodGenerator::update(sf::Time dt)
{
    temps = temps + dt;
    if(temps > sf::seconds(getAppConfig().food_generator_delta)) {
        temps = sf::Time::Zero;
        addRandomTarget();
    }
}

//Later we could add the option for the food frequency to vary in function of the environments number of entities and use the population formula)
//We could also use the random generators to make the food frequency random. At the moment only the position is random. If there is time at the end it could be cool to change that.



#ifndef FOODGENERATOR_H
#define FOODGENERATOR_H

#include <iostream>
#include <Utility/Utility.hpp>

class FoodGenerator
{

public:
    FoodGenerator();
    void update(sf::Time dt);


private:
    sf::Time temps;
    void addRandomTarget();

};


#endif

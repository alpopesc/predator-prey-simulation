#ifndef CHASING_AUTOMATON_H
#define CHASING_AUTOMATON_H

#include <iostream>
#include <Utility/Vec2d.hpp>
#include <Utility/Constants.hpp>
#include <Utility/Utility.hpp>
#include <Obstacle/CircularCollider.hpp>

class ChasingAutomaton : public CircularCollider
{

public:

    //Constructeur par default
    ChasingAutomaton(Vec2d posit, double vit = 0, Vec2d dir = Vec2d(0,1), Vec2d trg = Vec2d(0,0));

    double getStandardMaxSpeed();
    double getMass();

    void setTargetPosition(Vec2d new_targ);
    Vec2d getSpeedVector();

    void update(sf::Time dt);
    void draw(sf::RenderTarget& targetWindow);


    Vec2d force_attrac();
    void maj_deplacement(Vec2d f, sf::Time dt);


private:

    double vitesse;
    Vec2d direction;
    Vec2d target;
};

enum Deceleration {forte= 9, moy = 6, faible = 3};
//ne pas oublier de multiplier par 0.1


#endif

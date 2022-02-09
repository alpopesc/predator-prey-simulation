#ifndef ORGANICENTITY_H
#define ORGANICENTITY_H

#include <iostream>
#include <Obstacle/CircularCollider.hpp>



class Gerbil;
class Scorpion;
class Food;
class Animal;

class OrganicEntity : public CircularCollider, public Updatable, public Drawable
{


public:

    //Construceur et  destructeur
    OrganicEntity(Vec2d posit, double taille, double niv_energy,
                  sf::Time age_p = sf::Time::Zero);
    virtual ~OrganicEntity();

    virtual bool eatable(OrganicEntity const* other) const = 0;
    virtual bool eatableBy(Scorpion const* scorpion) const = 0;
    virtual bool eatableBy(Gerbil const* gerbil) const = 0;
    virtual bool eatableBy(Food const* food) const = 0;
    
    virtual bool matable(OrganicEntity const* other) const = 0;
	virtual bool canMate(Scorpion const* scorpion) const = 0;
	virtual bool canMate(Gerbil const* gerbil) const = 0;
	virtual bool canMate(Food const* food) const = 0;

	virtual void meet(OrganicEntity* mate) = 0;
	
    //Aging
    void update(sf::Time dt);
    double getMinEnergy() const;
    virtual sf::Time getLongevity() const = 0;
    bool IsDead() const;


	double getEnergy() const;
	double getAge() const;
	void setEnergy(double energy_final); // quand un animal mange une OrganicEntity, il met son energy à zéro.
	

									 
    //Methodes virtuelles pures
    //virtual void draw(sf::RenderTarget& targetWindow) = 0;
    //virtual void update(sf::Time dt) = 0;

protected:
    double energy;
    sf::Time age;

	

};




#endif

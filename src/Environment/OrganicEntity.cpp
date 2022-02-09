#include "OrganicEntity.hpp"
#include <iostream>
#include "OrganicEntity.hpp"




//Constructeur et destructeur
OrganicEntity::OrganicEntity(Vec2d posit, double taille, double niv_energy,
                             sf::Time age_p)
    : CircularCollider(posit, (0.5*taille)), energy(niv_energy), age(age_p)
{}

OrganicEntity::~OrganicEntity()
{}

//Aging
void OrganicEntity::update(sf::Time dt)
{
    age += dt;
}



double OrganicEntity::getMinEnergy() const
{
    return getAppConfig().animal_min_energy;
}

bool OrganicEntity::IsDead() const
{
    if((energy < getMinEnergy()) or (age.asSeconds() > getLongevity().asSeconds())) {
		std::cout << "Im deaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaad" << std::endl;
        return true;
        
    } else {
        return false;

    }
}

double OrganicEntity::getEnergy() const
{
	return energy;
}

double OrganicEntity::getAge() const{
	return age.asSeconds();
}

void OrganicEntity::setEnergy(double energy_final)
{
	energy = energy_final;
}


	

/*
void OrganicEntity::drawDebugText(sf::RenderTarget& targetWindow) const{
	std::string energy_text(to_nice_string(energy);
		auto text = buildText(energy_text,
                      position + direction,
                      getAppFont(),
                      getAppConfig().default_debug_text_size,
                      debug_color,
                      getRotation() / DEG_TO_RAD + 90); // si nécessaire
	targetWindow.draw(text);
*/

//If we want for food to have a different debug mode. We will have to make this method virtual so that it can be overwritten.

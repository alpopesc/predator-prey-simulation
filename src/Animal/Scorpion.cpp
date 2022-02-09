#include "Scorpion.hpp"
#include <iostream>
#include <Application.hpp>
#include <Config.hpp>
#include <cmath>
#include <Random/Uniform.hpp>

//Getters

double Scorpion::getStandardMaxSpeed() const
{
    return getAppConfig().scorpion_max_speed;
}

double Scorpion::getMass() const
{
    return getAppConfig().scorpion_mass;
}

double Scorpion::getSize() const
{
    return getAppConfig().scorpion_size;
}

double Scorpion::getInitialEnergy() const
{
    return getAppConfig().scorpion_energy_initial;
}

double Scorpion::getViewRange() const
{
    return getAppConfig().scorpion_view_range;
}

double Scorpion::getViewDistance() const
{
    return getAppConfig().scorpion_view_distance;
}

double Scorpion::getRandomWalkRadius() const
{
    return getAppConfig().scorpion_random_walk_radius;
}

double Scorpion::getRandomWalkDistance() const
{
    return getAppConfig().scorpion_random_walk_distance;
}

double Scorpion::getRandomWalkJitter() const
{
    return getAppConfig().scorpion_random_walk_jitter;
}

std::string Scorpion::getTexture() const
{
    return getAppConfig().scorpion_texture;
}

double Scorpion::getMaxSpeed() const
{

    double vitesseMax_tmp = getStandardMaxSpeed();

    //seuil de fatigue -> ajouter dans appTest.json? faire un nouvel attribut?
    if(energy < 30.0) {
        vitesseMax_tmp = getStandardMaxSpeed() * 0.2;
    }

    switch(state) {
    case RUNNING_AWAY: {
        return vitesseMax_tmp *= 4;
    }
    case FOOD_IN_SIGHT: {
        return vitesseMax_tmp *= 3;
    }
    case MATE_IN_SIGHT: {
        return vitesseMax_tmp *= 2;
    }
    default: {
        return vitesseMax_tmp;
    }
    }
}

sf::Time Scorpion::getLongevity() const
{
    return getAppConfig().scorpion_longevity;
}

double Scorpion::getFacteurPerteEnergy() const
{
    return getAppConfig().scorpion_energy_loss_factor;
}

double Scorpion::getEnergyLossPerChildF() const
{
	return getAppConfig().scorpion_energy_loss_female_per_child;
}

double Scorpion::getEnergyLossM() const
{
	return getAppConfig().scorpion_energy_loss_mating_male;
}

unsigned int Scorpion::getMinChild() const
{
	return getAppConfig().scorpion_min_children;
}

unsigned int Scorpion::getMaxChild() const
{
	return getAppConfig().scorpion_max_children;
}

float Scorpion::getGestationTime() const
{	
	return getAppConfig().scorpion_gestation_time;
}		


std::string Scorpion::getMatingTexture() const
{
	return "heart1.png";
}


double Scorpion::getMatingTextureSize() const
{
	switch(counter_draw) {
		
		case 1 : {
			return (0.3 * rayon);
			break;
		}
		
		case 2 : {
			return (0.4 * rayon);
			break;
		}
		
		case 3 : {
			return (0.5 * rayon);
			break;
		}
		
		case 4 : {
			return (0.4 * rayon);
			break;
		}
		
		case 5 : {
			return (0.3 * rayon);
			break;
		}
		
		default : {
			return 0.3 * rayon;
		}
	}

}

double Scorpion::getLossFactor() const
{
	return getAppConfig().scorpion_energy_loss_factor;
}


//Hunt or be hunted :)
bool Scorpion::eatable(OrganicEntity const* other) const
{
    return other->eatableBy(this);
}

bool Scorpion::eatableBy(Scorpion const* scorpion) const
{
    return false;
}

bool Scorpion::eatableBy(Gerbil const* gerbil) const
{
    return false;
}

bool Scorpion::eatableBy(Food const* food) const
{
    return false;
}





//Do it like they do it on DiscoveryChannel o/ o/
bool Scorpion::matable(OrganicEntity const* other) const{
	return other -> canMate(this);
}

bool Scorpion::canMate(Scorpion const* scorpion) const{
	
	if (scorpion -> getAge() >= getAppConfig().scorpion_min_age_mating){
		

		if(femelle){
			if(not scorpion -> isFemale() and 
			scorpion -> getEnergy() >= getAppConfig().scorpion_energy_min_mating_male and 
			this -> getEnergy() >= getAppConfig().gerbil_energy_min_mating_male and
			not this -> isPregnant()){
			return true;
			}
			
		}
    
		else{
			if(scorpion -> isFemale() and not scorpion -> isPregnant() and 
			scorpion -> getEnergy() >= getAppConfig().scorpion_energy_min_mating_female and
			this -> getEnergy() >= getAppConfig().scorpion_energy_min_mating_female){
				return true;
			}			
		}
	}
	//std::cerr << "Error Not female or male " << std::endl;
	return false;
}
 
bool Scorpion::canMate(Gerbil const* gerbil) const{
	return false;
}

bool Scorpion::canMate(Food const* food) const{
	return false;
}
	

/*In case off a much more differentiated meet method we could define this method as virtually pure in the class Animal and redefinde them in each corresponding Animal	
	
void Gerbil::meet(OrganicEntity* mate){

	if(matable(mate) and mate -> matable(this)){
		
		switch(femelle) {

		case true: {
			
			pregnant = true;
			nmb_offspring = uniform(getAppConfig().scorpion_min_children, getAppConfig().scorpion_max_children);
			energy = energy - nmb_offspring * getAppConfig().scorpion_energy_loss_female_per_child;
			compteur_gestation = getAppConfig().scorpion_gestation_time;
			compteur_mating = getAppConfig().animal_mating_time;
			
			break;
		}
    
		case false: {
			pregnant = false;
			energy = energy - getAppConfig().scorpion_energy_loss_mating_male;
			compteur_mating = getAppConfig().animal_mating_time;
			mate -> makePregnant(true);
			
			break;
		}
    

		default: {
			return false;
		}
		}
	}
}
*/



void Scorpion::give_birth(){
	for(unsigned int i(0); i < nmb_offspring; ++i){
			getAppEnv().addEntity(new Scorpion (position));
	}
}
	

void Scorpion::AnalyseEnvironment()
{
	dinner = nullptr;
	crush = nullptr;
    predators.clear();
    dinner_list.clear();
    crush_list.clear();

    for(auto entity : getAppEnv().getEntitiesInSightForAnimal(this)) {
        if(entity != nullptr and eatable(entity)) {   
            dinner_list.push_back(entity); //Maybe something has to be changed concerning the constructor of copies
            
		}else if(entity != nullptr and matable(entity) and entity -> matable(this)){
            crush_list.push_back(entity);
		}
			
		}/*else if(entity != nullptr and eatableBy(entity)){
            	predators.push_back(entity -> getPosition());
        }*/
            

        
        dinner = chooseNearestEntity(dinner_list);
        crush = chooseNearestEntity(crush_list);
}


/*
void Scorpion::AnalyseEnvironment()
{
	delete dinner;
	dinner = nullptr;
	
	for(auto entity : getAppEnv().getEntitiesInSightForAnimal(this)) {
        if((eatable(entity)) and (isEntityNearest(entity))) {
            dinner = entity;
		}else if((eatableBy(entity)) and (isEntityNearest(entity))){
            predator = entity;
        }else if((mateable()) and (isEntityNearest(entity))){
			crush = entity;
		}
	}
}
*/

//Dessin polymorphique

void Scorpion::drawVision(sf::RenderTarget& targetWindow) const
{
    Animal::drawVision(targetWindow);
}

void Scorpion::drawAnnulus(sf::RenderTarget& targetWindow) const
{
    Animal::drawAnnulus(targetWindow);
}

void Scorpion::draw(sf::RenderTarget& targetWindow) const
{
    Animal::draw(targetWindow);
}



//Constructeurs

Scorpion::Scorpion(Vec2d posit, double niv_enrg, bool f_gender)
    :Animal(posit, getSize(), niv_enrg, 0, Vec2d(1,0),
            Vec2d(0,0), Vec2d(1,0), f_gender)
{}

Scorpion::Scorpion(Vec2d posit)
    :Animal(posit,getSize(), getInitialEnergy(),
            0, Vec2d(1,0), Vec2d(0,0), Vec2d(1,0), bool(uniform(0,1) == 1))
{}



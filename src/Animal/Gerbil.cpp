#include "Gerbil.hpp"
#include <iostream>
#include <Application.hpp>
#include <Config.hpp>
#include <cmath>
#include <Random/Uniform.hpp>

//Getters

double Gerbil::getStandardMaxSpeed() const
{
    return getAppConfig().gerbil_max_speed;
}

double Gerbil::getMass() const
{
    return getAppConfig().gerbil_mass;
}

double Gerbil::getSize() const
{
    return getAppConfig().gerbil_size;
}

double Gerbil::getInitialEnergy() const
{
    return getAppConfig().gerbil_energy_initial;
}

double Gerbil::getViewRange() const
{
    return getAppConfig().gerbil_view_range;
}

double Gerbil::getViewDistance() const
{
    return getAppConfig().gerbil_view_distance;
}

double Gerbil::getRandomWalkRadius() const
{
    return getAppConfig().gerbil_random_walk_radius;
}

double Gerbil::getRandomWalkDistance() const
{
    return getAppConfig().gerbil_random_walk_distance;
}

double Gerbil::getRandomWalkJitter() const
{
    return getAppConfig().gerbil_random_walk_jitter;
}

std::string Gerbil::getTexture() const
{
    if(femelle) {
        return getAppConfig().gerbil_texture_female;
    } else {
        return getAppConfig().gerbil_texture_male;
    }
}

double Gerbil::getMaxSpeed() const
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

sf::Time Gerbil::getLongevity() const
{
    return getAppConfig().gerbil_longevity;
}

double Gerbil::getFacteurPerteEnergy() const
{
    return getAppConfig().gerbil_energy_loss_factor;
}


double Gerbil::getEnergyLossPerChildF() const
{
	return getAppConfig().gerbil_energy_loss_female_per_child;
}

double Gerbil::getEnergyLossM() const
{
	return getAppConfig().gerbil_energy_loss_mating_male;
}

unsigned int Gerbil::getMinChild() const
{
	return getAppConfig().gerbil_min_children;
}

unsigned int Gerbil::getMaxChild() const
{
	return getAppConfig().gerbil_max_children;
}

float Gerbil::getGestationTime() const
{	
	return getAppConfig().gerbil_gestation_time;
}

double Gerbil::getLossFactor() const
{
	return getAppConfig().gerbil_energy_loss_factor;
}



//Pour ca on pourrait auussi faire un getAppConfig truc
std::string Gerbil::getMatingTexture() const
{
	
	unsigned int counter_draw(1);
	counter_draw = uniform(1,5);
	switch(counter_draw) {
		
		case 1 : {
			return "gpixel1.png";
			break;
		}
		
		case 2 : {
			return "gpixel2.png";
			break;
		}
		
		case 3 : {
			return "gpixel3.png";
			break;
		}
		
		case 4 : {
			return "gpixel4.png";
			break;
		}
		
		case 5 : {
			return "gpixel5.png";
			break;
		}
		
		default : {
			return "gpixel1.png";
		}
	}
}




double Gerbil::getMatingTextureSize() const
{
	return 2.5* rayon;
}


//Hunt or be hunted

bool Gerbil::eatable(OrganicEntity const* other) const
{
    return other->eatableBy(this);
}

bool Gerbil::eatableBy(Scorpion const* scorpion) const
{
    return true;
}

bool Gerbil::eatableBy(Gerbil const* gerbil) const
{
    return false; //could be fun to turn them into cannibals(against other dying gerbils with low energy) if food is low)
}

bool Gerbil::eatableBy(Food const* food) const
{
    return false;
}

 
 
//Do it like they do it on DiscoveryChannel o/ o/
bool Gerbil::matable(OrganicEntity const* other) const{
	return other -> canMate(this);
}

bool Gerbil::canMate(Scorpion const* scorpion) const{
	return false;
}

//Do not think that const in the parameter is really necessary but I'll leave it how it is because it works
bool Gerbil::canMate(Gerbil const* gerbil) const{
	if (gerbil -> getAge() >= getAppConfig().gerbil_min_age_mating){
		

		if(femelle){
			if(not gerbil -> isFemale() and 
			gerbil -> getEnergy() >= getAppConfig().gerbil_energy_min_mating_male and
			this -> getEnergy() >= getAppConfig().gerbil_energy_min_mating_male and
			not this -> isPregnant()){
				return true;
			}
			
		}
    
		else{
			if(gerbil -> isFemale() and not gerbil -> isPregnant() and
			gerbil -> getEnergy() >= getAppConfig().gerbil_energy_min_mating_female and
			this -> getEnergy() >= getAppConfig().gerbil_energy_min_mating_female){
				return true;
			}
		}
	}
	//std::cerr << "Error not female or male " << std::endl;
	return false;
}

bool Gerbil::canMate(Food const* food) const{
	return false;
}


	
/*In case off a much more differentiated meet method we could define this method as virtually pure in the class Animal and redefinde them in each corresponding Animal
 *//*
void Gerbil::meet(OrganicEntity* mate){  //The switch takes as variable the gender of the current instance. If it's female it's gonna get pregnant.
	if(matable(mate) and mate -> matable(this)){
		
		switch(femelle) {

		case true: {
			
			pregnant = true;
			nmb_offspring = uniform(getAppConfig().gerbil_min_children, getAppConfig().gerbil_max_children);
			energy = energy - nmb_offspring * getAppConfig().gerbil_energy_loss_female_per_child;
			compteur_gestation = getAppConfig().gerbil_gestation_time;
			compteur_mating = getAppConfig().animal_mating_time;
			
			break;
		}
    
		case false: {
			pregnant = false;
			energy = energy - getAppConfig().gerbil_energy_loss_mating_male;
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



void Gerbil::give_birth(){
	for(unsigned int i(0); i < nmb_offspring; ++i){
			getAppEnv().addEntity(new Gerbil (position));
	}
	nmb_offspring = 0;
}



void Gerbil::AnalyseEnvironment()
{

    for(auto entity : getAppEnv().getEntitiesInSightForAnimal(this)) {
        if(entity != nullptr and eatable(entity)) {   
            dinner_list.push_back(entity); //Maybe something has to be changed concerning the constructor of copies
            
		}else if(entity != nullptr and matable(entity) and entity -> matable(this)){
              crush_list.push_back(entity);
              
		}if(entity != nullptr and entity -> eatable(this))
            	predators.push_back(entity -> getPosition());   
        }
        
        dinner = chooseNearestEntity(dinner_list);
        crush = chooseNearestEntity(crush_list);
        if(not predators.empty()){
			current_predator = chooseNearestPosition(predators);
		}
        
}




/*
void Gerbil::AnalyseEnvironment()
{
	//delete dinner;
	//dinner = nullptr;
	
	for(auto entity : getAppEnv().getEntitiesInSightForAnimal(this)) {
        if(entity != nullptr and eatable(entity) and isEntityNearest(entity)) {
            dinner = entity;
		}else if((eatableBy(entity)) and (isEntityNearest(entity))){
            predator = entity;
        }else if((matable) and (isEntityNearest(entity))){
			crush = entity;
		}
	}
}
*/


//Dessin polymorphique

void Gerbil::drawVision(sf::RenderTarget& targetWindow) const
{
    Animal::drawVision(targetWindow);
}

void Gerbil::drawAnnulus(sf::RenderTarget& targetWindow) const
{
    Animal::drawAnnulus(targetWindow);
}

void Gerbil::draw(sf::RenderTarget& targetWindow) const
{
    Animal::draw(targetWindow);
}



//Constructeur

Gerbil::Gerbil(Vec2d posit, double niv_enrg, bool f_gender)
    :Animal(posit, getSize(), niv_enrg, 0, Vec2d(1,0),
            Vec2d(0,0), Vec2d(1,0), f_gender)
{}

Gerbil::Gerbil(Vec2d posit)
    :Animal(posit, getSize(), getInitialEnergy(),
            0, Vec2d(1,0), Vec2d(0,0), Vec2d(1,0), bool(uniform(0,1) == 1))
{}


//Idea for optional work
//When a new gerbil is born print a texture with text that says Hello world or something else





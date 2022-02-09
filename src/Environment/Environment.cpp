#include "Environment.hpp"
#include <Application.hpp>
#include <Animal/Animal.hpp>
#include "OrganicEntity.hpp"
#include <Environment/Wave.hpp>
#include <Obstacle/Rock.hpp>
#include <Obstacle/Obstacle.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <algorithm>



//ajouts à l'environnement
void Environment::addEntity(OrganicEntity* entity)
{
    faune.push_back(entity);
}

void Environment::addGenerator(FoodGenerator* g)
{
    generateurs.push_back(g);
}

void Environment::addWave(Wave* onde)
{
	ondes.push_back(onde);
	std::cout << "  wave added  " << std::endl;
}

void Environment::addObstacle(Obstacle* obs)
{
	obstacles.push_back(obs);
	std::cout << "  rock added  " << std::endl;
}

/*
void Environment::removeDead()
{
	for(auto ptr_entite : faune){
		if (ptr_entite->IsDead()){
			delete ptr_entite;
			ptr_entite = nullptr;
		}
	}
}*/


void Environment::update(sf::Time dt)
{


    for(auto& entity : faune) {

        if (entity != nullptr and entity->IsDead()) {
            delete entity;
            entity = nullptr;
            std::cout << "another one bites the dust" << std::endl;
        }
        
        if(entity != nullptr){
            entity->update(dt);
		}
    }

    faune.erase(std::remove(faune.begin(), faune.end(), nullptr), faune.end());

    for(auto generateur : generateurs) {
        generateur->update(dt);
    }
	
	for(auto& onde : ondes){
		
		if(onde != nullptr and onde->IsWeak()){
			delete onde;
			onde = nullptr;
			std::cout << "  ONDE DETRUITE  " << std::endl;
		}
		
		if(onde != nullptr){
			onde->updateWave(dt, obstacles);		
		}
	}
	
	ondes.erase(std::remove(ondes.begin(), ondes.end(), nullptr), ondes.end());
	
	//obstacles.erase(std::remove(obstacles.begin(), obstacles.end(), nullptr), obstacles.end());
}

void Environment::draw(sf::RenderTarget& targetWindow) const     //dessine cibles de l'env. ds targetWindow
{

    //dessiner les animaux de la faune ici:
    for(auto ptr_entity : faune) {
        ptr_entity->draw(targetWindow);
    }
    
    for(auto onde: ondes){
		onde->drawWave(targetWindow);
	}

	for(auto obs : obstacles){
		
		obs->drawObstacle(targetWindow);
	}
}


void Environment::clean()
{
    //ens_target.clear();

    for(auto& entity : faune) {
        delete entity;    //supprime contenu pointé
        entity = nullptr; //met adr dans pointeur a nullptr
    }
    faune.clear(); //detruit tableau
    
    for(auto& generateur : generateurs) {
		delete generateur;    //supprime contenu pointé
		generateur = nullptr; //met adr dans pointeur a nullptr
    }
    generateurs.clear(); //detruit tableau
    
    for(auto& onde : ondes) {
		delete onde;    //supprime contenu pointé
		onde = nullptr; //met adr dans pointeur a nullptr
    }
    ondes.clear(); //detruit tableau
    
    for(auto& obstacle : obstacles) {
		delete obstacle;    //supprime contenu pointé
		obstacle = nullptr; //met adr dans pointeur a nullptr
    }
    obstacles.clear(); //detruit tableau    
}


Environment::~Environment()
{
	clean();	
}



Ret_entities Environment::getEntitiesInSightForAnimal(Animal const* indv) const
{
    Ret_entities ret;
    for(auto ptr_entity : faune) { // test 16 modif
        if ((indv -> isEntityInSight(ptr_entity)) and (ptr_entity != nullptr)) {
            ret.push_back(ptr_entity);
		}if(ptr_entity == nullptr){
			//std::cout << "pomme" << std::endl;

        }
    }
    return ret;
}
//The return of pointers on organic entities allows one to use the function getEntitiesInSightForAnimal to get access to some organic entities and even get the right to change them or to delete them.
//Therefore this is not very good for the encapsulation.


double Environment::calcIntensity(Vec2d pos) const
{	
	double ret(0);
	for(auto ptr_onde : ondes){
		if(ptr_onde -> IsCollidingWithPosition(pos)){
			ret += ptr_onde -> getIntensity();
		}
	}
	return ret;
}
			






//Peutetre reutilisable

/*
void Environment::addTarget(const Vec2d& t)
{
	ens_target.push_back(t);
}


possible_targets Environment::getTargetinSightForAnimal(Animal* indv){		//J'utilise des pointeur pq plus tard les cibles seront des animaux aussi
	possible_targets ret({0});
	for(auto target : ens_target){
		if (indv -> isTargetInSight(target)){
			ret.push_back(target);
			*



void Environment::draw(sf::RenderTarget& targetWindow)//dessine cibles de l'env. ds targetWindow
{

	//dessiner les animaux de la faune ici:
	for(auto ptr_entity : faune){
		ptr_entity->draw(targetWindow);

	}


	for(auto target : ens_target){
		auto circle (buildCircle(target, 5.0, sf::Color::Red));
		targetWindow.draw(circle);
		//std::cout << "circle drawn" << std::endl;
	}

}
*/

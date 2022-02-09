#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include <Utility/Vec2d.hpp>
#include <Utility/Utility.hpp>
#include <list>
#include "Environment/FoodGenerator.hpp"
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>

class Animal;
class OrganicEntity;
class Wave;
class Obstacle;

typedef std::list<OrganicEntity*> Ret_entities;

class Environment : public Updatable
{

public:

	//ajouts à l'environnement
    void addEntity(OrganicEntity* entity); //peupler l'env
    //void addTarget(const Vec2d& t); //placer des ressources (food)
    void addGenerator(FoodGenerator* g);
    void addWave(Wave* onde);
    void addObstacle(Obstacle* obs);

    //void removeDead(); //met le pointeur à nullptr (mais ne le supprime pas de la liste)

    void update(sf::Time dt) override; //fait évoluer tous les animaux à chaque pas de temps
    void draw(sf::RenderTarget& targetWindow) const; //dessine cibles ds env(+ animaux later)
    void clean(); //vide l'env de tout animal ou cible
    Ret_entities getEntitiesInSightForAnimal(Animal const* indv) const;
    double calcIntensity(Vec2d pos) const;

    //constructeur par défaut:
    // par défaut

    //constructeur:
    Environment() = default;

    //destructeur: (pour les pointeurs sur animaux)
    ~Environment();

    //un environnement n'est pas copiable:
    Environment(Environment const&) = delete;

private:
						
    std::list<OrganicEntity*> faune; //faune = scorpion + gerbil + food
    //std::list<Vec2d> ens_target;//ens des cibles recensées (food)
    std::list<FoodGenerator*> generateurs;
    std::list<Wave*> ondes;
    std::list<Obstacle*> obstacles;

};

#endif

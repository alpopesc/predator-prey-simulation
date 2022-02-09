#ifndef ANIMAL_H
#define ANIMAL_H

#include <iostream>

#include <Utility/Constants.hpp>
#include <Utility/Utility.hpp>
#include <Environment/OrganicEntity.hpp>
#include <vector>
#include <list>

enum State {
    FOOD_IN_SIGHT, // nourriture en vue
    FEEDING,       // en train de manger (là en principe il arrête de se déplacer)
    RUNNING_AWAY,  // en fuite
    MATE_IN_SIGHT, // partenaire en vue
    MATING,        // vie privée (rencontre avec un partenaire!)
    GIVING_BIRTH,  // donne naissance
    WANDERING,     // déambule

};

enum Deceleration {forte= 9, moy = 6, faible = 3};
//ne pas oublier de multiplier par 0.1

class Animal : public OrganicEntity
{

public:

    //Constructeur et destructeur
    Animal(Vec2d posit,double a_size, double enrg, double vit = 0, Vec2d dir = Vec2d(1,0),
           Vec2d trg = Vec2d(0,0), Vec2d c_trg = Vec2d(1,0), bool gender_f = false);	//can't have non-default parameters after default parameters start. Therefore   I initialized the gneder as 0 by default because inn the description it says it should be the last parameter.
   
    ~Animal();


    //methodes virtuelles pures redéfinies par Scorpion et Gerbil
    virtual double getStandardMaxSpeed() const = 0;
    virtual double getMass() const = 0;
    virtual double getSize() const = 0;
    virtual double getInitialEnergy() const = 0;
    virtual double getViewRange() const = 0;
    virtual double getViewDistance() const = 0;
    virtual double getRandomWalkRadius() const = 0;
    virtual double getRandomWalkDistance() const = 0 ;
    virtual double getRandomWalkJitter() const = 0;
    virtual std::string getTexture() const = 0;
    virtual double getMaxSpeed() const = 0;
    virtual double getEnergyLossPerChildF() const = 0;
	virtual double getEnergyLossM() const = 0;
	virtual unsigned int getMinChild() const = 0;
	virtual unsigned int getMaxChild() const = 0;
	virtual float getGestationTime() const = 0;
	virtual double getLossFactor() const = 0;
	
	//Little Bonus
	/* Scorpion should be getting a beating heart and 
	 * Gerbils environment get pixelated
	 */
	virtual std::string getMatingTexture() const =0;
	virtual double getMatingTextureSize() const =0;
     
    //SOME GETTERS
    Vec2d getSpeedVector() const;
    bool isFemale() const;
    bool isPregnant() const;
    bool isEntityInSight(OrganicEntity* cible) const;
    double getRotation() const;  //retourne angle polaire de directtion
    
									
    //gestion energy
    double getDepenseEnergyBase() const;
    virtual double getFacteurPerteEnergy() const = 0;
    
    
    protected:
    void perte_energie(sf::Time dt);
     
    //Time intervals getters
    public:
    float getMatingTime() const;
    float getDeliveryTime() const;

    
    //Important Methods for update State
    protected:
    Vec2d chooseNearestPosition(const std::vector<Vec2d>& pos_entities); 
	OrganicEntity* chooseNearestEntity(const std::list<OrganicEntity*>& entities);
	bool isEntityNearest(OrganicEntity const* entity) const; //Not really used this method so it is not that important
    virtual void AnalyseEnvironment() = 0; //It is virtual because everey animal could analyse completly different the the others Scorpion for exemple can sense seismic waves in the gorubd which are impossible for the mices
    virtual void give_birth() = 0; // Every Animal gives birth differently
    
    public:
    void meet(OrganicEntity* mate); // For a more differentiated meetin make this method virtual
    virtual void updateState(sf::Time dt); //Changes state and is responsible for the updating of all the counters to put it easy.
    virtual void update(sf::Time dt); // Resposible for the movment and when which force is calculated
    
    /*Note worthy anout this to methods is also the prioroties:
     * The hisghest priority has the state giving Birth since when giving
     * birht the animal is too exhausted to change to another state
     * Second highest ist running away in order to ensure survival
     * Third highest is Mating then comes mate in sight, then Feeding etc.
     * UPDATE job is to calculate the forces and update the position of
     * the animal
     */
    
    
    protected:
    //Force and Movment
    Vec2d breakingForce();
    Vec2d force_attrac();
    Vec2d force_attrac_p(Vec2d parameter); //Is th inverse of force_attrac and has also a parameter
    Vec2d force_fuite();
    void maj_deplacement(Vec2d force, sf::Time dt);
    void setRotation(double angle); //met à jour direction pour une rot de "angle"

    Vec2d ConvertToGlobalCoord(Vec2d local) const;
    Vec2d randomWalk(); 
    
    /* random walk uses a virtual target that changes randomly and also depending
     * on randomWalkJitter constant and  calculates the force.
     */
    
    
    //Drawing
    public:
    virtual void draw(sf::RenderTarget& targetWindow) const;
    
    protected:
    void drawMating(sf::RenderTarget& targetWindow) const;
    virtual void drawDebugText(sf::RenderTarget& targetWindow) const override;
    virtual void drawVision(sf::RenderTarget& targetWindow) const;
    virtual void drawAnnulus(sf::RenderTarget& targetWindow) const;
    void drawPregnancyAnnulus(sf::RenderTarget& targetWindow) const;
    
    private:
    void setTargetPosition(Vec2d new_targ); //Did not really use the method. Put it in private in order to assure encapsulation


protected:

	double vitesse; //norme vitesse
	Vec2d direction;
    Vec2d target; //vecteur de l'animal a la cible
    Vec2d current_target; //pour cible virtuelle
    bool femelle; //si c'est femelle alors la variable represente 1. Sinon 0
    OrganicEntity* dinner;
	OrganicEntity* crush;
	Vec2d current_predator; //Last known position of the nearest predator. This attribut was not really used but could come in handy later.
    

    bool pregnant;
    unsigned int nmb_offspring; // Don't forget the constructer
    State state; //Il faut encore faire adapter le constructeur pour ca ??
	sf::Time compteur_feeding; //temps de pause
	sf::Time compteur_mating;
	sf::Time compteur_gestation;
	sf::Time compteur_givingBirth;

	


    //These are the position of the entities that appear in sight for the current entity.
    //We know that it is not necessary make a vector of food positions but it could come in handy if we decide to not choose the food with the shortest distance but that choose
    //the food whose mean_distance to other foods in the sight of the animal is the smallest. Like this the animal chooses not only the food with shortest distance but also the bigger quantity of food.
    std::vector <Vec2d> predators;			//Current Predators that are in sight of the Animal
    std::vector <Vec2d> predators_memory;	//The position of the predator that the animal remembers
    std::list <OrganicEntity*> dinner_list;
    std::list <OrganicEntity*> crush_list;


	
	protected: //Used for the mating animation
	int counter_draw;	

									
};



#endif

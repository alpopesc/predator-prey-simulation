#ifndef SCORPION_H
#define SCORPION_H

#include <iostream>
#include <Animal/Animal.hpp>

class Scorpion : public Animal
{
public :

    //Construceurs
    Scorpion(Vec2d posit, double niv_enrg, bool f_gender);
    Scorpion(Vec2d posit);
    
protected:

    //Getters for the polymorphic behaviour
    double getStandardMaxSpeed() const override;
    double getMass() const override;
    double getSize() const override;
    double getInitialEnergy() const override;
    double getViewRange() const override;
    double getViewDistance() const override;
    double getRandomWalkRadius() const override;
    double getRandomWalkDistance() const override;
    double getRandomWalkJitter() const override;
    std::string getTexture() const override;
    double getMaxSpeed() const override;
    sf::Time getLongevity() const override;
    double getFacteurPerteEnergy() const override;
    double getEnergyLossPerChildF() const override;
	double getEnergyLossM() const override;
	unsigned int getMinChild() const override;
	unsigned int getMaxChild() const override;
	float getGestationTime() const override;			//Thes are not the counter but rather the constants in the json file
	double getLossFactor() const override;
	
	std::string getMatingTexture() const override;
	double getMatingTextureSize() const override;
	
public:	
    //Hunt or be hunted :)
    bool eatable(OrganicEntity const* other) const override;
    bool eatableBy(Scorpion const* scorpion) const override;
    bool eatableBy(Gerbil const* gerbil) const override;
    bool eatableBy(Food const* food) const override;
    
    //Do it like they do it on DiscoveryChannel o/ o/
    bool matable(OrganicEntity const* other) const override;
	bool canMate(Scorpion const* scorpion) const override;
	bool canMate(Gerbil const* gerbil) const override;
	bool canMate(Food const* food) const override;
  
    virtual void AnalyseEnvironment() override;
    //void meet(OrganicEntity* mate) override;
    void give_birth() override;



    //Drawing
    void draw(sf::RenderTarget& targetWindow) const override;


private:

    //Drawing
    void drawVision(sf::RenderTarget& targetWindow) const override;
    void drawAnnulus(sf::RenderTarget& targetWindow) const override;



};


#endif

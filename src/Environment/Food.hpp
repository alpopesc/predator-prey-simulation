#ifndef FOOD_H
#define FOOD_H

#include <iostream>
#include <Environment/OrganicEntity.hpp>

class Food : public OrganicEntity
{

public:

    //Constructeur et destructeur
    Food (Vec2d position);
    ~Food();

    //Getters
    double getSize() const;
    double getInitialEnergy() const;
    std::string getTexture() const;
    sf::Time getLongevity() const override;

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
	
	void meet(OrganicEntity* mate) override;
	
	/*We could have put the double dispatch also in Animal. We
	 * We put it in Organic Entity anyway because maybe we could introduce other living beeings that can reproduce too.
	 */

    //Drawing and update
    void draw(sf::RenderTarget& targetWindow) const override;
    void update(sf::Time dt) override;
    void drawDebugText(sf::RenderTarget& targetWindow) const override;
};

#endif

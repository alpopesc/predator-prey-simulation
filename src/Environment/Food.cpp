#include "Food.hpp"
#include <Application.hpp>

//Constructeur
Food::Food(Vec2d position)
    : OrganicEntity(position , getSize(), getInitialEnergy(), sf::Time::Zero)
{}

//Destructeur
Food::~Food()
{}



//Getters
double Food::getSize() const
{
    return getAppConfig().food_size;
}

double Food::getInitialEnergy() const
{
    return getAppConfig().food_energy;
}

std::string Food::getTexture() const
{
    return getAppConfig().food_texture;
}

sf::Time Food::getLongevity() const
{
    return sf::seconds(1E9);
}


//Hunt or be hunted
bool Food::eatable(OrganicEntity const* other) const
{
    return other->eatableBy(this);
}

bool Food::eatableBy(Scorpion const* scorpion) const
{
    return false;
}

bool Food::eatableBy(Gerbil const* gerbil) const
{
    return true;
}

bool Food::eatableBy(Food const* food) const
{
    return false;
}

//Do it like they do it on DiscoveryChannel o/ o/
bool Food::matable(OrganicEntity const* other) const{
	return other -> canMate(this);
}

bool Food::canMate(Scorpion const* scorpion) const{
	return false;
}

bool Food::canMate(Gerbil const* gerbil) const{
	return false;
}

bool Food::canMate(Food const* food) const{
	return false;
}


void Food::meet(OrganicEntity* mate){
	//std::cout << "should not be meeting" << std::endl;
}


//Drawing and Update
void Food::draw(sf::RenderTarget& targetWindow) const
{
    sf::Texture& texture = getAppTexture(getTexture());
    auto image_to_draw(buildSprite(position, rayon, texture));
    targetWindow.draw(image_to_draw);
}

void Food::drawDebugText(sf::RenderTarget& targetWindow) const
{
}

void Food::update(sf::Time dt)
{
    // TODO : Evolution de la source de nourriture à programmer

    //à chaque pas de temps:
    OrganicEntity::update(dt);
}

#include "Rock.hpp"
#include <Environment/Wave.hpp>
#include <Random/Uniform.hpp>
#include <Application.hpp>
#include <iostream>

//Constructeurs et destructeur
/*
Rock::Rock(const Vec2d& _position, double _rayon)
	:Obstacle(_position, _rayon), angle(uniform(-PI, PI))
	{}
*/
Rock::Rock(const Vec2d& _position)
	:Obstacle(_position,
		double(uniform(getAppConfig().simulation_world_size/50, 2*getAppConfig().simulation_world_size/50)))
	{
		angle = uniform(-PI/2.0, PI/2.0);
	} //les rock sont toutes dans la même orientation et de la même taille..

Rock::~Rock()
	{}

//Getter
double Rock::getAngle() const
{
	return angle;
}

std::string Rock::getTexture() const
{
	return getAppConfig().rock_texture;
}

//Dessin
void Rock::drawObstacle(sf::RenderTarget& targetWindow) const
{
	sf::Texture& texture = getAppTexture(getTexture());
    auto image_to_draw(buildSprite(position, 50, texture, angle));
    targetWindow.draw(image_to_draw);
}


/*
//Collision avec des ondes
bool Rock::IsCollidingWithWave(const Wave& wave) const
{
	list<std::pair<double, double>> arcs(wave.getFragments());
	
	for(auto arc : arcs){
		double angle (this->getPosition() - wave.getPosition());
		if((angle >= arc.first) and (angle <= arc.second)){
			return true
		}else{
			return false;
		}
	}
}
*/

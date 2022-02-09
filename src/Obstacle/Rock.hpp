#ifndef ROCK_H
#define ROCK_H

#include <iostream>
#include <Obstacle/CircularCollider.hpp>
#include <Utility/Vec2d.hpp>
#include <Utility/Utility.hpp>
#include <Obstacle/Obstacle.hpp>


class Rock : public Obstacle
{
	public:
	
	//Constructeurs et destructeur
	Rock(const Vec2d& _position);
	//Rock(const Vec2d& _position);
	
	
	
	~Rock();
	
	//Getter
	double getAngle() const override;
	std::string getTexture() const;
	
	//Dessin
	void drawObstacle(sf::RenderTarget& targetWindow) const override;
	
	private:
	
	double angle;
	
};

#endif


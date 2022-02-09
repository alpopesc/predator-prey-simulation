#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <iostream>
#include <Obstacle/CircularCollider.hpp>
#include <Utility/Vec2d.hpp>
#include <Utility/Utility.hpp>


class Obstacle : public CircularCollider
{
	public:
	
	//Constructeur et destructeur
	Obstacle(Vec2d _position, double _rayon);
	virtual ~Obstacle();
	
	//Dessin
	virtual std::string getTexture() const = 0;
	virtual void drawObstacle(sf::RenderTarget& targetWindow) const = 0;
	
	//Autre
	virtual double getAngle() const = 0;
	
};

#endif


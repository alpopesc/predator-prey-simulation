#include "Obstacle.hpp"
#include <Application.hpp>

Obstacle::Obstacle(Vec2d _position, double _rayon)
	:CircularCollider(_position, _rayon)
	{}
	
Obstacle::~Obstacle()
	{}


#include "CircularCollider.hpp"
#include <Application.hpp>
#include <cmath>
#include <Config.hpp>
//throw std::exception("blsablalb") + #include <stdexcept>

//type CircularCollider::nom()

//constructeurs
CircularCollider::CircularCollider(Vec2d une_pos, double un_rayon)
    : position (une_pos), rayon(un_rayon)
{
    clamping();
}
//constr par copie
CircularCollider::CircularCollider(const CircularCollider& autre)
    : position(autre.position), rayon(autre.rayon)
{
}

//getters
const Vec2d& CircularCollider::getPosition() const
{
    return position;
}

double CircularCollider::getRadius() const
{
    return rayon;
}

//opérateur =
void CircularCollider::operator=(const CircularCollider& c)
{
    position = c.position;
    rayon = c.rayon;
}

//algorithme de clamping
void CircularCollider::clamping()
{
    double worldSize = getAppConfig().simulation_world_size;

    if(rayon < 0) {
        throw "rayon négatif";
    }

    while(position.x < 0) {
        position.x += worldSize;
    }
    while(position.x > worldSize) {
        position.x -= worldSize;
    }

    while(position.y < 0) {
        position.y += worldSize;
    }
    while(position.y > worldSize) {
        position.y -= worldSize;
    }
}

Vec2d CircularCollider::directionTo(Vec2d to) const
{

    double worldSize = getAppConfig().simulation_world_size;
    auto width  = worldSize; // largeur
    auto height = worldSize; // hauteur
    Vec2d from(position);
    double new_distance(1000000000000);

    for(int i(-1); i < 2 ; ++i) {
        for(int j(-1); j < 2; ++j) {
            to = to + Vec2d(width*i, height*j);
            if(distance(to, from) < new_distance) {
                new_distance = distance(to, from);
                return to - from;
            }
        }
    }
    return to - from;
}


Vec2d CircularCollider::directionTo(CircularCollider a)const
{
    Vec2d to(a.getPosition());
    return directionTo(to);
}



double CircularCollider::distanceTo(Vec2d to) const
{
    return directionTo(to).length();
}


double CircularCollider::distanceTo(const CircularCollider& a) const
{
    Vec2d to(a.getPosition());
    return distanceTo(to);
}


void CircularCollider::move(Vec2d dx)
{
    /*Vec2d a;
    a = position + dx;
    setPosition(a);
    clamping();
    */
    position += dx;
    clamping();
}

/*
 * This method somehow does not work for Test18 so I coded it again.
 *
bool CircularCollider::isColliding(const CircularCollider& other) const
{
    if(distanceTo(other.position) <= (this->rayon + other.rayon)) {
        return true;
    } else {
        return false;
    }
}
*/

bool CircularCollider::isColliding(const CircularCollider& other) const
{
    if(distance(position, other.getPosition()) <= (rayon + other.getRadius())) {
        return true;
    } else {
        return false;
    }
}

bool CircularCollider::isPointInside(const Vec2d& point) const
{
    if(distanceTo(point) <= this->rayon) {
        return true;
    } else {
        return false;
    }
}

bool CircularCollider::operator>(const CircularCollider& other) const
{
    if(isCircularColliderInside(other)) {
        return true;
    } else {
        return false;
    }
}
bool CircularCollider::operator|(const CircularCollider& other) const
{
    if(isColliding(other)) {
        return true;
    } else {
        return false;
    }
}

bool CircularCollider::isCircularColliderInside(const CircularCollider& other) const
{
    if((other.getRadius() < rayon) and distanceTo(other)  < abs(rayon - other.getRadius())) {
        return true;
    }
    return false;
}


bool CircularCollider::operator>(Vec2d point) const
{
    if(distanceTo(point) < rayon) {
        return true;
    }
    return false;
}

void CircularCollider::drawCollisionZone(sf::RenderTarget& targetWindow) const
{

    auto circle (buildCircle(position, getRadius(), getAppConfig().debug_text_color));
    targetWindow.draw(circle);

}



std::ostream& operator<<(std::ostream& cout, const CircularCollider& a)
{
    cout << "CircularCollider: position =  " << a.getPosition() << "    radius = " << a.getRadius() << endl;
    return cout;
}

#include <Application.hpp>
#include <iostream>
#include <Utility/Vec2d.hpp>

#ifndef CIRCULAR_COLLIDER_H
#define CIRCULAR_COLLIDER_H

using namespace std;

class CircularCollider
{

public:

    CircularCollider(Vec2d une_pos, double un_rayon);
    CircularCollider(const CircularCollider& autre);
    const Vec2d& getPosition() const;
    double getRadius() const;
    void operator=(const CircularCollider& c);
    void clamping();




    //gestion du movment

    Vec2d directionTo(Vec2d to) const;
    Vec2d directionTo(CircularCollider a)const;
    double distanceTo(Vec2d to) const;
    double distanceTo(const CircularCollider& a) const;
    void move(Vec2d dx);


    //gestion des collisions

    bool isCircularColliderInside(const CircularCollider& other) const;
    bool operator>(Vec2d point) const;

    bool isColliding(const CircularCollider& other) const;
    bool isPointInside(const Vec2d& point) const;
    bool operator>(const CircularCollider& other) const;
    bool operator|(const CircularCollider& other) const;
    void drawCollisionZone(sf::RenderTarget& targetWindow) const;


protected:

    Vec2d position;
    double rayon;
};


#endif
















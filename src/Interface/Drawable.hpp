/*
 * infosv
 * 2012-19
 * Marco Antognini
 */

#ifndef INFOSV_DRAWABLE_HPP
#define INFOSV_DRAWABLE_HPP

#include <SFML/Graphics.hpp>

/*!
 * @class Drawable
 *
 * @brief Represents an entity that can be represented graphically
 */

class Drawable
{
public:
    virtual ~Drawable()
    {
        /* Default virtual dtor */
    }

    virtual void draw(sf::RenderTarget& target) const = 0;
    virtual void drawDebugText(sf::RenderTarget& target) const = 0;
};

#endif // INFOSV_DRAWABLE_HPP


/*
 * infosv
 * 2012-19
 * Marco Antognini
 */

#ifndef INFOSV_UPDATABLE_HPP
#define INFOSV_UPDATABLE_HPP

#include <SFML/System.hpp>

/*!
 * @class Updatable
 *
 * @brief Represents an entity that evolves over time
 */
class Updatable
{
public:
    virtual ~Updatable()
    {
        /* Default virtual dtor */
    }

    virtual void update(sf::Time dt) = 0;
};

#endif // INFOSV_UPDATABLE_HPP


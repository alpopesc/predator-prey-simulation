/*
 * infosv
 * feb 2013
 * Marco Antognini
 */

#ifndef INFOSV_VEC2D_HPP
#define INFOSV_VEC2D_HPP

#include <SFML/System.hpp>
#include <Utility/Constants.hpp>
/*!
 * @class Vec2d
 *
 * @brief Bridge vector class to sf::Vector2i/f that provide some commom math methods.
 *
 * It can be implicitely constructed from a sf::Vector2i or sf::Vector2f
 * and can be converted implictely to those same types.
 */
class Vec2d
{
public:
    Vec2d();
    Vec2d(double x, double y);

    Vec2d(sf::Vector2f const& sfvect);
    Vec2d(sf::Vector2i const& sfvect);

    Vec2d(Vec2d const& other);

    // We don't need :
    //    Vec2d& operator=(sf::Vector2f const& sfvect);
    //    Vec2d& operator=(sf::Vector2i const& sfvect);
    // Because C++ will automatically construct a Vec2d
    // with the appropriate constructor and the following
    // assignement operator will be called :

    Vec2d& operator=(Vec2d const& other);

    operator sf::Vector2f() const;
    operator sf::Vector2i() const; // TOOD is it a good idea to lose the precision ?

    /*!
     * @brief Compute the length of the vector (squarred)
     *
     * @return the square module of this
     */
    double lengthSquared() const;

    /*!
     * @brief Compute the length of te vector
     *
     * @return the module of this
     */
    double length() const;

    /*!
     * @brief Compute the normilsed vector
     *
     * @return w such that |w| and w * |this| = this
     */
    Vec2d normalised() const;

    /*!
     * @brief Compute the normal (orthogonal) vector
     *
     * @return n such that this · n = 0
     */
    Vec2d normal() const;

    /*!
     * @brief Compute the angle of this in polar coordinates
     *
     * @return the angle of this, in [-PI, PI]
     */
    double angle() const;

    /*!
     * @brief Compute the dot product
     *
     * @param other another vector
     * @return the inner product
     */
    double dot(Vec2d const& other) const;

    /*!
     * @brief Compare two vectors' angle
     *
     * @return 1 if other is clockwise of this vector, -1 otherwise (anticlockwise),
     *         or 0 if other is null or equal to this vector.
     */
    int sign(Vec2d const& other) const;

    Vec2d operator-() const; ///< Negation
    Vec2d operator-(Vec2d const& b) const;
    Vec2d operator+(Vec2d const& b) const;
    Vec2d operator*(double c) const;
    Vec2d operator/(double c) const;

    Vec2d& operator-=(Vec2d const& b);
    Vec2d& operator+=(Vec2d const& b);
    Vec2d& operator*=(double c);
    Vec2d& operator/=(double c);

    bool operator==(Vec2d const& b) const;
    bool operator!=(Vec2d const& b) const;

    /*!
     * @brief Index access, read-write
     *
     * @param axis only value 0 and 1 are allowed
     * @return x if axis is 0, y if axis if 1, undefined otherwise
     */
    double& operator[](int axis);

    /*!
     * @brief Index access, read-only
     *
     * @param axis only value 0 and 1 are allowed
     * @return x if axis is 0, y if axis if 1, undefined otherwise
     */
    double operator[](int axis) const;

public:
    double x, y; ///< DATA
};


Vec2d operator*(double c, Vec2d const& a);


/*!
 * @brief Compute the distance between two given points
 *
 * @param x a point
 * @param y another point
 * @param the distance between x and y
 */
double distance(Vec2d const& x, Vec2d const& y);

/*!
 * @brief Compute the normal vector of a segment
 *
 * @param a beginning of the segment
 * @param b end of the segment
 * @return normal vector of [a, b]
 */
Vec2d normal(Vec2d const& a, Vec2d const& b);

std::ostream& operator<<(std::ostream& out, Vec2d const& v);

#endif // INFOSV_VEC2D_HPP


/*
 * infosv
 * 2013-2019
 * Marco Antognini & Jamila Sam
 */

#ifndef INFOSV_NEURONAL_TEST_HPP
#define INFOSV_NEURONAL_TEST_HPP

#include <Application.hpp>
#include <Obstacle/CircularCollider.hpp>

//typedef CircularCollider Obstacle;

/*!
 * Test the neuronal scorpion.
 *
 * Generate one wave on left click, or continuous waves when right clic is kept pressed
 */
class NeuronalTest : public Application
{
public:
    NeuronalTest(int argc, char const** argv)
        : Application(argc, argv)
    {
    }

    virtual void onEvent(sf::Event event, sf::RenderWindow& window) final override;
    virtual void onSimulationStart() override final;

protected:
    std::vector<std::string> getHelperText() const final override;
private:
    void newWave(Vec2d const& cursor);
    void newObstacle(Vec2d const& cursor);
};

#endif // INFOSV_NEURONAL_TEST_HPP


/*
 * Projet 2019 ssv
 * 203-2019
 * Marco Antognini & Jamila Sam
 */

#include "NeuronalTest.hpp"
#include <Utility/Utility.hpp>
#include <Config.hpp>
#include <Environment/Wave.hpp>
#include <Obstacle/Rock.hpp>
#include <Animal/NeuronalScorpion/NeuronalScorpion.hpp>

IMPLEMENT_MAIN(NeuronalTest)


void NeuronalTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        switch (event.mouseButton.button) {
        case sf::Mouse::Button::Left:
            newWave(getCursorPositionInView());
            break;

        case sf::Mouse::Button::Right:
            newObstacle(getCursorPositionInView());
            break;

        default:
            break;
        }
    }

    if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
        case sf::Keyboard::R: {
            getAppEnv().clean();
            double position(getAppConfig().simulation_world_size / 2);
            getAppEnv().addEntity(new NeuronalScorpion({position, position}));
        }

        break;
        default:
            break;
        }
    }
}


void NeuronalTest::onSimulationStart()
{
    setSimulationMode(SimulationMode::NEURONAL);
    double position(getAppConfig().simulation_world_size / 2);
    getAppEnv().addEntity(new NeuronalScorpion({position, position}));
}

void NeuronalTest::newObstacle(Vec2d const& cursor)
{
    Obstacle* obstacle = new Rock(cursor);
    getEnv().addObstacle(obstacle);
}

void NeuronalTest::newWave(Vec2d const& cursor)
{
    Wave* wave = new Wave(cursor,
                          getAppConfig().wave_default_energy,
                          getAppConfig().wave_default_radius,
                          getAppConfig().wave_default_mu,
                          getAppConfig().wave_default_speed);
    getEnv().addWave(wave);
}

std::vector<std::string> NeuronalTest::getHelperText() const
{
   	return {    "---------------------",
			"(all entities added at ",
			  " mouse position)",
                "Left mouse button   : add wave",
                "Right mouse button   : add ",
                " obstacle",
     			"N   : add neuronal scorpion",
                "Esc : End of program",
                "C   : Reload config file",
                "D   : Toggle debug mode",
                "Z   : Zoom",
                "->  : Move view to right",
                "<-  : Move view to left",
                "Space : Pause"
           };
}

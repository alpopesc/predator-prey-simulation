/*
 * infosv
 * jan 2012
 * Marco Antognini
 */

#include "WaveTest.hpp"
#include <Utility/Utility.hpp>
#include <Config.hpp>
#include <Environment/Wave.hpp>
#include <Obstacle/Rock.hpp> // UNCOMMENT ME WHEN Rock IS CODED

IMPLEMENT_MAIN(WaveTest)


void WaveTest::onEvent(sf::Event event, sf::RenderWindow&)
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
}

void WaveTest::newObstacle(Vec2d const& cursor)
{
    // UNCOMMENT ME WHEN Rock AND addObstacle ARE CODED:
    
    Obstacle* obstacle = new Rock(cursor);
    getEnv().addObstacle(obstacle);
    
}

void WaveTest::newWave(Vec2d const& cursor)
{
    Wave* wave = new Wave(cursor,
                          getAppConfig().wave_default_energy,
                          getAppConfig().wave_default_radius,
                          getAppConfig().wave_default_mu,
                          getAppConfig().wave_default_speed); //commenté au test22: notre constructeur prend moins d'arguments
    getEnv().addWave(wave);
}

std::vector<std::string> WaveTest::getHelperText() const
{
    return {    "---------------------",
                "(MP : mouse position)",
                "Left mouse button   : add wave",
                "                    at MP",
                "Right mouse button   : add ",
                "                 obstacle at MP",
                "Esc : End of program",
                "C   : Reload config file",
                "D   : Toggle debug mode",
                "Z   : Zoom",
                "->  : Move view to right",
                "<-  : Move view to left",
                "Space : Pause"
           };
}

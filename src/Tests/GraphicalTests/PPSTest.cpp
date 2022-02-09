/*
 * infosv
 * jan 2012-19
 * Marco Antognini & Jamila Sam
 */

#include "PPSTest.hpp"
#include <Animal/Gerbil.hpp>
#include <Animal/Scorpion.hpp>
#include <Environment/Food.hpp>
//#include <Environment/FoodGenerator.hpp>

IMPLEMENT_MAIN(PPSTest)

void PPSTest::onRun()
{
}
void PPSTest::onEvent(sf::Event event, sf::RenderWindow& window)
{
    Application::onEvent(event, window);
    onEventPPS(event, window);
}


void PPSTest::onUpdate(sf::Time dt)
{
    Application::onUpdate(dt);
}

void PPSTest::onSimulationStart()
{
    setSimulationMode(SimulationMode::PPS);
    // IMPORTANT: UNCOMMENT WHEN CLASS FOODGENERATOR IS CODED:
    getAppEnv().addGenerator(new FoodGenerator());
}

void PPSTest::onEventPPS(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        case sf::Keyboard::S:
            getAppEnv().addEntity(new Scorpion(getCursorPositionInView()));
            break;

        case sf::Keyboard::G:
            getAppEnv().addEntity(new Gerbil(getCursorPositionInView()));
            break;

        case sf::Keyboard::F:
            // IMPORTANT: UNCOMMENT WHEN CLASS FOOD IS CODED:
            getAppEnv().addEntity(new Food(getCursorPositionInView()));
            break;

        default:
            break;
        }
    }
}

std::vector<std::string> PPSTest::getHelperText() const
{
    return {    "---------------------",
                "(MP : mouse position)",
                "R   : Reset the simulation",
                "Esc : End of program",
                "C   : Reload config file",
                "G   : Add a gerbil at MP",
                "S   : Add a scorpion at MP",
                "F   : Add a food at MP",
                "D   : Toggle debug mode",
                "Z   : Zoom",
                "->  : Move view to right",
                "<-  : Move view to left",
                "Space : Pause"
           };
}


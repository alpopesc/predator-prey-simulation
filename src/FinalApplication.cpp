/*
 * infosv
 * jan 2012
 * Marco Antognini
 */

#include "FinalApplication.hpp"
#include <Animal/Gerbil.hpp>
#include <Animal/Scorpion.hpp>
#include <Animal/NeuronalScorpion/NeuronalScorpion.hpp>
#include <Animal/NeuronalScorpion/WaveGerbil.hpp>
#include <Environment/Food.hpp>
#include <Environment/FoodGenerator.hpp>
#include <Environment/Wave.hpp>

IMPLEMENT_MAIN(FinalApplication)

void FinalApplication::onRun()
{
    /* nothing to do for the moment */
}
void FinalApplication::onEvent(sf::Event event, sf::RenderWindow& window)
{
    bool const toggle = event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Tab;

    if (toggle) {
        switch (getSimulationMode()) {
        case SimulationMode::PPS:
            setSimulationMode(SimulationMode::NEURONAL);
            break;

        case SimulationMode::NEURONAL:
            setSimulationMode(SimulationMode::PPS);
            break;
        case SimulationMode::TEST :
            /*nothing to do */
            break;
        }
    }

    else {
        switch (getSimulationMode()) {
        case SimulationMode::PPS:
            onEventPPS(event, window);
            break;

        case SimulationMode::NEURONAL:
            onEventNeuronal(event, window);
            break;
        case SimulationMode::TEST :
            /*nothing to do */
            break;
        }
    }

}

void FinalApplication::onUpdate(sf::Time dt)
{
    Application::onUpdate(dt);
    switch (getSimulationMode()) {
    case SimulationMode::PPS:
        onUpdatePPS(dt);
        break;

    case SimulationMode::NEURONAL:
        onUpdateNeuronal(dt);
        break;
    case SimulationMode::TEST :
        /*nothing to do */
        break;
    }
}

void FinalApplication::onSimulationStart()
{
    setSimulationMode(SimulationMode::PPS);

    getAppEnv().addGenerator(new FoodGenerator());

}

void FinalApplication::onEventPPS(sf::Event event, sf::RenderWindow&)
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
            getAppEnv().addEntity(new Food(getCursorPositionInView()));
            break;

        default:
            break;
        }
    }
}

void FinalApplication::onUpdatePPS(sf::Time)
{
    // Nothing
}

void FinalApplication::onEventNeuronal(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        case sf::Keyboard::W:
            getAppEnv().addEntity(new WaveGerbil(getCursorPositionInView()));
            break;

        case sf::Keyboard::N:
            getAppEnv().addEntity(new NeuronalScorpion(getCursorPositionInView()));
            break;

        default:
            break;
        }
    }
}

void FinalApplication::onUpdateNeuronal(sf::Time)
{
    // Nothing
}

std::vector<std::string> FinalApplication::getHelperText() const
{
    if (getSimulationMode() == SimulationMode::PPS) {
        return {    "---------------------",
                    "(MP : mouse position)",
                    "R   : Reset the simulation",
                    "Esc : End of program",
                    "C   : Reload config file",
                    "G   : Add a gerbil at MP",
                    "S   : Add a scorpion at MP",
                    "F   : Add a food at MP",
                    "D   : Toggle debug mode",
                    "Tab : Switch macro/micro view",
                    "Z   : Zoom",
                    "->  : Move view to right",
                    "<-  : Move view to left",
                    "Space : Pause"
               };
    } else {
        return  {    "---------------------",
                     "(MP: mouse position)",
                     "R  : Reset the simulation",
                     "Esc : End of program",
                     "C   : Reload config file"
                     "N  : Add a neuronal scorpion",
                     "         at MP",
                     "W  : Add a wave gerbil at MP",
                     "D  : Toggle debug mode",
                     "Tab: Switch macro/micro view",
                     "Z   : Zoom",
                     "->  : Move view to right",
                     "<-  : Move view to left",
                     "Space : Pause"
                };
    }
}

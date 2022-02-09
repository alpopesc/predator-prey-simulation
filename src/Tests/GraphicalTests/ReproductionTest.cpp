/*
 * infosv
 * jan 2017-19
 * Marco Antognini, Ari Sarfatis
 */

#include "ReproductionTest.hpp"
#include <Animal/Gerbil.hpp>
#include <Animal/Scorpion.hpp>
#include <Environment/Food.hpp>

IMPLEMENT_MAIN(ReproductionTest)

void ReproductionTest::onSimulationStart()
{
    setSimulationMode(SimulationMode::TEST);

}

std::vector<std::string> ReproductionTest::getHelperText() const
{
    return {    "---------------------",
                "(MP : mouse position)",
                "R   : Reset the simulation",
                "Esc : End of program",
                "C   : Reload config file",
                "G   : Add a female gerbil",
                "         at MP",
                "Ctrl-G  : add a male gerbil ",
                "         at MP",
                "S   : Add a female scorpion",
                "         at MP",
                "F   : Add a food at MP",
                "D   : Toggle debug mode",
                "Z   : Zoom",
                "->  : Move view to right",
                "<-  : Move view to left",
                "Space : Pause"
           };
}



Animal* create_animal(const Vec2d& position, char type, bool female)
{
    Animal* animal(nullptr);

    switch (type) {
    case 'S' :
        animal = new Scorpion(position, getAppConfig().scorpion_energy_min_mating_male*2, female);
        break;
    case 'G' :
        animal = new Gerbil(position, getAppConfig().scorpion_energy_min_mating_female*2, female);
        break;
    default:
        animal = nullptr;
    }
    return animal;
}


void ReproductionTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        case sf::Keyboard::S:
            if(event.key.control) {
                Animal* male = create_animal(getCursorPositionInView(), 'S', false);
                if (male != nullptr)
                    getAppEnv().addEntity(male);
            }

            else {
                Animal* female = create_animal(getCursorPositionInView(), 'S', true);
                if (female != nullptr)
                    getAppEnv().addEntity(female);
            }

            break;
        case sf::Keyboard::G:
            if(event.key.control) {
                Animal* male= create_animal(getCursorPositionInView(), 'G', false);
                if (male != nullptr)
                    getAppEnv().addEntity(male);
            }

            else {
                Animal* female = create_animal(getCursorPositionInView(), 'G', true);
                if (female != nullptr)
                    getAppEnv().addEntity(female);
            }

            break;
        case sf::Keyboard::F:
            getAppEnv().addEntity(new Food(getCursorPositionInView()));
            break;

        default:
            break;
        }

    }

}

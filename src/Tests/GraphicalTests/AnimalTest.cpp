/*
 * infosv
 * jan 2012
 * Marco Antognini
 */

#include <Tests/GraphicalTests/AnimalTest.hpp>
#include <Animal/Animal.hpp>
#include <Utility/Utility.hpp>
#define STEP 3

IMPLEMENT_MAIN(AnimalTest)

void AnimalTest::onEvent(sf::Event event, sf::RenderWindow&)
{

#if STEP == 2
    if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {

        case sf::Keyboard::T: {
            Vec2d const coord = getCursorPositionInView();
            getAppEnv().addTarget(coord);
        }

        break;
        case sf::Keyboard::R: {
            getAppEnv().clean();
#if STEP  == 2
            double position(getAppConfig().simulation_world_size / 2);
            getAppEnv().addAnimal(new Animal({ position , position }));
#endif
        }

        break;
        default:
            break;
        }
    }
#endif
}

void AnimalTest::onRun()
{
// TODO add more stuff here
}

void AnimalTest::onSimulationStart()
{
    setSimulationMode(SimulationMode::TEST);
#if STEP  == 2
    double position(getAppConfig().simulation_world_size / 2);
    getAppEnv().addAnimal(new Animal({ position , position }));
#else
    std::cout << "##WARNING : this test only works in step 2" << std::endl;
#endif
}

std::vector<std::string> AnimalTest::getHelperText() const
{
#if STEP  == 2
    return { "---------------------",
             "(MP : mouse position)",
             " T    : add target at MP",
             " R    : Reset the simulation",
             "Space : Pause",
             "Esc   : End of program"
           };
#else
    return { "---------------------",
             "This tests fully works "
             "in step 2 !"
           };
#endif
}

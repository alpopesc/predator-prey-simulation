/*
 * infosv
 * 2013-2019
 * Marco Antognini & Jamila Sam
 */

#include <Tests/GraphicalTests/EnvTest.hpp>
#include <iostream>
#define STEP 3

IMPLEMENT_MAIN(EnvTest)

void EnvTest::onEvent(sf::Event event, sf::RenderWindow&)
{
#if STEP == 2
    if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {

        case sf::Keyboard::T: {
            Vec2d const coord = getCursorPositionInView();
            getAppEnv().addTarget(coord);
        }

        break;

        default:
            break;
        }
    }
#endif
}

void EnvTest::onRun()
{
// TODO add more stuff here
}

void EnvTest::onSimulationStart()
{
    setSimulationMode(SimulationMode::TEST);
#if STEP != 2
    std::cout << "##WARNING : this test only works in step 2" << std::endl;
#endif
}

std::vector<std::string> EnvTest::getHelperText() const
{
#if STEP  == 2
    return { "---------------------",
             "(MP : mouse position)",
             " T  : add target at MP",
             " R  : Reset the simulation",
             " Esc: End of program",
             " Space : Pause"
           };
#else
    return { "---------------------",
             "This tests fully works "
             "in step 2 !"
           };
#endif
}

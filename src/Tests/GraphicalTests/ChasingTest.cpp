/*
 * infosv projet 2019
 *
 */

#include <Tests/GraphicalTests/ChasingTest.hpp>
#include<iostream>

IMPLEMENT_MAIN(ChasingTest)

ChasingTest::~ChasingTest()
{
    delete mAutomaton;
    mAutomaton = nullptr;
}

void ChasingTest::onSimulationStart()
{
    setSimulationMode(SimulationMode::TEST);
    double position(getAppConfig().simulation_world_size / 2);
    mAutomaton = new ChasingAutomaton({ position , position });
}

void ChasingTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {

        case sf::Keyboard::T: {
            Vec2d const coord = getCursorPositionInView();
            mAutomaton->setTargetPosition(coord);
        }

        break;

        default:
            break;
        }
    }
}

void ChasingTest::onUpdate(sf::Time dt)
{
    mAutomaton->update(dt);
}

void ChasingTest::onDraw(sf::RenderTarget& target)
{
    mAutomaton->draw(target);
}

void ChasingTest::onRun()
{
// TODO add more stuff here
}

std::vector<std::string> ChasingTest::getHelperText() const
{
    return {    "---------------------",
                "(MP : mouse position)",
                "Esc : End of program",
                "T   : Add a target at MP",
                "Z   : Zoom",
                "->  : Move view to right",
                "<-  : Move view to left",
                "Space : Pause"
           };
}

/*
 * infosv
 * Projet 2019
 * Marco Antognini & Jamila Sam
 */

#ifndef INFOSV_CHASING_TEST_HPP
#define INFOSV_CHASING_TEST_HPP

#include <Application.hpp>
#include <Animal/ChasingAutomaton.hpp>

/*!
 * Tests the ChasingAutomaton
 *
 * On click, sets the target of the chasing automaton
  * Run it with the correct cfg (res/app2.json) :
 * > scons ChasingTest-run --cfg=app2.json
 */
class ChasingTest : public Application
{
public:
    ChasingTest(int argc, char const** argv)
        : Application(argc, argv)
    {
    }

    virtual ~ChasingTest();
    virtual void onSimulationStart() override final;
    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
    virtual void onUpdate(sf::Time dt) override final;
    virtual void onDraw(sf::RenderTarget& target) override final;
    virtual void onRun() override final;
    virtual std::vector<std::string> getHelperText() const final override;
private:
    ChasingAutomaton* mAutomaton;
};

#endif // INFOSV_CHASING_TEST_HPP


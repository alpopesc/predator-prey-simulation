/*
 * infosv
 * jan 2012-19
 * Marco Antognini
 */

#ifndef INFOSV_PPS_TEST_HPP
#define INFOSV_PPS_TEST_HPP

#include "Application.hpp"

class PPSTest : public Application
{
public:
    PPSTest(int argc, char const** argv)
        : Application(argc, argv)
    {}


    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
    virtual void onUpdate(sf::Time dt) override final;
    virtual void onRun() override final;
private:
    void onEventPPS(sf::Event event, sf::RenderWindow& window);
    void onSimulationStart() override;
    virtual std::vector<std::string> getHelperText() const final override;

};

#endif // INFOSV_PPS_TEST_HPP


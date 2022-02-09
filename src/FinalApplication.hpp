/*
 * infosv
 * jan 2012
 * Marco Antognini
 */

#ifndef INFOSV_FINAL_APPLICATION_HPP
#define INFOSV_FINAL_APPLICATION_HPP

#include "Application.hpp"

class FinalApplication : public Application
{
public:
    FinalApplication(int argc, char const** argv)
        : Application(argc, argv)
    {}


    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
    virtual void onUpdate(sf::Time dt) override final;
    virtual void onRun() override final;
private:
    void onEventPPS(sf::Event event, sf::RenderWindow& window);
    void onUpdatePPS(sf::Time dt);

    void onSimulationStart() override;

    void onEventNeuronal(sf::Event event, sf::RenderWindow& window);
    void onUpdateNeuronal(sf::Time dt);
    virtual std::vector<std::string> getHelperText() const final override;
};

#endif // INFOSV_FINAL_APPLICATION_HPP


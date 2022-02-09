/*
 * infosv
 * 2013-2019
 * Marco Antognini & Jamila Sam
 */

#ifndef INFOSV_REPRODUCTION_TEST_HPP
#define INFOSV_REPRODUCTION_TEST_HPP

#include <Application.hpp>

/*!
 * Test the reproduction.
 *
 * Run it with the correct cfg (res/reprod.json) :
 * > scons ReproductionTest-run --cfg=reprod.json
 */
class ReproductionTest : public Application
{
public:
    ReproductionTest(int argc, char const** argv)
        :Application(argc, argv)
    {}
    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
    virtual void onSimulationStart() override final;
    virtual std::vector<std::string> getHelperText() const final override;
};

#endif // INFOSV_REPRODUCTION_TEST_HPP


/*
 * prjsv 2016
 * 2013, 2014, 2016
 * Marco Antognini
 */

#include <Application.hpp>
#include "Config.hpp"
#include <JSON/JSONSerialiser.hpp>
#include <Utility/Constants.hpp>
#include <iomanip> // setprecision
#include <sstream> // stringstream

#include <algorithm>
#include <cassert>

namespace // anonymous
{

Application* currentApp = nullptr; ///< Current application

std::string applicationDirectory(int argc, char const** argv)
{
    assert(argc >= 1);

    auto dir = std::string(argv[0]);

    auto lastSlashPos = dir.rfind('/');
    if (lastSlashPos == std::string::npos) {
        dir = "./";
    } else {
        dir = dir.substr(0, lastSlashPos + 1);
    }

    return dir;
}

std::string configFileRelativePath(int argc, char const** argv)
{
    if (argc >= 2) {
        return RES_LOCATION + argv[1];
    } else {
        return RES_LOCATION + DEFAULT_CFG;
    }
}

/*
 * get*Size and get*Position: see createViews for graphical layout
 */

Vec2d getWindowSize()
{
    auto width =
        getAppConfig().window_simulation_width +
        getAppConfig().window_control_width;;
    auto height =
        getAppConfig().window_simulation_height +
        getAppConfig().window_stats_width;
    return { width, height };
}

Vec2d getSimulationSize()
{
    auto width = getAppConfig().window_simulation_width;
    auto height = getAppConfig().window_simulation_height;
    return { width, height };
}

Vec2d getWorldSize()
{
    double size(getAppConfig().simulation_world_size);
    return { size, size };
}

Vec2d getControlSize()
{
    auto width = getAppConfig().window_control_width;
    auto height = getAppConfig().window_simulation_height/2;
    return { width, height };
}
Vec2d getControlPosition()
{
    return { getAppConfig().window_simulation_width, 0 };
}
Vec2d getSimulationPosition()
{
    return { 0, 0 };
}

Vec2d getStatsSize()
{
    auto width = getAppConfig().window_simulation_width
                 + getAppConfig().window_control_width;
    auto height = getAppConfig().window_stats_width;
    return { width, height };
}

Vec2d getStatsPosition()
{
    return { 0 , getAppConfig().window_simulation_height};
}

Vec2d getHelpSize()
{
    auto width = getAppConfig().window_control_width;
    auto height = getAppConfig().window_simulation_height/2;
    return { width, height };
}

Vec2d getHelpPosition()
{
    return { getAppConfig().window_simulation_width, getAppConfig().window_simulation_height/2 };
}
/*!
 * @brief Create a view with the given properties
 *
 * @param viewSize size of the OpenGL rendering space (might be bigger/smaller than allocation)
 * @param position top-left corner of the view in window coordinates
 * @param allocation size of the view in window
 * @param windowSize size of the window
 */
sf::View setupView(Vec2d const& viewSize,
                   Vec2d const& position, Vec2d const& allocation,
                   sf::Vector2u const& windowSize)
{
    sf::View view;
    view.reset({ { 0, 0 }, viewSize });
    view.setViewport({ static_cast<float>(position.x / windowSize.x),
                       static_cast<float>(position.y / windowSize.y),
                       static_cast<float>(allocation.x / windowSize.x),
                       static_cast<float>(allocation.y / windowSize.y)
                     });

    return view;
}

} // anonymous

Application::Application(int argc, char const** argv)
    : mAppDirectory(applicationDirectory(argc, argv))
    , mCfgFile(configFileRelativePath(argc, argv))
//, mJSONRead(mAppDirectory + mCfgFile)
    , mConfig(new Config(mAppDirectory + mCfgFile))
    , mCurrentGraphId(-1)
    ,mEnvPPS(nullptr)
    , mEnvNeuronal(nullptr)
    , mPaused(false)
    , mIsResetting(false)
    , mIsSwitchingView(false)
    , mIsDragging(false)

{
    setSimulationMode(SimulationMode::PPS);
    // Set global singleton
    assert(currentApp == nullptr);
    currentApp = this;

    std::cerr << "Using " << (mAppDirectory + mCfgFile) << " for configuration.\n";

    // Load the font
    if (!mFont.loadFromFile(mAppDirectory + FONT_LOCATION)) {
        std::cerr << "Couldn't load " << FONT_LOCATION << std::endl;
    }
    // prepare simulation background
    mSimulationBackground = sf::RectangleShape();
    mSimulationBackground.setSize(getWorldSize());
    //simulationBackground.setFillColor(sf::Color::Black);
    mSimulationBackground.setOutlineColor(sf::Color::Black);
    mSimulationBackground.setOutlineThickness(5);
    chooseBackground();
}


Application::~Application()
{
    // Destroy lab and stats, in reverse order
    delete mEnvPPS;
    mEnvPPS = nullptr;
    delete mEnvNeuronal;
    mEnvNeuronal = nullptr;
    delete mConfig;

    // Release textures
    for (auto& kv : mTextures) {
        delete kv.second;
        kv.second = nullptr;
    }
    mTextures.clear();

    // Reset the global pointer
    currentApp = nullptr;
}

void Application::run()
{
    // Load Environment and stats
    mEnvPPS = new Environment();
    mEnvNeuronal = new Environment();
    // Set up subclasses
    onRun();
    onSimulationStart();

    // Create the SFML window
    createWindow(getWindowSize());

    // Views for rendering regions
    createViews();

    // Create the Stats background (grey board)
    auto statsBackground = sf::RectangleShape();
    statsBackground.setSize(getStatsSize());
    statsBackground.setFillColor(sf::Color::Black);
    // Create the control background (grey board)
    auto controlBackground = sf::RectangleShape();
    controlBackground.setSize(getControlSize());
    controlBackground.setFillColor(sf::Color(128, 128, 128));
    // Use a clock to track time
    sf::Clock clk;

    // FPS counter
    sf::Clock fpsClk;
    int frameCount = 0;
    int nbCycles = 10;
    // Main loop
    while (mRenderWindow.isOpen()) {
        // Handle events
        sf::Event event;
        while (mRenderWindow.pollEvent(event)) {
            handleEvent(event, mRenderWindow);
        }


        // Update logics

        float timeFactor = getAppConfig().simulation_time_factor;
        auto elapsedTime = clk.restart() * timeFactor; // Always reset the clock!

        if (!mPaused && !mIsResetting) {
            // Update simulation with the elapsed time, possibly
            // by calling update(dt) several time to avoid update
            // with high delta time.
            //
            // An alternative implementation could be based on fixed
            // timesteps.
            sf::Time maxDt = getAppConfig().simulation_time_max_dt;
            while (elapsedTime > sf::Time::Zero) {
                auto dt = std::min(elapsedTime, maxDt);
                elapsedTime -= dt;
                getEnv().update(dt);
                onUpdate(dt);
                --nbCycles;

            }
        }
        // Render everything
        render(mSimulationBackground, statsBackground, controlBackground);
        ++frameCount;

        // In case we were resetting the simulation
        mIsResetting = false;

        // FPS computation
        //++frameCount;
        if (fpsClk.getElapsedTime() > sf::seconds(2)) {
            auto dt = fpsClk.restart().asSeconds();

            auto fps = frameCount / dt;
            std::cout << "FPS: " << fps << "\r" << std::flush;

            frameCount = 0;
        }
    }
}

Environment& Application::getEnv()
{
    return getSimulationMode() == SimulationMode::PPS ? *mEnvPPS : *mEnvNeuronal;
}

Environment const& Application::getEnv() const
{
    return getSimulationMode() == SimulationMode::PPS ? *mEnvPPS : *mEnvNeuronal;
}

Config& Application::getConfig()
{
    return *mConfig;
}

Config const& Application::getConfig() const
{
    return *mConfig;
}

sf::Font const& Application::getFont() const
{
    return mFont;
}

sf::Texture& Application::getTexture(std::string const& name)
{
    auto const it = mTextures.find(name);
    if (it != mTextures.end())
        return *it->second;
    else {
        // The texture is not yet in memory so we load it now
        sf::Texture* newTexture = new sf::Texture;
        if (newTexture->loadFromFile(getResPath() + name)) {
            // The texture was correctly loaded so we save it
            newTexture->setRepeated(true);
            mTextures[name] = newTexture;
            // And return the texture
            return *newTexture;
        } else {
            // The file was not loaded correctly so we clean up the mess
            delete newTexture;
            newTexture = nullptr;
            // And return the default texture
            return mDefaultTexture;
        }
    }
}


std::string Application::getResPath() const
{
    return mAppDirectory + RES_LOCATION;
}


Vec2d Application::getCentre() const
{
    // TODO : add organ
    return getSimulationSize() / 2.0;
}

void Application::onRun()
{
    // By default nothing is done here
    chooseBackground();
}

void Application::chooseBackground()
{

    mSimulationBackground = mSimulationBackground;
    mSimulationBackground.setTexture(&getAppTexture(isDebugOn() ?
                                     getAppConfig().simulation_world_debug_texture :
                                     getAppConfig().simulation_world_texture)
                                     , true);

}

void Application::onEvent(sf::Event, sf::RenderWindow&)
{
    // By default nothing is done here
}

void Application::onSimulationStart()
{
    // By default nothing is done here
}

void Application::onUpdate(sf::Time)
{
    // By default nothing is done here
}

void Application::onDraw(sf::RenderTarget&)
{
    // By default nothing is done here

}

Vec2d Application::getCursorPositionInView() const
{
    return mRenderWindow.mapPixelToCoords(sf::Mouse::getPosition(mRenderWindow), mSimulationView);
}


void Application::createWindow(Vec2d const& size)
{
    sf::VideoMode vm(size.x, size.y);

    auto title = getAppConfig().window_title;

    sf::ContextSettings contextSettings;
    contextSettings.antialiasingLevel = getAppConfig().window_antialiasing_level;

    // Create the window
    mRenderWindow.create(vm, title, sf::Style::Close, contextSettings);
    mRenderWindow.setKeyRepeatEnabled(true);
    mRenderWindow.setFramerateLimit(60);
}

void Application::createViews()
{
    //   WINDOW STRUCTURE
    //
    //   ----------------------------
    //   |                      |   | <- mControlFrame
    //   |                      |   |
    //   |   mSimulationFrame   |   |
    //   |                      |   |
    //   |                      |---|
    //   |                      |   | <- mCommandsHelpFrame
    //   |                      |   |
    //   ----------------------------
    //   |                          | <- mStatsFrame
    //   ----------------------------


    mSimulationView = setupView(getWorldSize(),
                                getSimulationPosition(), getSimulationSize(),
                                mRenderWindow.getSize());

    mStatsView = setupView(getStatsSize(),
                           getStatsPosition(), getStatsSize(),
                           mRenderWindow.getSize());

    mHelpView = setupView(getHelpSize(),
                          getHelpPosition(), getHelpSize(),
                          mRenderWindow.getSize());
    mControlView = setupView(getControlSize(),
                             getControlPosition(), getControlSize(),
                             mRenderWindow.getSize());

}

void Application::handleEvent(sf::Event event, sf::RenderWindow& window)
{
    // zoom factor
    auto const ZOOM = 1.1f;

    switch (event.type) {
    case sf::Event::Closed:
        window.close();
        break;

    case sf::Event::KeyPressed:
        switch (event.key.code) {

        // Toggle debug mode
        case sf::Keyboard::D:
            switchDebug();
            break;

        // Exit simulation
        case sf::Keyboard::Escape:
            window.close();
            break;

        case sf::Keyboard::C:
            delete mConfig;
            mConfig = new Config(mAppDirectory + mCfgFile); // reconstruct
            chooseBackground();
            break;

        // Toggle pause for simulation
        case sf::Keyboard::Space:
            mPaused = !mPaused;
            break;

        // Reset the simulation
        case sf::Keyboard::R:
            getEnv().clean();
            break;

        case sf::Keyboard::Tab:
            // TODO add TAB binding for switching from graphs
            break;

        // Reset the simulation
        case sf::Keyboard::Right:
            mSimulationView.move(100, 0);
            break;
        case sf::Keyboard::Left:
            mSimulationView.move(-100, 0);
            break;
        case sf::Keyboard::Up:
            mSimulationView.move(0, -100);
            break;
        case sf::Keyboard::Down:
            mSimulationView.move(0, 100);
            break;



        default:
            onEvent(event, window);
            break;
        } // event.key.code switchfor sf::Event::KeyReleased
        break;


        // Handle zoom for SFML 2.3+ and older versions as well
#if SFML_VERSION_MAJOR >= 2 && (SFML_VERSION_MINOR > 3 || (SFML_VERSION_MINOR == 3 && SFML_VERSION_PATCH >= 2))
    case sf::Event::MouseWheelScrolled:
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            sf::Vector2i pos = { event.mouseWheelScroll.x, event.mouseWheelScroll.y };

            if (event.mouseWheelScroll.delta > 0)
                zoomViewAt(pos, (1.f / ZOOM));
            else if (event.mouseWheelScroll.delta < 0)
                zoomViewAt(pos, ZOOM);
        }
        break;
#else
    // MouseWheelMoved is deprecated. If possible, use MouseWheelScrolled as replacement
    case sf::Event::MouseWheelMoved: {
        sf::Vector2i pos = { event.mouseWheel.x, event.mouseWheel.y };

        if (event.mouseWheel.delta > 0)
            zoomViewAt(pos, (1.f / ZOOM));
        else if (event.mouseWheel.delta < 0)
            zoomViewAt(pos, ZOOM);

        break;
    }
#endif

    // Drag view: initiate drag
    //  - or -
    // Select a bee to follow
    // case sf::Event::MouseButtonPressed:
    //     if (event.mouseButton.button == sf::Mouse::Left) {
    //         mIsDragging = true;
    //         mLastCursorPosition = { event.mouseButton.x, event.mouseButton.y };
    //     } else if (event.mouseButton.button == sf::Mouse::Right) {
    //         auto pos = getCursorPositionInView();
    //         auto* bee = getLab().getAnimalAt(pos);
    //         if (bee == nullptr) {
    //             // Stop tracking animal
    //             getAnimalTracker().stopTrackingAnimal();
    //         } else {
    //             // Track the animal
    //             getAnimalTracker().startTrackingAnimal(bee);
    //         }
    //     }
    //     break;

    // Drag view: end drag
    case sf::Event::MouseButtonReleased:
        if (event.mouseButton.button == sf::Mouse::Left)
            mIsDragging = false;
        break;

    // Drag view: move view
    case sf::Event::MouseMoved:
        if (mIsDragging) {
            sf::Vector2i newCurosrPosition = { event.mouseMove.x, event.mouseMove.y };

            dragView(mLastCursorPosition, newCurosrPosition);

            mLastCursorPosition = newCurosrPosition;

            // Stop tracking bee
            //getBeeTracker().stopTrackingBee();
        }
        break;

    default:
        onEvent(event, window);
        break;
    } // event.type switch
}

void Application::render(sf::Drawable const& simulationBackground, sf::Drawable const& statsBackground, sf::Drawable const& controlBackground)
{
    mRenderWindow.clear();

    // Render the simulation
    updateSimulationView();
    mRenderWindow.setView(mSimulationView);
    mRenderWindow.draw(simulationBackground);
    getEnv().draw(mRenderWindow);
    onDraw(mRenderWindow);

    // Render the command help
    mRenderWindow.setView(mHelpView);
    mRenderWindow.draw(controlBackground);
    drawOnHelp(mRenderWindow);
    // Render the controls
    mRenderWindow.setView(mControlView);
    mRenderWindow.draw(controlBackground);

    // Render the controls
    drawControls(mRenderWindow);


    // Render the stats
    mRenderWindow.setView(mStatsView);
    mRenderWindow.draw(statsBackground);
    // Finally, publish everything onto the screen
    mRenderWindow.display();

    // Reconfigure the window to use the simulation view
    // so that handling event (zoom + move) is easier
    mRenderWindow.setView(mSimulationView);

}

void Application::togglePause()
{
    mPaused = !mPaused;
}

//void Application::saveConfig() const
//{
//    j::writeToFile(getConfig().getJsonRead(), mAppDirectory + mCfgFile);
//}

void Application::zoomViewAt(sf::Vector2i const& /*pixel*/, float zoomFactor)
{
    // Note: we know that the simulation view is active
    sf::View& view = mSimulationView;

    //auto beforeCoord = mRenderWindow.mapPixelToCoords(pixel);

    view.zoom(zoomFactor);
    mRenderWindow.setView(view);

    // if (!getAnimalTracker().isTrackingAnimal()) {
    //     // If no animal  is selected, center on the cursor position
    //     auto afterCoord = mRenderWindow.mapPixelToCoords(pixel);
    //     auto offsetCoords = beforeCoord - afterCoord;

    //     view.move(offsetCoords);
    //     mRenderWindow.setView(view);
    // }
}

void Application::dragView(sf::Vector2i const& srcPixel, sf::Vector2i const& destPixel)
{
    // Note: we know that the simulation view is active
    sf::View& view = mSimulationView;

    auto src = mRenderWindow.mapPixelToCoords(srcPixel);
    auto dest = mRenderWindow.mapPixelToCoords(destPixel);
    auto dx = src - dest;

    view.move(dx);
    mRenderWindow.setView(view);
}

void Application::updateSimulationView()
{
    // if (getAnimalTracker().isTrackingAnimal()) {
    //     auto pos = getAnimalTracker().getTrackedAnimalPosition();
    //     mSimulationView.setCenter(pos);
    // }
    /*
    	 mSimulationView = setupView(getWorldSize(),
                                    getSimulationPosition(), getSimulationSize(),

    								mRenderWindow.getSize());
    */
    /*
     auto viewSize = getWorldSize();
     mSimulationView.reset({ { 0, 0 }, viewSize });
    */
    /*
    mSimulationView.setViewport({ static_cast<float>(position.x / windowSize.x),
                      static_cast<float>(position.y / windowSize.y),
                      static_cast<float>(allocation.x / windowSize.x),
                      static_cast<float>(allocation.y / windowSize.y) });
    */
}

void Application::switchDebug()
{
    getAppConfig().switchDebug();
    chooseBackground();
}

std::vector<std::string> Application::getHelperText() const
{
    // by default no helper text displayed
    return { };
}

void Application::drawOnHelp(sf::RenderWindow& window) const
{
    auto const LEGEND_MARGIN = 10;
    auto const FONT_SIZE = 20;
    auto lastLegendY = LEGEND_MARGIN;

    auto text(getHelperText());

    for (auto& command : text) {
        auto legend = sf::Text(command, getAppFont(), FONT_SIZE);
        legend.setPosition(LEGEND_MARGIN, lastLegendY);
#if SFML_VERSION_MAJOR >= 2 && SFML_VERSION_MINOR >= 4
        legend.setFillColor(sf::Color::Black);
#else
        legend.setColor(sf::Color::Black);
#endif
        window.draw(legend);

        lastLegendY += FONT_SIZE + 4;
    }
}


Application& getApp()
{
    assert(currentApp != nullptr);

    return *currentApp;
}

Environment& getAppEnv()
{
    return getApp().getEnv();
}

// if needed
// AnimalTracker& getAppAnimalTracker()
// {
//     return getApp().getAnimalTracker();
// }

Config& getAppConfig()
{
    return getApp().getConfig();
}

sf::Font const& getAppFont()
{
    return getApp().getFont();
}

sf::Texture& getAppTexture(std::string const& name)
{
    return getApp().getTexture(name);
}


bool isDebugOn()
{
    return getAppConfig().getDebug();
}


void Application::drawControls(sf::RenderWindow& target)
{
    auto const LEGEND_MARGIN(10);
    auto lastLegendY(std::max (10., getSimulationSize().y/5));
    auto const FONT_SIZE = 20;
    drawTitle(target, sf::Color::White, LEGEND_MARGIN, lastLegendY, FONT_SIZE);
    lastLegendY += FONT_SIZE + 4;
    /*
    drawOneControl(target, s::DELTAGLUC, mLab->getDelta(GLUCOSE),
    			   sf::Color::Blue, LEGEND_MARGIN, lastLegendY, FONT_SIZE);
    lastLegendY += FONT_SIZE + 4;

    drawOneControl(target, s::DELTABROM, mLab->getDelta(BROMOPYRUVATE),
    			   sf::Color::Yellow, LEGEND_MARGIN, lastLegendY, FONT_SIZE);
    lastLegendY += FONT_SIZE + 4;

    drawOneControl(target, s::DELTAVGEF, mLab->getDelta(VGEF),
    			   sf::Color::Green, LEGEND_MARGIN, lastLegendY, FONT_SIZE);
    */
}

void Application::drawTitle(sf::RenderWindow& target
                            , sf::Color color
                            , size_t xcoord
                            , size_t ycoord
                            , size_t font_size
                           )
{
    std::string text;

    switch(mMode) {
    case SimulationMode::PPS :
        text = s::PPS;
        break;
    case SimulationMode::NEURONAL :
        text = s::NEURONAL;
        break;
    case SimulationMode::TEST :
    default:
        text = s::TEST;
        break;
    }

    auto legend = sf::Text(text, getAppFont(), font_size);
    legend.setPosition(xcoord, ycoord);
#if SFML_VERSION_MAJOR >= 2 && SFML_VERSION_MINOR >= 4
    legend.setFillColor(color);
#else
    legend.setColor(color);
#endif
    legend.setStyle(sf::Text::Bold);
    legend.setStyle(sf::Text::Underlined);
    target.draw(legend);
}
void Application::drawOneControl(sf::RenderWindow& target
                                 , std::string name
                                 , double value
                                 , sf::Color color
                                 , size_t xcoord
                                 , size_t ycoord
                                 , size_t font_size
                                )
{
    std::stringstream tmpStream;

    tmpStream << std::fixed << std::setprecision(2) << value;
    auto value_str = tmpStream.str();
    value_str = value_str.substr(0, value_str.find_last_not_of("0"));
    auto text = name + " : " +  value_str;
    auto legend = sf::Text(text, getAppFont(), font_size);
    legend.setPosition(xcoord, ycoord);
#if SFML_VERSION_MAJOR >= 2 && SFML_VERSION_MINOR >= 4
    legend.setFillColor(color);
#else
    legend.setColor(color);
#endif
    target.draw(legend);
}

void Application::setSimulationMode(SimulationMode mode)
{
    mMode = mode;
}

Application::SimulationMode Application::getSimulationMode() const
{
    return mMode;
}



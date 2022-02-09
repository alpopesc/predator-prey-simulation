#include "Config.hpp"
#include <JSON/JSONSerialiser.hpp>
// window
Config::Config(std::string path) : mConfig(j::readFromFile(path))
    , simulation_debug(mConfig["debug"].toBool())
    ,window_simulation_width(mConfig["window"]["simulation"]["width"].toDouble())
    , window_simulation_height(mConfig["window"]["simulation"]["height"].toDouble())
    , window_stats_width(mConfig["window"]["stats"]["width"].toDouble())
    , window_control_width(mConfig["window"]["control"]["width"].toDouble())
    , window_title(mConfig["window"]["title"].toString())
    , window_antialiasing_level(mConfig["window"]["antialiasing level"].toInt())

// stats
    , stats_refresh_rate(mConfig["stats"]["refresh rate"].toDouble())

// simulation
    , simulation_world_texture(mConfig["simulation"]["world"]["texture"].toString())
    , simulation_world_debug_texture(mConfig["simulation"]["world"]["debug texture"].toString())
    , simulation_world_size(mConfig["simulation"]["world"]["size"].toDouble())
    , simulation_time_factor(mConfig["simulation"]["time"]["factor"].toDouble())
    , simulation_time_max_dt(sf::seconds(mConfig["simulation"]["time"]["max dt"].toDouble()))

// food generator
    , food_generator_delta(mConfig["simulation"]["food generator"]["delta"].toDouble())
// food
    ,food_texture(mConfig["simulation"]["food"]["texture"].toString())
    ,food_size(mConfig["simulation"]["food"]["size"].toDouble())
    ,food_energy(mConfig["simulation"]["food"]["energy"].toDouble())
// animal
    , animal_random_walk_low_energy(mConfig["simulation"]["animal"]["random walk low energy"].toBool())
//, animal_safe_distance(mConfig["simulation"]["animal"]["random walk low energy"].toDouble())
    , animal_meal_retention(mConfig["simulation"]["animal"]["meal retention"].toDouble())
    , animal_feed_time(mConfig["simulation"]["animal"]["feed time"].toDouble())
    , animal_delivery_time(mConfig["simulation"]["animal"]["reproduction"]["delivery time"].toDouble())
    , animal_mating_time(mConfig["simulation"]["animal"]["reproduction"]["mating time"].toDouble())
    , animal_min_energy(mConfig["simulation"]["animal"]["min energy"].toDouble())
    , animal_base_energy_consumption(mConfig["simulation"]["animal"]["base consumption"].toDouble())
// gerbil
    , gerbil_max_speed(mConfig["simulation"]["animal"]["gerbil"]["max speed"].toDouble())
    , gerbil_mass(mConfig["simulation"]["animal"]["gerbil"]["mass"].toDouble())
    , gerbil_energy_loss_factor(mConfig["simulation"]["animal"]["gerbil"]["energy"]["loss factor"].toDouble())
    , gerbil_view_range(mConfig["simulation"]["animal"]["gerbil"]["view"]["range"].toDouble())
    , gerbil_view_distance(mConfig["simulation"]["animal"]["gerbil"]["view"]["distance"].toDouble())
    , gerbil_random_walk_jitter(mConfig["simulation"]["animal"]["gerbil"]["random walk"]["jitter"].toDouble())
    , gerbil_random_walk_radius(mConfig["simulation"]["animal"]["gerbil"]["random walk"]["radius"].toDouble())
    , gerbil_random_walk_distance(mConfig["simulation"]["animal"]["gerbil"]["random walk"]["distance"].toDouble())
    , gerbil_longevity(sf::seconds(mConfig["simulation"]["animal"]["gerbil"]["longevity"].toDouble()))
    , gerbil_energy_initial(mConfig["simulation"]["animal"]["gerbil"]["energy"]["initial"].toDouble())
    , gerbil_energy_min_mating_female(mConfig["simulation"]["animal"]["gerbil"]["energy"]["min mating female"].toDouble())
    , gerbil_energy_min_mating_male(mConfig["simulation"]["animal"]["gerbil"]["energy"]["min mating male"].toDouble())
    , gerbil_min_children(mConfig["simulation"]["animal"]["gerbil"]["reproduction"]["min children"].toInt())
    , gerbil_max_children(mConfig["simulation"]["animal"]["gerbil"]["reproduction"]["max children"].toInt())
    , gerbil_gestation_time(mConfig["simulation"]["animal"]["gerbil"]["reproduction"]["gestation time"].toDouble())
    , gerbil_energy_loss_female_per_child(mConfig["simulation"]["animal"]["gerbil"]["energy"]["loss female per child"].toDouble())
    , gerbil_energy_loss_mating_male(mConfig["simulation"]["animal"]["gerbil"]["energy"]["loss mating male"].toDouble())
    , gerbil_texture_male(mConfig["simulation"]["animal"]["gerbil"]["texture"]["male"].toString())
    , gerbil_texture_female(mConfig["simulation"]["animal"]["gerbil"]["texture"]["female"].toString())
    , gerbil_size(mConfig["simulation"]["animal"]["gerbil"]["size"].toDouble())
    , gerbil_min_age_mating(mConfig["simulation"]["animal"]["gerbil"]["min age mating"].toDouble())
    , wave_gerbil_frequency(mConfig["simulation"]["animal"]["gerbil"]["wave"]["frequency"].toDouble())
    , wave_gerbil_energy_loss_factor(mConfig["simulation"]["animal"]["gerbil"]["wave"]["loss factor"].toDouble())
// scorpion
    , scorpion_max_speed(mConfig["simulation"]["animal"]["scorpion"]["max speed"].toDouble())
    , scorpion_mass(mConfig["simulation"]["animal"]["scorpion"]["mass"].toDouble())
    , scorpion_energy_loss_factor(mConfig["simulation"]["animal"]["scorpion"]["energy"]["loss factor"].toDouble())
    , scorpion_view_range(mConfig["simulation"]["animal"]["scorpion"]["view"]["range"].toDouble())
    , scorpion_view_distance(mConfig["simulation"]["animal"]["scorpion"]["view"]["distance"].toDouble())
    , scorpion_random_walk_jitter(mConfig["simulation"]["animal"]["scorpion"]["random walk"]["jitter"].toDouble())
    , scorpion_random_walk_radius(mConfig["simulation"]["animal"]["scorpion"]["random walk"]["radius"].toDouble())
    , scorpion_random_walk_distance(mConfig["simulation"]["animal"]["scorpion"]["random walk"]["distance"].toDouble())
    , scorpion_longevity(sf::seconds(mConfig["simulation"]["animal"]["scorpion"]["longevity"].toDouble()))
    , scorpion_energy_initial(mConfig["simulation"]["animal"]["scorpion"]["energy"]["initial"].toDouble())
    , scorpion_energy_min_mating_female(mConfig["simulation"]["animal"]["scorpion"]["energy"]["min mating female"].toDouble())
    , scorpion_energy_min_mating_male(mConfig["simulation"]["animal"]["scorpion"]["energy"]["min mating male"].toDouble())
    , scorpion_min_children(mConfig["simulation"]["animal"]["scorpion"]["reproduction"]["min children"].toInt())
    , scorpion_max_children(mConfig["simulation"]["animal"]["scorpion"]["reproduction"]["max children"].toInt())
    , scorpion_gestation_time(mConfig["simulation"]["animal"]["scorpion"]["reproduction"]["gestation time"].toDouble())
    , scorpion_energy_loss_female_per_child(mConfig["simulation"]["animal"]["scorpion"]["energy"]["loss female per child"].toDouble())
    , scorpion_energy_loss_mating_male(mConfig["simulation"]["animal"]["scorpion"]["energy"]["loss mating male"].toDouble())
    , scorpion_texture((mConfig["simulation"]["animal"]["scorpion"]["texture"].toString()))
    , scorpion_size(mConfig["simulation"]["animal"]["scorpion"]["size"].toDouble())
    ,scorpion_min_age_mating(mConfig["simulation"]["animal"]["scorpion"]["min age mating"].toDouble())
    , scorpion_sensor_radius(mConfig["simulation"]["animal"]["scorpion"]["sensor radius"].toDouble())
    , scorpion_minimal_score_for_action(mConfig["simulation"]["animal"]["scorpion"]["score for action"].toDouble())
    , scorpion_rotation_angle_precision(mConfig["simulation"]["animal"]["scorpion"]["rotation"]["angle precision"].toDouble())
    , neuronal_scorpion_texture(mConfig["simulation"]["animal"]["scorpion"]["neuronal texture"].toString())
// sensor
    ,sensor_intensity_threshold(mConfig["simulation"]["sensor"]["intensity threshold"].toDouble())
    ,sensor_inhibition_factor(mConfig["simulation"]["sensor"]["inhibition"]["factor"].toDouble())
    ,sensor_inhibition_max(mConfig["simulation"]["sensor"]["inhibition"]["max"].toDouble())
    ,sensor_activation_duration(mConfig["simulation"]["sensor"]["activation duration"].toDouble())
//wave
    ,wave_intensity_thickness_ratio(mConfig["simulation"]["wave"]["intensity"]["thickness ratio"].toDouble())
    ,wave_intensity_threshold(mConfig["simulation"]["wave"]["intensity"]["threshold"].toDouble())
    ,wave_on_wave_marging(mConfig["simulation"]["wave"]["on wave marging"].toDouble())
    ,wave_default_energy(mConfig["simulation"]["wave"]["default energy"].toDouble())
    ,wave_default_radius(mConfig["simulation"]["wave"]["default radius"].toDouble())
    ,wave_default_mu(mConfig["simulation"]["wave"]["default MU"].toDouble())
    ,wave_default_speed(mConfig["simulation"]["wave"]["default speed"].toDouble())
    
//rock
	,rock_texture(mConfig["simulation"]["rock"]["texture"].toString())
{
}

// TODO : getter for debug
void Config::switchDebug()
{
    mConfig["debug"] = j::boolean(!mConfig["debug"].toBool());
    simulation_debug = mConfig["debug"].toBool();
}

bool Config::getDebug()
{
    return simulation_debug;
}

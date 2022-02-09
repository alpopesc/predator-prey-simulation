/*
 * prjsv 2016
 * Marco Antognini
 */

#ifndef INFOSV_CONFIG_HPP
#define INFOSV_CONFIG_HPP

#include <string>
#include "JSON/JSON.hpp"
#include <SFML/Graphics.hpp>

// Define resources location
std::string const RES_LOCATION = "../res/";

// Define the location of the config file
std::string const DEFAULT_CFG = "app.json";

// Define the location of the font file (could be in the config instead of here)
std::string const FONT_LOCATION = RES_LOCATION + "sansation.ttf";


class Config
{
private:
    j::Value mConfig;
    bool simulation_debug;

public:
    Config(std::string path);

    // enables / disables debug mode
    void switchDebug();
    bool getDebug();

    // returns read
    j::Value& getJsonRead()
    {
        return mConfig;
    }

    j::Value const& getJsonRead() const
    {
        return mConfig;
    }


    //window
    const double   window_simulation_width;
    const double  window_simulation_height;
    const double  window_stats_width;
    const double  window_control_width;
    const std::string  window_title;
    const int window_antialiasing_level;

    // stats
    const double stats_refresh_rate;
    const std::string stats_log_folder = "logs/";
    const std::string stats_log_prefix = "log_";
    const std::string stats_log_header = "# Plot with GNUPLOT : gnuplot -e \"plot for [i=1:6] 'log_0.txt' u i w l title columnheader(i)\"";

    // debug
    const sf::Color debug_text_color = sf::Color::White;
    const size_t default_debug_text_size = 20;


    //simulation
    const std::string simulation_world_texture;
    const std::string simulation_world_debug_texture;
    const int  simulation_world_size;
    const double  simulation_time_factor;
    const sf::Time  simulation_time_max_dt;

    // organic entity
    const std::string entity_texture_tracked = "target.png";
    const double entity_energy_critical = 5;

    //food generator
    const double food_generator_delta;

    // food
    const std::string food_texture;
    const double food_size; // deprecated
    const double food_energy; // deprecated


    // animal
    const bool animal_random_walk_low_energy;
//        const double animal_safe_distance = 1000;
    const double animal_meal_retention ;
    const float animal_feed_time;
    const float animal_delivery_time;
    const float animal_mating_time;
    const double animal_min_energy;
    const float animal_base_energy_consumption;

    // gerbils
    const double gerbil_max_speed;
    const double gerbil_mass;
    const double gerbil_energy_loss_factor;
    const double gerbil_view_range;
    const double gerbil_view_distance;
    const double gerbil_random_walk_jitter;
    const double gerbil_random_walk_radius;
    const double gerbil_random_walk_distance;
    const sf::Time gerbil_longevity;
    const double gerbil_energy_initial;
    const double gerbil_energy_min_mating_female;
    const double gerbil_energy_min_mating_male;
    const int gerbil_min_children;
    const int gerbil_max_children;
    const double gerbil_gestation_time;
    const double gerbil_energy_loss_female_per_child;
    const double gerbil_energy_loss_mating_male;
    const std::string gerbil_texture_male;
    const std::string gerbil_texture_female;
    const double gerbil_size;
    const double gerbil_min_age_mating;
    const double wave_gerbil_frequency;
    const double wave_gerbil_energy_loss_factor;


    // scorpion
    const double scorpion_max_speed;
    const double scorpion_mass;
    const double scorpion_energy_loss_factor;
    const double scorpion_view_range;
    const double scorpion_view_distance;
    const double scorpion_random_walk_jitter;
    const double scorpion_random_walk_radius;
    const double scorpion_random_walk_distance;
    const sf::Time scorpion_longevity;
    const double scorpion_energy_initial;
    const double scorpion_energy_min_mating_female;
    const double scorpion_energy_min_mating_male;
    const int scorpion_min_children;
    const int scorpion_max_children;
    const double scorpion_gestation_time;
    const double scorpion_energy_loss_female_per_child;
    const double scorpion_energy_loss_mating_male;
    const std::string scorpion_texture;
    const double scorpion_size;
    const double scorpion_min_age_mating;
    const double scorpion_sensor_radius;
    const double scorpion_minimal_score_for_action;
    const double scorpion_rotation_angle_precision;
    const std::string neuronal_scorpion_texture;
    // sensor
    const double sensor_intensity_threshold;
    const double sensor_inhibition_factor;
    const double sensor_inhibition_max;
    const double sensor_activation_duration;

    //wave
    const double wave_intensity_thickness_ratio;
    const double wave_intensity_threshold;
    const double wave_on_wave_marging;
    const double wave_default_energy;
    const double wave_default_radius;
    const double wave_default_mu;
    const double wave_default_speed;
    
    //rock
    const std::string rock_texture;
};

#endif // INFOSV_CONFIG_HPP


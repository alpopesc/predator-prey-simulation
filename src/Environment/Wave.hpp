#ifndef WAVE_H
#define WAVE_H

#include <iostream>
#include <Obstacle/CircularCollider.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Constants.hpp>
#include <list>
#include <utility>
#include <Utility/Vec2d.hpp>

class Obstacle;
class Rock;

class Wave : public CircularCollider
{
	public:
	
	//Constructeur et destructeur
	
	Wave(Vec2d _pt_depart, double enrg_init, double ray_init, 
		double _mu, double vit_propag,
		std::pair<double, double> un_arc = std::pair<double, double>(-PI, PI), 
		sf::Time temps = sf::Time::Zero);
	
	//Wave(Vec2d _pt_depart, std::pair<double, double> un_arc = std::pair<double, double>(-PI, PI));
	
	~Wave();
	
	//Getters
	double getRayonInitial() const;
	double getEnergieInitiale() const;
	//double getMu() const;
	//double getVitesseDefault() const;
	double getIntensityThicknessRatio() const;
	double getIntensityThreshold() const;
	double getIntensity() const;
	list<std::pair<double, double>> getFragments() const;
	
	//Onde au cours du temps (clock)
	void rayon_t();
	void energie_t();
	void intensite_t();
	bool IsWeak() const;
	bool IsCollidingWithPosition(Vec2d pos) const;
	void fragmentation(Obstacle* obs);
	void updateWave(sf::Time dt, std::list<Obstacle*> obstacles);
	
	//Dessin onde
	void drawWave(sf::RenderTarget& targetWindow) const;
	
	private:
	
	double energie_initiale;
	double rayon_initial;
	double mu;
	double vit_propagation;
	double energie;
	double intensite;
	list<std::pair<double, double>> fragments_angles;
	sf::Time clock;
	
};


#endif

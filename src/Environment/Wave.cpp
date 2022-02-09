#include "Wave.hpp"
#include <cmath>
#include <Application.hpp>
#include <Obstacle/Obstacle.hpp>
#include <Obstacle/Rock.hpp>
#include <iostream>

//Constructeur et destructeur

Wave::Wave(Vec2d _pt_depart, double enrg_init, double ray_init, 
	double _mu, double vit_propag, std::pair<double, double> un_arc, sf::Time temps)
	:CircularCollider(_pt_depart, ray_init), energie_initiale(enrg_init), rayon_initial(ray_init),
	mu(_mu), vit_propagation(vit_propag), 
	energie(getAppConfig().wave_default_energy), clock(temps)
	{
		intensite = getEnergieInitiale() / (2 * PI * getRayonInitial()); //t=0
		fragments_angles.push_back(un_arc);
		std::cout << "  wave constructed  " << std::endl;
	}

/*Wave::Wave(Vec2d _pt_depart, std::pair<double, double> un_arc)
	:CircularCollider(_pt_depart, getRayonInitial()), 
	energie(getEnergieInitiale()), vit_propagation(getVitesseDefault())
	{
		intensite = getEnergieInitiale() / (2 * PI * getRayonInitial()); //t=0
		fragments_angles.push_back(un_arc);
	}
*/

Wave::~Wave()
	{}

//Getters
double Wave::getRayonInitial() const
{
	return getAppConfig().wave_default_radius;
}

double Wave::getEnergieInitiale() const
{
	return getAppConfig().wave_default_energy;
}

/*
double Wave::getMu() const
{
	return 10000.0;//getAppConfig().wave_default_MU; --> erreur:
	               // 'class Config' has no member named 'wave_default_MU' ?

}
*/

/*
double Wave::getVitesseDefault() const
{
	return getAppConfig().wave_default_speed;
}
*/

double Wave::getIntensityThicknessRatio() const
{
	return getAppConfig().wave_intensity_thickness_ratio;
}

double Wave::getIntensityThreshold() const
{
	return getAppConfig().wave_intensity_threshold;
}

double Wave::getIntensity() const
{
	return intensite;
}

list<std::pair<double, double>> Wave::getFragments() const
{
	return fragments_angles;
}

//Onde au cours du temps
void Wave::rayon_t()
{
	rayon = (vit_propagation * clock.asSeconds()) + rayon_initial;
}

void Wave::energie_t()
{
	energie = energie_initiale * exp(-(rayon / mu));
}

void Wave::intensite_t()
{
	//intensite = (energie_initiale * exp(-(dt.asSeconds()) * (vit_propagation / mu))) / (2 * PI * rayon);
	intensite = (energie_initiale * exp(-(rayon / mu))) / (2 * PI * rayon);
}

bool Wave::IsWeak() const
{
	if(intensite < getIntensityThreshold()){
		return true;
	}else{
		return false;
	}
}


bool Wave::IsCollidingWithPosition(Vec2d pos) const
{	
	double angle ((pos - position).angle());
	for(auto& arc : fragments_angles){
		if(((angle >= arc.first) and (angle <= arc.second)) and abs(distance(pos, position) - getRadius()) <= getAppConfig().wave_on_wave_marging){
			return true;
		}
	}
	return false;
}


void Wave::fragmentation(Obstacle* obs)
{
	
	double alpha;  //problème avec alpha -> fragmentation ne fonctionne pas..
	alpha = 0.5; //(2 * (std::atan2(obs->getRadius(), getRadius() + obs->getRadius())) / DEG_TO_RAD);
	double angle ((obs->getPosition() - getPosition()).angle());
	for(auto& arc : fragments_angles){
		if((angle >= arc.first) and (angle <= arc.second) and isColliding(*obs)){
			
			std::pair<double, double> new_arc;
		
			new_arc.first = angle + (alpha/2.0);
			new_arc.second = arc.second; //PI
			
			arc.second = angle - (alpha/2);
			
			fragments_angles.push_back(new_arc);
		}
			
	}
		
}


void Wave::updateWave(sf::Time dt, std::list<Obstacle*> obstacles)
{
	clock += dt;
	rayon_t();
	energie_t();
	intensite_t();
	for (auto& obs : obstacles){
		fragmentation(obs);
	}
}

//Dessin onde
void Wave::drawWave(sf::RenderTarget& targetWindow) const
{
	sf::Color color = sf::Color::Black;
	
	for(auto paire_angles : fragments_angles){
		//std::cout << intensite;
		Arc arc (buildArc(paire_angles.first/DEG_TO_RAD, paire_angles.second/DEG_TO_RAD, rayon, position, color, 0.0,
		intensite * getIntensityThicknessRatio()));//rotation)
		targetWindow.draw(arc);
	}
	
}



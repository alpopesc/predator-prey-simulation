
#include "Sensor.hpp"
#include <Application.hpp>
#include <iostream>


Sensor::Sensor(size_t inx, NeuronalScorpion* home_p) :
	index(inx),home(home_p), activity(false),score(0), deg_inh(0)
{}
	
void Sensor::nullActivityAttribut()
{
	
	activity = false;
	score = 0;
	deg_inh = 0;
	
}         	

double Sensor::getIntensity()
{
	return getAppEnv().calcIntensity(getPosition());
}

void Sensor::inhibit()
{
	for(size_t i(3); i < 6; ++i){
		size_t j(0);
		j = ((i + index) % 8);
		(home -> getSensor(j)) -> setInhibition(score * getAppConfig().sensor_inhibition_factor);
	}
}
				
Vec2d Sensor::getPosition() 
{
	return home -> getPositionSensor(this);
}

void Sensor::updateScore()
{
	score = score + (1.0 - deg_inh);
}

void Sensor::update (sf::Time dt){
	updateActivity();
	if(activity){
		updateScore();
		inhibit();
	}
	
	//Debug
	
	std::cout << deg_inh << "    " << score << std::endl;
	
}
		
void Sensor::setInhibition(double inh)
{
	if(inh >= 0 and (inh + deg_inh) <= getAppConfig().sensor_inhibition_max){
		deg_inh = deg_inh + inh;
		
	}else if(inh > getAppConfig().sensor_inhibition_max and deg_inh == 0){
		deg_inh = getAppConfig().sensor_inhibition_max;
	}	
}
	
void Sensor::updateActivity()
{
	//DEBUG std::cout << getIntensity() << std::endl;
	if(getIntensity() > getAppConfig().sensor_intensity_threshold){
		activity = true;
		
	}
}

bool Sensor::getActivity() const
{
	return activity;
}

double Sensor::getScore() const{
	return score;
}

void Sensor::draw(sf::RenderTarget& targetWindow)
{
		
	sf::Color color = sf::Color::Black;
	
    if(activity and deg_inh > 0.2){
		color = sf::Color::Magenta;
		
	}else if(not activity and deg_inh > 0.2){
		color = sf::Color::Blue;
		
	}else if(activity and deg_inh < 0.2){
		color = sf:: Color::Red;
		
	}else{
		color = sf::Color::Green;
	}
	
	auto circle (buildCircle(getPosition(), 0.25*(home -> getRadius()), color));
	targetWindow.draw(circle);
	
	std::string sensor_index = std::to_string(index);
	auto text = buildText(sensor_index,
						getPosition(),
						getAppFont(),
						getAppConfig().default_debug_text_size,
                        sf::Color::Black);

	targetWindow.draw(text);
	
	//Debugg in order to find the mistake of misdirection
	Vec2d v;
	Vec2d w;
	v = home -> getPosition();
	w = getPosition() + (getScore() + 1) * (getPosition() - v);
	
	auto line = buildLine(v,w,sf::Color::Blue, 1);
    targetWindow.draw(line);

}

 



//Belongs to the attempt for a second methos to estimate de direction of a Neuronal Scorpion
Vec2d Sensor::getVector()
{	
	return getPosition() - home -> getPosition();
}















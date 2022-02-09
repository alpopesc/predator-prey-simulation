#include "WaveGerbil.hpp"
#include <Environment/Wave.hpp>
#include <Environment/Environment.hpp>
#include <FinalApplication.hpp>

//Constructeurs et destructeur
WaveGerbil::WaveGerbil(Vec2d posit)
	:Gerbil(posit), temps(sf::Time::Zero)
	{}
	
WaveGerbil::WaveGerbil(Vec2d posit, double niv_enrg, bool f_gender)	
	:Gerbil(posit, niv_enrg, f_gender)
	{}
	
WaveGerbil::~WaveGerbil()
{}

//Getter
double WaveGerbil::getWaveGerbilFrequence() const
{
	return 1.0/getAppConfig().wave_gerbil_frequency;
}

//Mise à jour
void WaveGerbil::update(sf::Time dt)
{
	Animal::update(dt);
	temps += dt;
	if(temps > sf::seconds(getWaveGerbilFrequence())){
		temps = sf::Time::Zero;
		emitWave();
	}
}

//Emission d'ondes
void WaveGerbil::emitWave() const
{
	Wave* onde = new Wave(position, getAppConfig().wave_default_energy,
	 getAppConfig().wave_default_radius, getAppConfig().wave_default_mu,
	 getAppConfig().wave_default_speed);
	 
	getAppEnv().addWave(onde);
}

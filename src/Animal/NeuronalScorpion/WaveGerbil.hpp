#ifndef WAVEGERBIL_H
#define WAVEGERBIL_H

#include <iostream>
#include <Animal/Gerbil.hpp>
#include <Utility/Utility.hpp>

class WaveGerbil : public Gerbil
{
	public:
	
	//Constructeurs et destructeur
	WaveGerbil(Vec2d posit, double niv_enrg, bool f_gender);
	WaveGerbil(Vec2d posit);
	
	~WaveGerbil();
	
	//Getter
	double getWaveGerbilFrequence() const;
	
	//Mise à jour
	void update(sf::Time dt) override;
	
	private:
	
	sf::Time temps;
	
	//Emission d'ondes
	void emitWave() const;
};

#endif

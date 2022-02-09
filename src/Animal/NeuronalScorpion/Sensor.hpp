#ifndef SENSOR_H
#define SENSOR_H

#include <Utility/Vec2d.hpp>
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>
#include <Animal/NeuronalScorpion/NeuronalScorpion.hpp>


class NeuronalScorpion;

class Sensor : public Updatable
{
	public:
	
	//Constructor  and Destructor
	Sensor(size_t inx, NeuronalScorpion* home_p);
	
	void nullActivityAttribut();         	
	
	//turns the attribut deg_inh and score to 0
	
	
	//Getters
	double getIntensity(); 
	bool getActivity() const;
	double getScore() const;
	Vec2d getPosition(); 
	Vec2d getVector();
	
	/*getVector returns the Vector that goes from a scorpions position to the the this sensor
	 * This is used to code an alternative calculation of the estimated direction.
	 * It could have been coded in NeuronalScorpion too*/
	
	/* For some reason getPosition, getIntensity and getVector can not be const
	 * getVector and getIntensity uses getPosition which is why those two 
	 * function cant be const but I had not time to find out why getPosition cannot be const*/


	void update (sf::Time dt) override;	  //The parameter is not really used
	void setInhibition(double inh);
	void draw(sf::RenderTarget& targetWindow);
	
	/* updates using the methods that were put in private
	 * setInhinition is used by inhibit whhich the again inhibits
	 * the sensors whose index is calculated with a formula
	 */
	
	private: 
	void updateScore();
	void updateActivity();
	void inhibit();

	
	
	private:
	
	size_t index;
	NeuronalScorpion* home;
	bool activity; //if on then true else false
	double score;
	double deg_inh;


};

#endif

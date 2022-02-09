#ifndef NEURONALSCORPION_H
#define NEURONALSCORPION_H

#include <array>
#include <map>
#include <iostream>
#include <Animal/NeuronalScorpion/Sensor.hpp>
#include <Animal/Scorpion.hpp>
#include <Utility/Utility.hpp>


class Sensor;

enum State_N {FOOD_IN_SIGHT_N, MOVING_N, IDLE_N, WANDERING_N};

class NeuronalScorpion : public Scorpion
{
	public:
	
	//Constructor and Destructor
	NeuronalScorpion(Vec2d pos);
	~NeuronalScorpion();
	
	
	Vec2d getPositionSensor(Sensor* sensor) const;
	 
	//ATTENTION to not use operator[] because the method cant be const anymore otherwise
	/* As the name says it calculates position of a sensor using the map 
	 * resp_angles. It is used by class Sensors to calculate its position
	 */
	 
	void update(sf::Time dt) override;
	
	/* update similar to update in class Animal 
	 * but calls also the update method of class Sensor
	 */
	 
	void updateState(sf::Time dt) override;
	
	/* update State is similar to the one in Animal
	 */
	 
	Vec2d calc_target_direction() const;
	
	/* Calculates the estimates direction depending on the score of each 
	 * sensor and the according position the sensor.
	 * ATTENTION However I suspect that this method does not work 
	 * properly and I do not know why
	 */
	 
	Sensor* getSensor(size_t inx);
			 
	// This method is made to improve encapsulation so that we do not have to acces a Sensor through its array
	/* We can acces the sensor with a certain index (0-7) which is 
	 * usefull for inhibiting other sensors with a given index.
	 */
	 
	void AnalyseEnvironment() override;	 
	
	/* Method Similar to the one in Scorpion except for not analyzing 
	 * potential mates or predators but detecting waves
	*/
	
	bool sensing() const;
	
	/* Iterates on all the Sensors and test if one of them is active
	 * if one sensor -> return true
	 * else -> return false
	 */
	 
	void resetSensors();
	
	/* resets all the sensors to initial condition meaning:
	 * score = 0
	 * degree of inhibition = 0
	 * activity = false;
	 */
	
	void drawDebugText(sf::RenderTarget& targetWindow) const override;
	
	/* Similar method to the one in Animal */
	
	void draw(sf::RenderTarget& targetWindow) const override;
	
	/* Similar method to the one in Animal but calls also on the 
	 * draw method of class sensor in order to draw the sensors and it 
	 * nummerotation
	 */


	
	
	private:
	std::map<Sensor*, int> resp_angles;
	std::array<Sensor*, 8> sensors;
	State_N state_n;
	
	//counts how much time left in the respective state;
	sf::Time compteur_move; 
	sf::Time compteur_idle;
	sf::Time compteur_active;  
	
	Vec2d direction_estimation; 

	
};

#endif

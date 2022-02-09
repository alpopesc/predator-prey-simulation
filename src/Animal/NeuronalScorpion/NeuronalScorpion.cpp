#include "NeuronalScorpion.hpp"
#include <Application.hpp>
#include <cmath>
#include <iostream>
#include <Utility/Vec2d.hpp>
#include <Random/Uniform.hpp>
#include <Utility/Constants.hpp>
#include <map>




	
//NeuronalScorpion() = default
NeuronalScorpion::NeuronalScorpion(Vec2d pos)
	:Scorpion(pos),state_n(WANDERING_N), compteur_move(sf::Time::Zero), compteur_idle(sf::Time::Zero), compteur_active(sf::Time::Zero), direction_estimation(1,0)
{
	for(size_t i(0); i < 8; ++i){
		sensors[i] = new Sensor(i,this);
	}
	
	resp_angles[sensors[0]] = 18;
	resp_angles[sensors[1]] = 54;
	resp_angles[sensors[2]] = 90;
	resp_angles[sensors[3]] = 140;
	resp_angles[sensors[4]] = -140;
	resp_angles[sensors[5]] = -90;
	resp_angles[sensors[6]] = -54;
	resp_angles[sensors[7]] = -18;
}

NeuronalScorpion::~NeuronalScorpion()
//Will liberate the memory of all the Sensors
{
		for(size_t i(0); i < 8; ++i){
			delete sensors[i];
			sensors[i] = nullptr;
		}
		
		resp_angles.clear();
}

Vec2d NeuronalScorpion::getPositionSensor(Sensor* sensor) const
{
	Vec2d ret(cos(resp_angles.at(sensor)), sin(resp_angles.at(sensor)));	//Definition of position of vector in local coordinates
	// First Method: return ConvertToGlobalCoord(getAppConfig().scorpion_sensor_radius * ret.normalised());
	return position + getAppConfig().scorpion_sensor_radius*(cos(resp_angles.at(sensor) * DEG_TO_RAD) * direction.normalised() + sin(resp_angles.at(sensor)* DEG_TO_RAD) * direction.normal()); 
}


void NeuronalScorpion::update(sf::Time dt){
	
	Vec2d force; //= direction.normalised(); //Je le initialiser pour eviter des probèmes
	
	for(size_t i(0); i < 8; i++){
		sensors[i] -> update(dt);
	}
    
    updateState(dt);
    
    if(sensing() and compteur_active > sf::Time::Zero){
		compteur_active -= dt;
	}else if(sensing()){
		direction_estimation = calc_target_direction();
		
		resetSensors();
		compteur_active = sf::Time::Zero;
		
	}

    switch(state_n) {
		
		
	case WANDERING_N: {
        force = randomWalk();
        break;
    }	

    case FOOD_IN_SIGHT_N: {
		target = dinner -> getPosition();
        force = force_attrac();
        break;
    }
    
    case IDLE_N: {
		force = Vec2d(0,0);
	}
	
	case MOVING_N: {
		if((abs(direction.angle() - direction_estimation.angle())) > getAppConfig().scorpion_rotation_angle_precision /*and 
		(abs(direction.angle() - direction_estimation.angle()) < (360 * DEG_TO_RAD - getAppConfig().scorpion_rotation_angle_precision))*/) {
			//direction = -direction_estimation;
			//vitesse = 0;
			//force = Vec2d(0,0);	//Not sure if I understood the description should it brake or just change direction?
			target = position + direction_estimation;
			force = force_attrac();
		}else{
			target = 10 * (position + direction);
		}
	}
	
    default: {}
    }
    
    
    //Debugging
    cout << "Direction : " << direction << endl;
    cout << "Calculated Direction" << calc_target_direction() << endl;
    cout << "Direction_Estimation : " << direction_estimation << endl;
    cout << "Force : " << force << endl;
    cout << "Current Target : " << current_target << endl;
    cout << "Target : " << target << endl;
    cout << "Vitesse : " << vitesse << endl;
    cout << " Sensing : " << sensing() << endl;
    cout << " Position : " << position << endl;
    /*
    cout << "Direction : " << direction << endl;
    cout << "Direction : " << direction << endl;
    cout << "Direction : " << direction << endl;
    cout << "Direction : " << direction << endl;
    cout << "Direction : " << direction << endl;
    */
	

	OrganicEntity::update(dt);
    perte_energie(dt);
	maj_deplacement(force, dt);
    
	dinner = nullptr;
    dinner_list.clear();

}

void NeuronalScorpion::updateState(sf::Time dt){

     AnalyseEnvironment();
     
    
    if(compteur_idle > sf::Time::Zero){ // maybe compteur should be also tested to be bigger then dt in order to avoid negative numbers. But that's just detail
		compteur_idle -= dt;
	}else{
		compteur_idle = sf::Time::Zero;
	}
	
	if(compteur_move > sf::Time::Zero){ 
		compteur_move -= dt;
	}else{
		compteur_move = sf::Time::Zero;
	}
	

	
	if(dinner != nullptr){
		state_n = FOOD_IN_SIGHT_N;	//FOOD_IN_SIGHT has in this case the highest priority
		compteur_move = sf::Time::Zero;
		compteur_idle = sf::Time::Zero;
	}else{
		
		switch(state_n){

			case WANDERING_N : {
				if(sensing()){
					state_n = IDLE_N;
					compteur_idle = sf::seconds(5);
				}else{
					state_n = WANDERING_N;
				}
				break;
			}
		
			
			case IDLE_N : {

				if(sensing() and calc_target_direction().length() > getAppConfig().scorpion_minimal_score_for_action){
					state_n = MOVING_N;
					compteur_move = sf::seconds(3);
					compteur_idle = sf::Time::Zero;
				}else if(compteur_idle > sf::Time::Zero){
					state_n = IDLE_N;
				}else{
					state_n = WANDERING_N;
				}
				break;
			}
		
		
			case MOVING_N : {
				
				//direction_estimation = calc_target_direction();
				
				if(sensing() and compteur_active == sf::Time::Zero){			//This if Im not sure if it is necessary. Did not understand the description
					compteur_move = sf::seconds(0);
					compteur_idle = sf::seconds(5);
					state_n = IDLE_N;
								
				}else if(compteur_move > sf::Time::Zero){
					state_n = MOVING_N;
					
				}else{
					state_n= IDLE_N;
					resetSensors();
					compteur_idle = sf::seconds(5);
					compteur_move = sf::Time::Zero;
					//Debugging
					/*
					for(size_t i(0); i< 10000; ++i){
						cout << "asjdlkjshdfkajhflkahjflakjhfalksjdalkjfakldjfalksd" << endl;
					}*/
				}
				break;
			}
			
			
			case FOOD_IN_SIGHT_N : {
				if(dinner != nullptr){
					state_n = FOOD_IN_SIGHT_N;
				}else{
					compteur_idle = sf::seconds(5);
					state_n = IDLE_N;
				}
				break;
			}
		
			default : {}
		}
		
	}
	
	if(sensing() and compteur_active == sf::Time::Zero){
		compteur_active = sf::seconds(getAppConfig().sensor_activation_duration);
	}
}


Vec2d NeuronalScorpion::calc_target_direction() const
{	
	Vec2d ret(0,0);
	for(size_t i(0); i < 8; ++i ){
		ret += (sensors[i] -> getScore()) * (sensors[i] -> getVector());
	}
	return ret;
		
	/* //Im pretty sure that this method is faulty. At least according to my debug methods the bug should be in this method 
	Vec2d v(0,0);
	Vec2d w(0,0);
	for(size_t i(0); i < 8; ++i){
		w = sensors[i] -> getScore() * Vec2d(cos(resp_angles.at(sensors[i]) * DEG_TO_RAD + getRotation()), sin(resp_angles.at(sensors[i]) * DEG_TO_RAD + getRotation()));
		v = v + getAppConfig().scorpion_sensor_radius * w;
	}
	return v;
	*/
}
	
Sensor* NeuronalScorpion::getSensor(size_t inx)		 // This method is made to improve encapsulation so that we do not have to acces a Sensor through its array
{
	return sensors[inx];
}

void NeuronalScorpion::resetSensors()
{
	for(size_t i(0); i < 8; ++i){
		sensors[i] -> nullActivityAttribut();
	}
}


void NeuronalScorpion::AnalyseEnvironment()	 //Method Similar to the one in Scorpion except for not analyzing potential mates or predators but detecting waves
{
	dinner = nullptr;
    dinner_list.clear();

    for(auto entity : getAppEnv().getEntitiesInSightForAnimal(this)) {
        if(entity != nullptr and eatable(entity)) {   
            dinner_list.push_back(entity); 
		}
	}        

	dinner = chooseNearestEntity(dinner_list);      
}

bool NeuronalScorpion::sensing() const{
	for(size_t i(0); i < 8; ++i){
		if(sensors[i] -> getActivity() == true){
			return true;
		}
	}
	return false;
}

void NeuronalScorpion::drawDebugText(sf::RenderTarget& targetWindow) const
{
    //OrganicEntity::drawDebugText(targetWindow);
    sf::Color debug_color(getAppConfig().debug_text_color); //The color is magenta if it's female and otherwise it's Black
    std::string state_text("DEFAULT");
		
    switch(state_n) {

    case FOOD_IN_SIGHT_N: {
        state_text = "FOOD_IN_SIGHT";
        break;
    }

    case WANDERING_N: {
        state_text = "WANDERING";
        break;
    }
    
    case MOVING_N: {
		state_text = "MOVING";
		break;
	}
	
	case IDLE_N: {
		state_text = "IDLE";
		break;
	}
    }

    if(femelle) {
        debug_color = sf::Color::Magenta;
    } else {
        debug_color = sf::Color::Black;
    }

    auto text = buildText(state_text,
                          position + getRandomWalkDistance()*direction.normalised(),
                          getAppFont(),
                          getAppConfig().default_debug_text_size,
                          debug_color,
                          getRotation() / DEG_TO_RAD + 90); // si nécessaire

    targetWindow.draw(text);

    //Drawing the energy of the Animal
    std::string energy_text(to_nice_string(energy));
    auto text_enrg = buildText(energy_text,
                               position + getRandomWalkDistance()*1.5*direction.normalised(),
                               getAppFont(),
                               getAppConfig().default_debug_text_size,
                               debug_color,
                               getRotation() / DEG_TO_RAD + 90); // si nécessaire
    targetWindow.draw(text_enrg);
}

void NeuronalScorpion::draw(sf::RenderTarget& targetWindow) const{
	Animal::draw(targetWindow);
	
	if(isDebugOn()){
		for(size_t i(0); i < 8; ++i){
			sensors[i] -> draw(targetWindow);
		}
	
	
	//Debugg in order to find the mistake of misdirection
	Vec2d v;
	Vec2d w;
	v = getPosition();
	w = getPosition() + calc_target_direction();
	
	auto line = buildLine(v,w,sf::Color::Red, 1);
    targetWindow.draw(line);
  
	}	
}



	
	
	
	
	
	
	
	
	

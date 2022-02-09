#include "Animal.hpp"
#include <Application.hpp>
#include <cmath>
#include <iostream>
#include <Utility/Vec2d.hpp>
#include <Random/Uniform.hpp>
#include <Application.hpp>

//Setters and Getters
void Animal::setRotation(double angle)
{
    direction = {cos(angle), sin(angle)};
}
double Animal::getRotation() const
{
    return direction.angle();
}
void Animal::setTargetPosition(Vec2d new_targ)
{
    target = new_targ;
}

Vec2d Animal::getSpeedVector() const
{
    return direction * vitesse;
}

bool Animal::isFemale() const{
	return femelle;
}

bool Animal::isPregnant() const{
	return pregnant;
}	

double Animal::getDepenseEnergyBase() const
{
    return getAppConfig().animal_base_energy_consumption;
}

float Animal::getMatingTime() const
{
	return getAppConfig().animal_mating_time;
}

float Animal::getDeliveryTime() const
{
	return getAppConfig().animal_delivery_time;
}


void Animal::perte_energie(sf::Time dt)
{
    double perte_energie(0.0);

    perte_energie = getDepenseEnergyBase() + ((vitesse * getFacteurPerteEnergy()) * dt.asSeconds());

    energy = energy - perte_energie;
}




Vec2d Animal::breakingForce(){
	Vec2d ret_force(-1,0);
	ret_force = -direction;

	ret_force = ret_force * (vitesse * vitesse /(2* distance(position, target))) * getMass();

	return ret_force;
}


//Do it like they do it on DiscoveryChannel o/ o/
void Animal::meet(OrganicEntity* mate){  //The switch takes as variable the gender of the current instance. If it's female it's gonna get pregnant.
	if(matable(mate) and mate -> matable(this) and state != MATING){	
		
		state = MATING;	
		compteur_mating = sf::seconds(getMatingTime());		
		mate -> meet(this);	//The breakingpoint is when the female gets pregnant
		
									
		if(femelle){

			pregnant = true;
			nmb_offspring = uniform(getMinChild(), getMaxChild());
			energy = energy - nmb_offspring * getEnergyLossPerChildF();
			compteur_gestation = sf::seconds(getMatingTime()); // I initialized the gestation time to the Mating time in order not to pass directly from mating to giving birth (concerning the females)
			compteur_feeding  = sf::Time::Zero;
			
		}else{
			pregnant = false;
			energy = energy - getEnergyLossM();
			compteur_feeding = sf::Time::Zero;
			
		}
	}
}



//Movment d'un Animal


void Animal::updateState(sf::Time dt)  //Here we could also use a switch but I do not know if the other states can be implemented in the same switch. So I chose to use if an else.
{
    /* The states with highest priority are updated the last.
     * For exemple RUNNING_AWAY has highest priority because it ensures the animals survival. Therefore the state RUNNING AWAY is updated last in this list of conditions.
     */
     
     
     AnalyseEnvironment();
     
     
    //Feeding iteration 
    if(compteur_feeding > sf::Time::Zero){
		compteur_feeding -= dt;
	}else{
		compteur_feeding = sf::Time::Zero;
	}
	
	//Mating interation
	if(compteur_mating > sf::Time::Zero){
		compteur_mating -= dt;
	}else{
		compteur_mating = sf::Time::Zero;
	}
	
	
	//Giving birth iteration
	if(compteur_givingBirth > sf::Time::Zero){
		compteur_givingBirth -= dt;
	}else{
		compteur_givingBirth = sf::Time::Zero;
	}
	
	
	//Gestation iteration
	if(compteur_gestation > sf::Time::Zero){
		compteur_gestation -= dt;
	}else{
		compteur_gestation = sf::Time::Zero;
	}
	

	
	
	switch(state){
		
		case WANDERING : 
		case FOOD_IN_SIGHT : {
			
			if(compteur_gestation == sf::Time::Zero and femelle and pregnant){
				state = GIVING_BIRTH;
				compteur_givingBirth = sf::seconds(getDeliveryTime());
				compteur_feeding = sf::Time::Zero;
				give_birth();
				
			}else if(not predators.empty()){
				state = RUNNING_AWAY;
				compteur_feeding = sf::Time::Zero;
				predators_memory = predators;
				
			}else if(crush != nullptr){
				state = MATE_IN_SIGHT;
				compteur_feeding = sf::Time::Zero;
				
			}else if(dinner != nullptr and isColliding(*dinner)){
				state = FEEDING;
				compteur_feeding = sf::seconds(getAppConfig().animal_feed_time); 
				energy += (getAppConfig().animal_meal_retention * dinner->getEnergy());
				dinner->setEnergy(0.0);	
				
			}else if (dinner != nullptr) {
				state = FOOD_IN_SIGHT;
				
			}else{
				state = WANDERING;
			}
	
			break;
		}
		

		case FEEDING : {
			
			if(compteur_gestation == sf::Time::Zero and femelle and pregnant){
				state = GIVING_BIRTH;
				compteur_givingBirth = sf::seconds(getDeliveryTime());
				compteur_feeding = sf::Time::Zero;
				give_birth();
				
			}else if(not predators.empty()){
				state = RUNNING_AWAY;
				compteur_feeding = sf::Time::Zero;
				predators_memory = predators;
				
			}else if(crush != nullptr){
				state = MATE_IN_SIGHT;
				compteur_feeding = sf::Time::Zero;
				
			}else if(compteur_feeding > sf::Time::Zero){
				state = FEEDING;
				
			}else if(compteur_feeding == sf::Time::Zero){
				state = WANDERING;
				
				//Switching to Food in Sight
				if (dinner != nullptr) {
				state = FOOD_IN_SIGHT;
				
				}else{
					state = WANDERING;
				}
			}
			break;
		}
		
		case MATE_IN_SIGHT : {
			state = WANDERING;
			
			if(compteur_gestation == sf::Time::Zero and femelle and pregnant){
				state = GIVING_BIRTH;
				compteur_givingBirth = sf::seconds(getDeliveryTime());
				compteur_feeding = sf::Time::Zero;
				give_birth();
				
			}else if(not predators.empty()){								//I put it as lower priority then giving birth
				state = RUNNING_AWAY;
				predators_memory = predators;
				
			}else if(crush != nullptr and isColliding(*crush)){
				meet(crush);
				
			}else if(crush != nullptr){
				state = MATE_IN_SIGHT;
				
				
			}else{
				state = WANDERING;
			}
			break;
		}
				
			
					
		case MATING : {
			
			if(not predators.empty()){
				state = RUNNING_AWAY;
				predators_memory = predators;
				compteur_mating = sf::Time::Zero;
				
			}else if(compteur_mating == sf::Time::Zero){

				if(femelle){
					compteur_gestation = sf::seconds(getGestationTime());
				}
				
				if(crush != nullptr){
					state = MATE_IN_SIGHT;
					compteur_feeding = sf::Time::Zero;
					
				}else if(dinner != nullptr){
					state = FOOD_IN_SIGHT;
					
				}else{
					state = WANDERING;
				}
								
			}else if(compteur_mating > sf::Time::Zero){
				state = MATING;
			}
			
			break;
		}
				
		
		
		case GIVING_BIRTH : {				//In the case of giving birth it does not seem very logic to me that any entity would be able to run away. 
	
			if(compteur_givingBirth > sf::Time::Zero){
				state = GIVING_BIRTH;
				
			}else if(compteur_givingBirth == sf::Time::Zero){
				pregnant =  false;
				
				if(not predators.empty()){
					state = RUNNING_AWAY;
					predators_memory = predators;
				
				}else if(crush != nullptr){
					state = MATE_IN_SIGHT;
					compteur_feeding = sf::Time::Zero;
					
				}else if (dinner != nullptr) {
					state = FOOD_IN_SIGHT; 
				}else{
					state = WANDERING;	
				}
			}
			break;
		}
		
		case RUNNING_AWAY : {
			if(distance(chooseNearestPosition(predators_memory), position) <  getAppConfig().simulation_world_size/2){
				state = RUNNING_AWAY;
				energy -= getLossFactor();
					
			}else{
				predators_memory.clear();
				state = WANDERING;			//In this case his can not switch immediatly to the other state because he will regenerate first before going for food.
			}								//Would be nice to introduce a timer too.	
			break;
		}
		

		
		default : {
	
		}
	}
}


void Animal::update(sf::Time dt)
{
    //std::cout << age.asSeconds() << std::endl;
    
	Vec2d force = direction.normalised(); //Je le initialiser pour eviter des probèmes
    updateState(dt);
    
    //This operations is made in order that draw the mating animation:
    if(counter_draw < 5){
		counter_draw += counter_draw;
	}else{
		counter_draw = 1;
	}

    
    switch(state) {
		
		
	case WANDERING: {
        force = randomWalk();
        break;
    }	

    case FOOD_IN_SIGHT: {
		target = dinner -> getPosition();
        force = force_attrac();
        break;
    }
    
    case FEEDING: {
		
		
		if(distance(position, target) > 0.5*rayon){  //If we do the condition "if isEqual(vitesse,0)" it does not work that good. It bugs sometimes
			force = breakingForce();
			//compteur_feeding = compteur_feeding + dt;  //There are braking bugs if we do it like this. Therefore we'll just adjust the value for feedingTime in appTest.json
		}else{								//The Debug mode bugs and I have the impression that it the mice  
			vitesse = 0;
			//force = Vec2d(0,0);
		}
		
		//vitesse = 0; 			//Without the bonus
		//force = Vec2d(0,0)

		
    	break;
	}
    
    case RUNNING_AWAY: {
        force = force_fuite();
        break;
    }

    case MATE_IN_SIGHT: {
		target = crush -> getPosition();
        force = force_attrac();
        break;
    }

    case MATING: {
		
		if(vitesse > 5){
			force = -50 * direction ;  //would be nice to make another constant in json file called birth braking force
		}else{
			vitesse = 0;
			//force = Vec2d(0,0);
		}
		
        break;
    }

    case GIVING_BIRTH: {
		if(vitesse > 5){
			force = -50 * direction ;  //would be nice to make another constant in json file called birth braking force
		}else{
			vitesse = 0;
			//force = Vec2d(0,0);
		}
			
        break;
        
    }


    default: {
    }
    }

	
    //std::cout<< "norme de force " << force.length() << std::endl;		//Decommenter our le debug

	maj_deplacement(force, dt);
    OrganicEntity::update(dt);
    perte_energie(dt);
    
    
    crush = nullptr;
	dinner = nullptr;
    dinner_list.clear();
    crush_list.clear();
    predators.clear();
}


Vec2d Animal::force_fuite(){
	
	Vec2d force_tot(0,0);
	for(size_t i(0); i < predators_memory.size(); ++i){
		double constant1 = 5000;
		double constant2 = 1.2 ;//force_attrac_p(predators[i]).length();
		Vec2d force_vect(0,0);
		force_vect = (position - predators_memory[i]);
		force_tot += constant1 * (force_vect / pow(force_vect.length(),constant2));
	}
	return force_tot;
}
		

//calcul de la force d'attraction (update):

Vec2d Animal::force_attrac_p(Vec2d parameter)		// same fonction as force_attrac but with parameter and inverse.
{
	Vec2d to_target(0,0);
    to_target =  parameter - position;


    if(to_target.length() == 0.0) {
        Vec2d force_attrac_r(0.0, 0.0);
        return force_attrac_r;
    }

    Deceleration deceleration(faible);

    double v_compare = (to_target.length() / (deceleration * 0.1 ));			
    double v(0.0);

    if(v_compare > getMaxSpeed()) {
        v = v_compare;
    } else {
        v = getMaxSpeed();
    }

    Vec2d v_target;
    v_target = (to_target/ to_target.length()) * v;
    Vec2d force_attrac_r(v_target - getSpeedVector());
   
    return force_attrac_r;
	
}


Vec2d Animal::force_attrac()
{
    Vec2d to_target(0,0);
    to_target = target - position;


    if(to_target.length() == 0.0) {
        Vec2d force_attrac_r(0.0, 0.0);
        return force_attrac_r;
    }

    Deceleration deceleration(faible);



    double v_compare = (to_target.length() / (deceleration * 0.1 ));			//Belongs to the attempt to code the simulation in a toric world

    double v(0.0);

    if(v_compare > getMaxSpeed()) {
        v = v_compare;
    } else {
        v = getMaxSpeed();
    }
	
	
    Vec2d v_target;
    v_target = (to_target/ to_target.length()) * v;
    //std::cout << "v_target" << v_target.length() << std::endl;

    Vec2d force_attrac_r(v_target - getSpeedVector());
   //std::cout<<"Affiche norme de force attr" << force_attrac_r.length() << std::endl;
    return force_attrac_r;
    
}



//mise à jour des données de déplacement (update):
void Animal::maj_deplacement(Vec2d f, sf::Time dt)
{

    Vec2d v_courante;
    v_courante = vitesse * direction.normalised();

    //2
    Vec2d acc(0,0);
    acc = f/getMass();													// We could construct here a throw in case there is a division by 0.

    //3
    v_courante += (acc * dt.asSeconds());

    //4
    direction = v_courante.normalised();

    //5
    if(v_courante.length() > getMaxSpeed()) {
        v_courante = direction.normalised() * getMaxSpeed();
    }

    vitesse = v_courante.length();

    //6
    if(v_courante != Vec2d(0,0)) {
        move(v_courante * dt.asSeconds());
    }

	/*//Debugging
    std::cout << "position " << position << std::endl;
    std::cout << "direction" << direction << std::endl;
    std::cout << "target" << current_target << endl;
    std::cout << vitesse << std::endl;
    std::cout <<  "gestation " << femelle<< "    "<< compteur_gestation.asSeconds() << std::endl;
    std::cout <<  "feeding" << "    "<< compteur_feeding.asSeconds() << std::endl;
    std::cout << "norme_force_f" << force_fuite().length() << std::endl;
    if(not predators_memory.empty()){
		std::cout << "predator position" << predators_memory[0] << std::endl;
	}
	*/
}



// promenade au hasard (update):
Vec2d Animal::ConvertToGlobalCoord(Vec2d local) const
{
    // create a transformation matrix
    sf::Transform matTransform;

    // first, translate
    matTransform.translate(position);

    // then rotate      //convertir angle en radian
    matTransform.rotate(direction.angle()/DEG_TO_RAD);

    // now transform the point
    Vec2d global = matTransform.transformPoint(local);

    return global;
}

Vec2d Animal::randomWalk()
{
    //regarder les figures dans l'énoncé pour comprendre
    Vec2d random_vec(uniform(-1.0, 1.0), uniform(-1.0, 1.0));

    current_target += random_vec * getRandomWalkJitter();

    current_target = current_target.normalised() * getRandomWalkRadius();

    Vec2d moved_current_target = current_target + Vec2d(getRandomWalkDistance(),0);

    Vec2d x_cible(ConvertToGlobalCoord(moved_current_target));
    //force régissant le mouvement :
    Vec2d f(x_cible - position);

    return f;
}



//Dessin de l'Animal

void Animal::drawDebugText(sf::RenderTarget& targetWindow) const
{
    //OrganicEntity::drawDebugText(targetWindow);
    sf::Color debug_color(getAppConfig().debug_text_color); //The color is magenta if it's female and otherwise it's Black
    std::string state_text("DEFAULT");
		
    switch(state) {

    case FOOD_IN_SIGHT: {
        state_text = "FOOD_IN_SIGHT";
        break;
    }

    case WANDERING: {
        state_text = "WANDERING";
        break;
    }

    case FEEDING: {
        state_text = "FEEDING";
        break;
    }

    case RUNNING_AWAY: {
        state_text = "RUNNING_AWAY";
        break;
    }

    case MATE_IN_SIGHT: {
        state_text = "MATE_IN_SIGHT";
        break;
    }

    case MATING: {
        state_text = "MATING";
        break;
    }
	
    case GIVING_BIRTH: {
        state_text = "GIVING_BIRTH";
        break;
    }
	
    default: {
		/*
        state_text = "DOING_NOTHING";
        break;
        */
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



void Animal::drawVision(sf::RenderTarget& targetWindow) const
{
    double start_angle(-((getViewRange()/2.0)/DEG_TO_RAD));
    double end_angle(((getViewRange()/2.0)/DEG_TO_RAD));

    sf::Color color = sf::Color::Black;
    color.a = 16; // light, transparent grey
    Arc arc(buildArc(start_angle, end_angle, getViewDistance(), position, color,
                     (this->getRotation()/DEG_TO_RAD)));//arc (centré sur vecteur direction)tourne

    targetWindow.draw(arc);
}

void Animal::drawAnnulus(sf::RenderTarget& targetWindow) const  	//Attention a ordre de build. Si c'est pas dans le bon ordre ca marche pas.Il y a une dependence entre current_target et direction
{
    Vec2d moved_current_target = current_target + Vec2d(getRandomWalkDistance(), 0);
    sf::Color color = sf::Color::Black;
    color.a = 16;
    auto circle (buildCircle(ConvertToGlobalCoord(moved_current_target), 5.0, sf::Color::Blue));
    auto annulus (buildAnnulus(position + direction.normalised()*getRandomWalkDistance(), getRandomWalkRadius(), sf::Color(255, 150, 0),2,color));
    targetWindow.draw(circle);
    targetWindow.draw(annulus);

}

void Animal::drawPregnancyAnnulus(sf::RenderTarget& targetWindow) const
{
	sf::Color color = sf::Color::Magenta;
	double thickness(3);
	auto annulus(buildAnnulus(position, getRadius(), color, thickness));
	targetWindow.draw(annulus);
}

void Animal::drawMating(sf::RenderTarget& targetWindow) const{
	

	sf::Texture& texture_m = getAppTexture(getMatingTexture());
	 auto image_to_draw_m(buildSprite(position, getMatingTextureSize(), texture_m,
                                   (this->getRotation()/DEG_TO_RAD)));//image de animal (centré sur vecteur direction) tourne
    //this->setRotation(direction.angle()/DEG_TO_RAD)

    targetWindow.draw(image_to_draw_m);

}


void Animal::draw(sf::RenderTarget& targetWindow) const
{

    if(isDebugOn()) {
        drawCollisionZone(targetWindow);
        drawVision(targetWindow);
        drawDebugText(targetWindow);
        if(state == WANDERING) {
            drawAnnulus(targetWindow);
        }
        if(pregnant){
			drawPregnancyAnnulus(targetWindow);
		}
    }
    
   

    sf::Texture& texture = getAppTexture(getTexture());
    auto image_to_draw(buildSprite(position, 2.0 * rayon, texture,
                                   (this->getRotation()/DEG_TO_RAD)));//image de animal (centré sur vecteur direction) tourne
    //this->setRotation(direction.angle()/DEG_TO_RAD)
    targetWindow.draw(image_to_draw);
    
    
    if(state == MATING){
		drawMating(targetWindow);
	}
}





bool Animal::isEntityInSight(OrganicEntity* entity) const
{
	Vec2d d(0,0);
	
	if (entity != nullptr){ // Il faut faire attention de toujours considerer les cas ou on a des nullptr: c'est la cause du segmentation fault.
		//std::cout << "nullptr" << std::endl;
		d = (entity->getPosition()) - position;
			if(isEqual((sqrt(d.lengthSquared())), 0.0)) {
			return true;
			}
	}

    if((sqrt(d.lengthSquared()) <= getViewDistance()) and
       (direction.dot(d.normalised()) >= (cos((getViewRange() + 0.001)/2.0)))) {
        return true; //direction aussi normalisé??
    }

    return false;
}



// version Vec2d
//A method that calculates the nearest target could come in handy.      fait partie de version Vec2d

Vec2d Animal::chooseNearestPosition(const std::vector<Vec2d>& pos_entities)
{
    Vec2d final_target = pos_entities.front();
    for(auto pos_entity : pos_entities) {
        if(distance(pos_entity,position) <= distance(final_target, position)) {
            final_target = pos_entity;
        }
    }
    return final_target;
}



OrganicEntity* Animal::chooseNearestEntity(const std::list <OrganicEntity*>& entities){
	
	OrganicEntity* nearest_entity(entities.front());
	
	for(auto entity : entities) {
        if(distance(entity -> getPosition(),position) <= distance(nearest_entity -> getPosition(), position)) {
            nearest_entity = entity;
        }
    }
    return nearest_entity;
}
	
	



//Constructeur
Animal::Animal(Vec2d posit,double a_size, double enrg, double vit, Vec2d dir,
               Vec2d trg, Vec2d c_trg, bool gender_f)					//  vit and dir have to be parametrised otherwise the initialised target is gonna rest placed during the whole test
    : OrganicEntity(posit,a_size, enrg),vitesse(vit), direction(dir),
      target(trg), current_target(c_trg), femelle(gender_f), dinner(nullptr), crush(nullptr), pregnant(false),nmb_offspring(0), state(WANDERING), 
      compteur_feeding(sf::Time::Zero), compteur_mating(sf::Time::Zero), compteur_gestation(sf::Time::Zero), compteur_givingBirth(sf::Time::Zero), counter_draw(1)
     
         
      
      
{
	std::cout << "I've been created" << std::endl;
	}

//Destructeur
Animal::~Animal()
{}

//Pour ameliorer le code il faut voir ou on peut utiliser la fonction setTargetPosition. En plus il faut voir si on doit vraiment mettre les methode draw annulus et draw sight en virtuel.
//En en plus ce qu'on peut mettre en private faut mettre en privat en particulièredes methode qui sont utilise seulment dans les autres methodes.






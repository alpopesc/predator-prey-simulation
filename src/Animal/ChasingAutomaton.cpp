#include "ChasingAutomaton.hpp"
#include <iostream>
#include <Animal/ChasingAutomaton.hpp>
#include <cmath>
#include <Utility/Constants.hpp>
#include <Application.hpp>


double ChasingAutomaton::getStandardMaxSpeed()
{
    return CHASING_AUTOMATON_MAX_SPEED;
}

double ChasingAutomaton::getMass()
{
    return CHASING_AUTOMATON_MASS;
}

void ChasingAutomaton::setTargetPosition(Vec2d new_targ)
{
    target = new_targ;
}

Vec2d ChasingAutomaton::getSpeedVector()
{
    return direction * vitesse;
}


void ChasingAutomaton::update(sf::Time dt)
{

    //1
    Vec2d f(force_attrac());

    //2 à 6
    maj_deplacement(f, dt);


}


//calcul de la force d'attraction :
Vec2d ChasingAutomaton::force_attrac()
{
    Vec2d to_target(0,0);
    to_target = target - position;
    //to_target = directionTo(target);			//I wanted to make the test in a toric world. Unfortunately it does not seem to work.

    if(to_target.length() == 0.0) {
        Vec2d force_attrac_r(0.0, 0.0);
        return force_attrac_r;
    }

    Deceleration deceleration(forte);

    /*
    if(distanceTo(target) < a){					//a, b and c can be some constants to be determined or so it seems to me if I understood the formulation.
    	deceleration = faible;
    }

    if(distanceTo(target) < b){
    	deceleration = moy;
    }

    if(distanceTo(target) < c){
    	deceleration = forte;
    }*/

    double v_compare = (/*distanceTo(to_target)*/ to_target.length() / (deceleration * 0.1 ));			//Belongs to the attempt to code the simulation in a toric world

    double v(0.0);

    if(v_compare > getStandardMaxSpeed()) {
        v = v_compare;
    } else {
        v = getStandardMaxSpeed();
    }

    Vec2d v_target;
    v_target = (to_target/ to_target.length()) * v;
    std::cout << "v_target" << v_target.length() << std::endl;

    Vec2d force_attrac_r(v_target - getSpeedVector());
    std::cout<<"Affiche norme de force attr" << force_attrac_r.length() << std::endl;
    return force_attrac_r;
}

//mise à jour des données de déplacement :
void ChasingAutomaton::maj_deplacement(Vec2d f, sf::Time dt)
{

    Vec2d v_courante;
    v_courante = vitesse * direction.normalised();

    //2
    Vec2d acc(0,0);
    acc = f/getMass();													// We could construct here a throw in case there is a division by 0.

    /*
    double acc_norme(f.length()/getMass());
    Vec2d acc;
    acc = acc_norme * direction;
    */


    //3
    v_courante += (acc * dt.asSeconds());

    //4
    direction = v_courante.normalised();

    //5
    if(v_courante.length() > getStandardMaxSpeed()) {

        v_courante = direction.normalised() * getStandardMaxSpeed();

    }
    vitesse = v_courante.length();
    //6
    move(v_courante * dt.asSeconds());										//Belongs to the attempt to code the simulation in a toric world
    //position += (v_courante * dt.asSeconds());

    std::cout << "vitesse " << vitesse << std::endl;
    std::cout << "direction" << direction << std::endl;
    std::cout << "target" << target << endl;
}

void ChasingAutomaton::draw(sf::RenderTarget& targetWindow)
{



    sf::Texture& texture = getAppTexture(GHOST_TEXTURE);
    auto image_to_draw(buildSprite(position, 2.0*rayon,texture));
    targetWindow.draw(image_to_draw);
    //std::cout << "ghost drawn" << std::endl;


    //for(auto target : ens_target){
    auto circle (buildCircle(target, 5.0, sf::Color::Red));
    targetWindow.draw(circle);
    //std::cout << "circle drawn" << std::endl;



}

ChasingAutomaton::ChasingAutomaton(Vec2d posit, double vit, Vec2d dir, Vec2d trg) 						//  vit and dir have to be parametrised otherwise the initialised target is gonna rest placed during the whole test
    : CircularCollider(posit, CHASING_AUTOMATON_RADIUS),vitesse(vit), direction(dir), target(trg)
{}



#pragma once

class Brain;

#include "neuron.hpp"
#include "sensor.hpp"
#include "motor.hpp"

#include <list>
#include <memory>

class Brain
{
private:
	
	std::list<std::shared_ptr<Neuron>> neurons;
	std::list<Sensor*> sensors;
	std::list<Motor*> motors;
	
public:

	Brain();
	void add(std::shared_ptr<Neuron> n);
	void reg_sensor(Sensor* sensor);
	void reg_motor(Motor* motor);
	void update();
	void render();
};


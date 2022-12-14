
#pragma once

class Brain;

#include "neuron.hpp"
#include "sensor.hpp"
#include "motor.hpp"

#include <list>
#include <memory>
#include <unordered_map>

class Brain
{
private:

	std::unordered_map<long, std::list<std::shared_ptr<Neuron>>> neurons;
	std::list<Sensor*> sensors;
	std::list<Motor*> motors;
	
public:

	struct FoundItem
	{
		std::shared_ptr<Neuron> neuron;
		double distance;
	};
	
	typedef std::list<FoundItem> Found;
	double box_radius;

	Brain(double box_radius);
	void find(Found& found, Vector pos, double radius);
	void add(std::shared_ptr<Neuron> n);
	void reg_sensor(Sensor* sensor);
	void reg_motor(Motor* motor);
	void update();
	void render();
};


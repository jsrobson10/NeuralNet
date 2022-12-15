
#pragma once

class Sensor;

#include "vector.hpp"
#include "entity.hpp"

class Sensor : public Entity
{
private:

	double value;
	
public:

	Sensor();
	Sensor(Vector pos);
	double get(Vector pos);
	void set(double value);
	void render();
};


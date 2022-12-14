
#pragma once

class Sensor;

#include "vector.hpp"

class Sensor
{
private:

	double value;
	
public:

	Vector pos;

	Sensor();
	Sensor(Vector pos);
	double get(Vector pos);
	void set(double value);
	void render();
};


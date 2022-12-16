
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
	void update_out();
	void set(double value);
	void render();
};


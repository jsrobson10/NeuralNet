
#pragma once

class Motor;

#include "vector.hpp"

class Motor
{
private:

	double value;
	Vector pos;
	
public:

	Motor(Vector pos);
	void reset();
	void add(Vector pos, double value);
	void render();
	double get();
};



#pragma once

class Motor;

#include "vector.hpp"
#include "entity.hpp"

class Motor : public Entity
{
private:

	double value;
	
public:
	
	Motor();
	Motor(Vector pos);
	void reset();
	void add(Vector pos, double value);
	void render();
	double get();
};


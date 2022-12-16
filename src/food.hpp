
#pragma once

class Food;

#include "entity.hpp"

class Food : public Entity
{
private:
	
	bool eaten;
	
public:

	Food();
	Food(double radius);

	void render();
	void update_out();
	bool alive();
	void eat();
};



#pragma once

class Food;

#include "entity.hpp"

class Food : public Entity
{
private:
	
	bool eaten;
	
public:

	Vector vel;

	Food();
	Food(double radius);

	void render();
	void update1();
	bool alive();
	void eat();
};


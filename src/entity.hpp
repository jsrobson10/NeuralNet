
#pragma once

class Entity;

#include "vector.hpp"

#include <memory>

class Entity
{
public:

	constexpr static const double M_DRAG = 15.0/16.0;
	
	Vector pos;
	std::weak_ptr<Entity> self;
	
	virtual void update1();
	virtual void update2();
	virtual void render();
	virtual bool alive();
};



#pragma once

class Entity;

#include "vector.hpp"

#include <memory>

class Entity
{
public:

	constexpr static const double M_DRAG = 15.0/16.0;
	
	Vector pos;
	Vector vel;
	std::weak_ptr<Entity> self;
	
	virtual void update_in();
	virtual void update_out();
	virtual void render();
	virtual bool alive();
};


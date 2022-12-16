
#pragma once

class Motor;

#include "vector.hpp"
#include "entity.hpp"
#include "taker.hpp"

#include <list>

class Motor : public Entity, public Taker
{
private:

	double value;
	double value_in;

	std::list<Giver::Item> s_giver;
	
public:
	
	Motor();
	Motor(Vector pos);
	std::shared_ptr<Taker> get_taker_self();
	void reg_giver(std::shared_ptr<Synapse> synapse, std::weak_ptr<Giver> giver);
	void give(double val);
	void update_in();
	void render();
	double get();
};



#pragma once

class Giver;

#include "synapse.hpp"
#include "entity.hpp"
#include "taker.hpp"

#include <memory>

class Giver
{
public:
	
	struct Item
	{
		Item(std::shared_ptr<Synapse> synapse, std::weak_ptr<Giver> giver) : synapse(synapse), giver(giver) {};
		
		std::shared_ptr<Synapse> synapse;
		std::weak_ptr<Giver> giver;
	};

	virtual std::shared_ptr<Giver> get_giver_self() = 0;
	virtual void reg_taker(std::shared_ptr<Synapse> synapse, std::weak_ptr<Taker> taker) {};
};


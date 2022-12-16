
#pragma once

class Taker;

#include "synapse.hpp"
#include "entity.hpp"
#include "giver.hpp"

#include <memory>

class Taker
{
public:
	
	struct Item
	{
		Item(std::shared_ptr<Synapse> synapse, std::weak_ptr<Taker> taker) : synapse(synapse), taker(taker) {};
		
		std::shared_ptr<Synapse> synapse;
		std::weak_ptr<Taker> taker;
	};

	virtual std::shared_ptr<Taker> get_taker_self() = 0;
	virtual void reg_giver(std::shared_ptr<Synapse> synapse, std::weak_ptr<Giver> giver) = 0;
	virtual void give(double value) = 0;
};


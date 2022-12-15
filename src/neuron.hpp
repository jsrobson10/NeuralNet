
#pragma once

class Neuron;

#include "vector.hpp"
#include "brain.hpp"
#include "synapse.hpp"
#include "entity.hpp"

#include <memory>
#include <list>

class Neuron : public Entity
{
private:

	struct SynapseItem
	{
		SynapseItem(std::shared_ptr<Synapse> synapse, std::weak_ptr<Neuron> neuron) : synapse(synapse), neuron(neuron) {};
		
		std::shared_ptr<Synapse> synapse;
		std::weak_ptr<Neuron> neuron;
	};

	Brain* host;
	unsigned char state;
	double d_length;
	double life;
	int time;
	int time_last;
	int diff_last;
	
	Vector vel;
	Vector vel_out;
	std::list<SynapseItem> s_in;
	std::list<SynapseItem> s_out;

	void update_entities();
	void update_synapse();
	void create_synapse();
	void on_active();
	void split();

public:
	
	double voltage;
	double food;

	Vector pos_out;
	
	Neuron(Brain* host);
	Neuron(Brain* host, Neuron* parent);

	void update1();
	void update2();
	void render();
	bool alive();
};


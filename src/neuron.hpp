
#pragma once

class Neuron;

#include "vector.hpp"
#include "brain.hpp"
#include "synapse.hpp"

#include <memory>
#include <list>

class Neuron
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
	int time;
	int time_last;
	int diff_last;
	
	Vector vel;
	Vector vel_out;
	std::list<SynapseItem> s_in;
	std::list<SynapseItem> s_out;

	void create_synapse();
	void on_active();
	void split();

public:
	
	int repeats;
	double voltage;

	Vector pos;
	Vector pos_out;
	std::weak_ptr<Neuron> self;
	
	Neuron(Brain* host);
	Neuron(Brain* host, Neuron* parent);
	void update();
	void absorb();
	void render();
	bool alive();
};


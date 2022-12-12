
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

	Brain* host;
	unsigned char state = 0;
	double duration_avg = 0;
	double duration_last = 0;
	double score = 0;

	std::list<std::shared_ptr<Synapse>> s_in;
	std::list<std::shared_ptr<Synapse>> s_out;
	
	Vector vel_in;
	Vector vel_out;

public:
	
	int repeats = 8;
	double life = 64;
	double voltage = -0.5;
	std::weak_ptr<Neuron> self;

	Vector pos_in;
	Vector pos_out;
	
	Neuron(Brain* host);
	Neuron(Brain* host, Neuron* parent);
	void update();
	void absorb();
	void render();
	bool alive();
};



#pragma once

class Neuron;

#include "vector.hpp"
#include "brain.hpp"
#include "synapse.hpp"
#include "entity.hpp"
#include "taker.hpp"
#include "giver.hpp"

#include <memory>
#include <list>

class Neuron : public Entity, public Taker, public Giver
{
private:

	Brain* host;
	unsigned char state;
	double d_length;
	double life;
	int time;
	int time_last;
	int diff_last;
	
	Vector vel_out;
	std::list<Giver::Item> s_giver;
	std::list<Taker::Item> s_taker;

	void update_entities();
	void update_synapse();
	void create_synapse();
	void create_synapse_next();
	void create_synapse_back();
	void on_active();
	void split();

public:
	
	double voltage;
	double food;

	Vector pos_out;
	
	Neuron(Brain* host);
	Neuron(Brain* host, Neuron* parent);

	std::shared_ptr<Taker> get_taker_self();
	std::shared_ptr<Giver> get_giver_self();
	void reg_taker(std::shared_ptr<Synapse> synapse, std::weak_ptr<Taker> taker);
	void reg_giver(std::shared_ptr<Synapse> synapse, std::weak_ptr<Giver> giver);
	void give(double val);
	void update_in();
	void update_out();
	void render();
	bool alive();
};


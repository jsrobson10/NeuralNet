
#include "brain.hpp"
#include "random.hpp"

#include <iostream>

Brain::Brain()
{
	neurons.push_back(std::shared_ptr<Neuron>(new Neuron(this)));
}

void Brain::add(std::shared_ptr<Neuron> n)
{
	n->self = n;
	neurons.push_back(n);
}

void Brain::update()
{
	for(auto& n : neurons)
	{
		n->absorb();

		for(auto s : sensors)
		{
			n->voltage += s->get(n->pos_in);
		}
	}

	for(auto m : motors)
	{
		m->reset();
	}

	std::shared_ptr<Neuron> max_n;
	double max_v = 0;

	for(auto it = neurons.begin(); it != neurons.end();)
	{
		auto& n = *it;

		n->update();

		if(n->voltage > 0)
		{
			for(auto m : motors)
			{
				m->add(n->pos_out, n->voltage);
			}
		}

		if(n->alive())
		{
			if(n->life > max_v && n->voltage > 0)
			{
				max_n = n;
				max_v = n->life;
			}

			it++;
		}
		
		else
		{
			it = neurons.erase(it);
		}
	}

	if(Random::num() < 0.01 && max_n)
	{
		max_n->repeats += 1;
	}
}

void Brain::reg_sensor(Sensor* sensor)
{
	sensors.push_back(sensor);
}

void Brain::reg_motor(Motor* motor)
{
	motors.push_back(motor);
}

void Brain::render()
{
	for(auto n : neurons)
	{
		n->render();
	}
	
	for(auto s : sensors)
	{
		s->render();
	}

	for(auto& m : motors)
	{
		m->render();
	}
}


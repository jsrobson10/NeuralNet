
#include <iostream>
#include <cmath>

#include "neuron.hpp"
#include "display.hpp"
#include "random.hpp"
#include "food.hpp"

constexpr double calc_mul(double m)
{
	return (m - 1) / m;
}

const double VOLTAGE_MUL = calc_mul(4);
const double DRAG_MUL = Entity::M_DRAG;
const double LIFE_MUL = calc_mul(4096);

const double SETPOINTS[] = {-0.5, 4, -0.5, 4, -2, -2};
const double THRESHOLDS[] = {-0.25, 2, -0.75};
const unsigned char STATES[] = {0, 1, 0, 1, 2, 2};

Neuron::Neuron(Brain* host)
{
	this->host = host;
	this->pos = Random::vector() * (Random::num() * 0.5 + 0.5) * 4;
	this->pos_out = Random::vector() * (Random::num() * 0.5 + 0.5) * 4;
	this->d_length = Random::num() * 56 + 8;
	this->voltage = -0.5;
	this->state = 0;
	this->time = 0;
	this->time_last = 0;
	this->diff_last = 0;
	this->life = 2;
}

Neuron::Neuron(Brain* host, Neuron* parent) : Neuron(host)
{
	this->pos += parent->pos;
	this->pos_out += parent->pos;
	this->vel += parent->vel;
	this->vel_out += parent->vel;
	this->life = parent->life / 2;
}

void Neuron::create_synapse_next()
{
	Brain::Found<Taker>::Type found;
	this->host->find<Taker>(found, this->pos_out, 16);
	int at = (int)(Random::num() * (found.size() - 1));
	auto it = found.begin();

	for(int i = 0; i < at; i++)
	{
		it++;
	}

	if(it != found.end() && it->entity.get() != this)
	{
		bool found = false;

		for(auto& item : s_taker)
		{
			if(item.taker.lock() == it->entity)
			{
				found = true;
				break;
			}
		}

		if(!found)
		{
			for(auto& item : s_taker)
			{
				if(item.taker.lock() == it->entity)
				{
					found = true;
					break;
				}
			}
		}

		if(!found)
		{
			std::shared_ptr<Synapse> synapse(new Synapse);
			it->entity->reg_giver(synapse, get_giver_self());
			reg_taker(synapse, it->entity);
		}
	}
}

void Neuron::create_synapse_back()
{
	Brain::Found<Giver>::Type found;
	this->host->find<Giver>(found, this->pos, 16);
	int at = (int)(Random::num() * (found.size() - 1));
	auto it = found.begin();

	for(int i = 0; i < at; i++)
	{
		it++;
	}

	if(it != found.end() && it->entity.get() != this)
	{
		bool found = false;

		for(auto& item : s_giver)
		{
			if(item.giver.lock() == it->entity)
			{
				found = true;
				break;
			}
		}

		if(!found)
		{
			for(auto& item : s_giver)
			{
				if(item.giver.lock() == it->entity)
				{
					found = true;
					break;
				}
			}
		}

		if(!found)
		{
			std::shared_ptr<Synapse> synapse(new Synapse);
			it->entity->reg_taker(synapse, get_taker_self());
			reg_giver(synapse, it->entity);
		}
	}
}

void Neuron::create_synapse()
{
	if(s_taker.size() < 4)
	{
		create_synapse_next();
	}

	//if(s_giver.size() < 2)
	{
		//create_synapse_back();
	}
}

void Neuron::on_active()
{
	int diff = std::abs(time_last - time);
	time_last = time;
	time = 0;

	if(diff > 0)
	{
		if(std::abs(diff - diff_last) < 2)
		{
			create_synapse();
			life += 0.25;
		}

		else
		{
			life -= 0.125;
		}
	}
	
	diff_last = diff;

	for(auto& s : s_taker)
	{
		s.synapse->update(this->voltage);
	}
}

void Neuron::split()
{
	std::shared_ptr<Neuron> n1(new Neuron(this->host, this));
	this->host->add(n1);
	this->life /= 2;
}

void Neuron::update_entities()
{
	int density = 0;
	Brain::Found<Entity>::Type found;
	this->host->find<Entity>(found, pos, 16);
	for(auto& item : found)
	{
		Neuron* item_neuron = dynamic_cast<Neuron*>(item.entity.get());

		if(item_neuron && item_neuron != this)
		{
			// move away from other nearby cell nuceli
			double d = item.distance * item.distance;
			density += 1;

			if(d && item.distance < 8)
			{
				vel += (pos - item_neuron->pos) / d * 0.25;
			}

			continue;
		}

		Food* item_food = dynamic_cast<Food*>(item.entity.get());

		if(item_food)
		{
			if(item.distance < 4)
			{
				// very close; eat
				item_food->eat();
				life += 1;
			}

			else
			{
				// move the food closer to me
				auto& n = item.entity;
				double d = item.distance * item.distance;

				if(d)
				{
					item_food->vel += (pos - item_food->pos) / d;
				}
			}

			continue;
		}
	}

	if(life > 4 && density < 2)
	{
		split();
	}
}

void Neuron::update_synapse()
{
	for(auto it = s_giver.begin(); it != s_giver.end();)
	{
		if(!it->giver.expired() && it->synapse->alive)
		{
			it++;
		}

		else
		{
			it = s_giver.erase(it);
		}
	}

	for(auto it = s_taker.begin(); it != s_taker.end();)
	{
		std::shared_ptr<Entity> n = std::dynamic_pointer_cast<Entity>(it->taker.lock());
		
		if(n && it->synapse->alive)
		{
			// move to radius of other connected synapses
			Vector diff = pos_out - n->pos;
			double len = diff.length() - it->synapse->length();
			Vector change = diff * len * 0.0001;
			n->vel += change / 2;
			vel_out -= change;
			it++;
		}

		else
		{
			it = s_taker.erase(it);
		}
	}
}

void Neuron::update_out()
{
	int off = state + (this->voltage > THRESHOLDS[state] ? 3 : 0);
	double set = SETPOINTS[off];
	
	this->voltage = (this->voltage + 0.5) * VOLTAGE_MUL - 0.5;
	this->state = STATES[off];
	this->pos += this->vel;
	this->pos_out += this->vel_out;
	this->vel *= DRAG_MUL;
	this->vel_out *= DRAG_MUL;
	this->life *= LIFE_MUL;

	update_entities();
	update_synapse();

	// manage dendrite length
	Vector diff = pos_out - pos;
	double len = diff.length() - d_length;
	Vector change = diff * len * 0.0001;
	vel_out -= change;
	vel += change / 2;

	// too much power
	if(this->voltage > 8)
	{
		//life = 0;
	}

	// active
	if(this->voltage > 0)
	{
		on_active();
	}

	else
	{
		time += 1;
	}
}

void Neuron::update_in()
{
	double v = 0;

	for(auto& s : s_giver)
	{
		v += s.synapse->swap();
	}

	if(v > 0)
	{
		this->voltage = std::max(this->voltage, v);
	}
}

void Neuron::give(double val)
{
	voltage += val;
}

void Neuron::render()
{
	if(s_taker.size() == 0 && s_giver.size() == 0)
	{
		//return;
	}
	
	double v = 0;
	double r = 0;

	if(this->voltage > 0)
	{
		v = 0.25;
		r = 1;
	}

	Display::Draw::colour(0.25+v, 0.5+v, 0.5+v);
	Display::Draw::line(this->pos, this->pos_out);
	Display::Draw::dot(this->pos_out, 2+r);

	Display::Draw::colour(0.5+v, 0.5+v, 0.25+v);
	Display::Draw::dot(this->pos, 3+r);
}

void Neuron::reg_taker(std::shared_ptr<Synapse> synapse, std::weak_ptr<Taker> taker)
{
	s_taker.push_back(Taker::Item(synapse, taker));
}

void Neuron::reg_giver(std::shared_ptr<Synapse> synapse, std::weak_ptr<Giver> giver)
{
	s_giver.push_back(Giver::Item(synapse, giver));
}

bool Neuron::alive()
{
	return life > 1;
}

std::shared_ptr<Giver> Neuron::get_giver_self()
{
	return dynamic_pointer_cast<Giver>(self.lock());
}

std::shared_ptr<Taker> Neuron::get_taker_self()
{
	return dynamic_pointer_cast<Taker>(self.lock());
}


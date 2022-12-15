
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

void Neuron::create_synapse()
{
	Brain::Found<Neuron>::Type found;
	this->host->find<Neuron>(found, this->pos_out, 16);
	int at = (int)(Random::num() * (found.size() - 1));
	auto it = found.begin();

	for(int i = 0; i < at; i++)
	{
		it++;
	}

	if(it != found.end() && it->entity.get() != this)
	{
		bool found = false;

		for(auto& item : s_in)
		{
			if(item.neuron.lock() == it->entity)
			{
				found = true;
				break;
			}
		}

		if(!found)
		{
			for(auto& item : s_out)
			{
				if(item.neuron.lock() == it->entity)
				{
					found = true;
					break;
				}
			}
		}

		if(!found)
		{
			std::shared_ptr<Synapse> synapse(new Synapse);
			s_out.push_back(SynapseItem(synapse, std::static_pointer_cast<Neuron, Entity>(it->entity->self.lock())));
			it->entity->s_in.push_back(SynapseItem(synapse, std::static_pointer_cast<Neuron, Entity>(self.lock())));
		}
	}
}

void Neuron::on_active()
{
	int diff = std::abs(time_last - time);
	time_last = time;
	time = 0;

	if(diff <= diff_last && s_out.size() < 4 && Random::num() < 1.0/16.0)
	{
		create_synapse();
	}
	
	diff_last = diff;

	for(auto& s : s_out)
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
	for(auto it = s_in.begin(); it != s_in.end();)
	{
		if(!it->neuron.expired() && it->synapse->alive)
		{
			it++;
		}

		else
		{
			it = s_in.erase(it);
		}
	}

	for(auto it = s_out.begin(); it != s_out.end();)
	{
		std::shared_ptr<Neuron> n = it->neuron.lock();
		
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
			it = s_out.erase(it);
		}
	}
}

void Neuron::update2()
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

	/*if(this->repeats > 0 && Random::num() < this->repeats / 1024.0)
	{
		split();
	}*/

	update_entities();
	update_synapse();

	// move away from walls
//	double box_rad = host->box_radius;
//	if(pos.x > box_rad && vel.x > 0) {vel.x *= -1; pos.x = box_rad;}
//	if(pos.y > box_rad && vel.y > 0) {vel.y *= -1; pos.y = box_rad;}
//	if(pos.x < -box_rad && vel.x < 0) {vel.x *= -1; pos.x = -box_rad;}
//	if(pos.y < -box_rad && vel.y < 0) {vel.y *= -1; pos.y = -box_rad;}
//	if(pos_out.x > box_rad && vel_out.x > 0) {vel_out.x *= -1; pos_out.x = box_rad;}
//	if(pos_out.y > box_rad && vel_out.y > 0) {vel_out.y *= -1; pos_out.y = box_rad;}
//	if(pos_out.x < -box_rad && vel_out.x < 0) {vel_out.x *= -1; pos_out.x = -box_rad;}
//	if(pos_out.y < -box_rad && vel_out.y < 0) {vel_out.y *= -1; pos_out.y = -box_rad;}

	// manage dendrite length
	Vector diff = pos_out - pos;
	double len = diff.length() - d_length;
	Vector change = diff * len * 0.0001;
	vel_out -= change;
	vel += change / 2;

	// active
	if(this->voltage > 0)
	{
		on_active();
	}
}

void Neuron::update1()
{
	double v = 0;

	for(auto& s : s_in)
	{
		v += s.synapse->swap();
	}

	if(v > 0)
	{
		this->voltage = std::max(this->voltage, v);
	}
}

void Neuron::render()
{
	if(s_out.size() == 0 && s_in.size() == 0)
	{
//		return;
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

	for(auto& s : s_out)
	{
		std::shared_ptr<Neuron> n = s.neuron.lock();

		if(n)
		{
			Display::Draw::line(this->pos_out, n->pos);
		}
	}

	Display::Draw::dot(this->pos, 3+r);
}

bool Neuron::alive()
{
	return life > 1;
}


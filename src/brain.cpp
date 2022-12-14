
#include "brain.hpp"
#include "random.hpp"
#include "display.hpp"

#include <iostream>
#include <cmath>

static long hash_int_m16(int x, int y)
{
	int pos[] = {x / 16, y / 16};
	return *(long*)&pos;
}

static long hash(Vector p)
{
	int pos[] = {(int)std::floor(p.x / 16), (int)std::floor(p.y / 16)};
	return *(long*)&pos;
}

Brain::Brain(double box_radius) : box_radius(box_radius)
{
	add(std::shared_ptr<Neuron>(new Neuron(this)));
}

void Brain::add(std::shared_ptr<Neuron> n)
{
	n->self = n;
	long key = hash(n->pos);
	neurons[key].push_back(n);
}

void Brain::update()
{
	for(auto& box : neurons)
	{
		for(auto& n : box.second)
		{
			n->absorb();

			for(auto s : sensors)
			{
				n->voltage += s->get(n->pos);
			}
		}
	}

	for(auto m : motors)
	{
		m->reset();
	}

	for(auto& box : neurons)
	{
		for(auto it = box.second.begin(); it != box.second.end();)
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
				it++;
			}
			
			else
			{
				it = box.second.erase(it);
			}
		}
	}

	// update all neurons to make sure they're in the right place

	std::list<std::shared_ptr<Neuron>> to_add;

	for(auto a = neurons.begin(); a != neurons.end(); a++)
	{
		long key = a->first;
		
		for(auto b = a->second.begin(); b != a->second.end();)
		{
			if(hash((*b)->pos) != key)
			{
				to_add.push_back(*b);
				b = a->second.erase(b);
			}

			else
			{
				b++;
			}
		}
	}

	for(auto& n : to_add)
	{
		add(n);
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
	for(auto& box : neurons)
	{
		for(auto& n : box.second)
		{
			n->render();
		}
	}
	
	for(auto s : sensors)
	{
		s->render();
	}

	for(auto& m : motors)
	{
		m->render();
	}

	Display::Draw::colour(1, 1, 1);
	Display::Draw::line(Vector(-box_radius, -box_radius), Vector(-box_radius, box_radius));
	Display::Draw::line(Vector(-box_radius, -box_radius), Vector(box_radius, -box_radius));
	Display::Draw::line(Vector(box_radius, box_radius), Vector(-box_radius, box_radius));
	Display::Draw::line(Vector(box_radius, box_radius), Vector(box_radius, -box_radius));
}

void Brain::find(Found& found, Vector pos, double radius)
{
	int x_s = std::floor((pos.x - radius) / 16) * 16;
	int y_s = std::floor((pos.y - radius) / 16) * 16;

	for(int x = x_s; x <= pos.x + radius; x += 16)
	{
		for(int y = y_s; y <= pos.y + radius; y += 16)
		{
			long h = hash_int_m16(x, y);
			auto& l = neurons[h];

			for(auto& n : l)
			{
				double d = (pos - n->pos).length();

				if(d < radius)
				{
					FoundItem item;
					item.neuron = n;
					item.distance = d;
					found.push_back(item);
				}
			}
		}
	}
}


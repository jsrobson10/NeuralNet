
#include "brain.hpp"
#include "random.hpp"
#include "display.hpp"
#include "entity.hpp"
#include "neuron.hpp"
#include "food.hpp"

#include <iostream>
#include <cmath>

Brain* Brain::Current = nullptr;

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
	add(std::make_shared<Neuron>(this));
}

Brain::Brain() : Brain(0)
{

}

void Brain::add(std::shared_ptr<Entity> e)
{
	e->self = e;
	long key = hash(e->pos);
	entities[key].push_back(e);
}

void Brain::update()
{
	//std::cout << "entity\t" << entity->pos << "\t" << entity->voltage << std::endl;
	
	//add some food
	add(std::make_shared<Food>());

	for(auto& box : entities)
	{
		for(auto& n : box.second)
		{
			n->update1();
		}
	}

	for(auto& box : entities)
	{
		for(auto it = box.second.begin(); it != box.second.end();)
		{
			auto& n = *it;

			n->update2();

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

	// update all entities to make sure they're in the right place

	std::list<std::shared_ptr<Entity>> to_add;

	for(auto a = entities.begin(); a != entities.end(); a++)
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

void Brain::render()
{
	for(auto& box : entities)
	{
		for(auto& n : box.second)
		{
			n->render();
		}
	}

//	Display::Draw::colour(1, 1, 1);
//	Display::Draw::line(Vector(-box_radius, -box_radius), Vector(-box_radius, box_radius));
//	Display::Draw::line(Vector(-box_radius, -box_radius), Vector(box_radius, -box_radius));
//	Display::Draw::line(Vector(box_radius, box_radius), Vector(-box_radius, box_radius));
//	Display::Draw::line(Vector(box_radius, box_radius), Vector(box_radius, -box_radius));
}



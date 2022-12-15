
#pragma once

class Brain;

#include "entity.hpp"
#include "sensor.hpp"
#include "motor.hpp"

#include <list>
#include <memory>
#include <unordered_map>
#include <cmath>

class Brain
{
private:

	std::unordered_map<long, std::list<std::shared_ptr<Entity>>> entities;
	
public:

	static Brain* Current;
	
	template <class T>
	struct Found
	{
		struct Item
		{
			std::shared_ptr<T> entity;
			double distance;
		};

		typedef std::list<Item> Type;
	};

	double box_radius;

	Brain();
	Brain(double box_radius);
	void add(std::shared_ptr<Entity> e);
	void update();
	void render();

	template <class T>
	void find(typename Found<T>::Type& found, Vector pos, double radius)
	{
		int x_s = std::floor((pos.x - radius) / 16) * 16;
		int y_s = std::floor((pos.y - radius) / 16) * 16;
	
		for(int x = x_s; x <= pos.x + radius; x += 16)
		{
			for(int y = y_s; y <= pos.y + radius; y += 16)
			{
				int at[] = {x / 16, y / 16};
				long h = *(long*)at;
				auto& l = entities[h];
	
				for(auto& e : l)
				{
					typename Found<T>::Item item;
					item.entity = std::dynamic_pointer_cast<T, Entity>(e);

					if(!item.entity)
					{
						continue;
					}

					item.distance = (pos - e->pos).length();

					if(item.distance < radius)
					{
						found.push_back(item);
					}
				}
			}
		}
	}
};



#include "sensor.hpp"
#include "display.hpp"

Sensor::Sensor()
{
	
}

Sensor::Sensor(Vector pos)
{
	this->pos = pos;
}

void Sensor::update_out()
{
	Brain::Found<Taker>::Type found;
	Brain::Current->find<Taker>(found, pos, 32);

	for(Brain::Found<Taker>::Item& item : found)
	{
		item.entity->give(value);
	}
}

void Sensor::set(double value)
{
	this->value = value;
}

void Sensor::render()
{
	double b = 1 / (-2 - this->value) + 1;
	Display::Draw::colour(0.25+b, 0.25+b, 0.5+b);
	Display::Draw::dot(this->pos, 4+value);
}


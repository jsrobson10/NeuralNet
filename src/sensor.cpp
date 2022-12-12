
#include "sensor.hpp"
#include "display.hpp"

Sensor::Sensor(Vector pos)
{
	this->pos = pos;
}

double Sensor::get(Vector pos)
{
	return this->value / ((this->pos - pos).length() + 1);
}

void Sensor::set(double value)
{
	this->value = value;
}

void Sensor::render()
{
	double b = 1 / (-2 - this->value) + 1;
	Display::Draw::colour(0.25+b, 0.25+b, 0.5+b);
	Display::Draw::dot(this->pos, 8);
}



#include "motor.hpp"
#include "display.hpp"

Motor::Motor(Vector pos)
{
	this->pos = pos;
}

void Motor::reset()
{
	this->value = 0;
}

void Motor::add(Vector pos, double value)
{
	value = value / ((this->pos - pos).length() - 1);

	if(value > this->value)
	{
		this->value = value;
	}
}

double Motor::get()
{
	return this->value;
}

void Motor::render()
{
	double b = 1 / (-2 - this->value) + 1;
	Display::Draw::colour(0.5+b, 0.25+b, 0.25+b);
	Display::Draw::dot(this->pos, 8);
}


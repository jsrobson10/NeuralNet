
#include "motor.hpp"
#include "display.hpp"

Motor::Motor()
{
	value = 0;
	value_in = 0;
}

Motor::Motor(Vector pos)
{
	this->pos = pos;
}

double Motor::get()
{
	return this->value;
}

void Motor::reg_giver(std::shared_ptr<Synapse> synapse, std::weak_ptr<Giver> giver)
{
	s_giver.push_back(Giver::Item(synapse, giver));
}

void Motor::update_in()
{
	value = value_in;
	value_in = 0;

	for(auto it = s_giver.begin(); it != s_giver.end();)
	{
		if(!it->giver.expired())
		{
			value += it->synapse->swap();
			it++;
		}

		else
		{
			it = s_giver.erase(it);
		}
	}
}

void Motor::give(double value)
{
	value_in += value;
}

void Motor::render()
{
	double b = 1 / (-2 - this->value) + 1;
	Display::Draw::colour(0.5+b, 0.25+b, 0.25+b);
	Display::Draw::dot(this->pos, 4+value);
}

std::shared_ptr<Taker> Motor::get_taker_self()
{
	return dynamic_pointer_cast<Taker>(self.lock());
}


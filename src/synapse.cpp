
#include "synapse.hpp"

#include <iostream>
#include <cmath>

Synapse::Synapse() noexcept
{
	mul = 1.0/64.0;
	voltage = 0;
	time = 0;
	time_last = 0;
	diff_last = 0;
	alive = true;
}

double Synapse::swap()
{
	double v = voltage;
	voltage = 0;
	time += 1;

	//kill when very overloaded
	if(v > 4)
	{
		alive = false;
	}

	//reduce when overloaded
	else if(v > 1)
	{
		modify(7.0/8.0);
	}

	else if(v > 0)
	{
		int diff = std::abs(time_last - time);
		time_last = time;
		time = 0;

		if(diff <= diff_last)
		{
			modify(9.0/8.0);
		}

		else
		{
			modify(7.0/8.0);
		}
		
		diff_last = diff;
	}

	return v;
}

void Synapse::update(double v) noexcept
{
	voltage = std::fmax(0, mul * v);
}

void Synapse::modify(double m) noexcept
{
	mul *= m;
}

double Synapse::length() noexcept
{
	return 4;
}


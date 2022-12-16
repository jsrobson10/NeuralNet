
#include "synapse.hpp"

#include <cmath>
#include <iostream>

Synapse::Synapse() noexcept
{
	mul = 1.0/16.0;
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
		modify(3.0/4.0);
	}

	else if(v > 0)
	{
		int diff = std::abs(time_last - time);
		time_last = time;
		time = 0;
		

		if(diff > 0)
		{
			if(std::abs(diff - diff_last) < 2)
			{
				modify(5.0/4.0);
			}

			else
			{
				modify(3.0/4.0);
			}
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



#pragma once

class Synapse;

#include "neuron.hpp"

#include <memory>

class Synapse
{
private:

	double m;
	double v;

public:

	std::weak_ptr<Neuron> in;
	std::weak_ptr<Neuron> out;

	Synapse(std::weak_ptr<Neuron> in, std::weak_ptr<Neuron> out);
	void add(double v);
	void modify(double m);
	double get();
};


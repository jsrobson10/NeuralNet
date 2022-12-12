
#include "synapse.hpp"

Synapse::Synapse(std::weak_ptr<Neuron> in, std::weak_ptr<Neuron> out)
{
	this->in = in;
	this->out = out;
	this->m = 1;
	this->v = 0;
}

double Synapse::get()
{
	double v = this->v;
	this->v = 0;
	return v;
}

void Synapse::add(double v)
{
	this->v += this->m * v;
}

void Synapse::modify(double m)
{
	this->m *= m;
}


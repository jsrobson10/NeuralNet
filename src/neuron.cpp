
#include <iostream>
#include <cmath>

#include "neuron.hpp"
#include "display.hpp"
#include "random.hpp"

constexpr double calc_mul(double m)
{
	return (m - 1) / m;
}

const double VOLTAGE_MUL = calc_mul(4);
const double DRAG_MUL = calc_mul(64);
const double LIFE_MUL = calc_mul(4);

const double SETPOINTS[] = {-0.5, 4, -0.5, 4, -2, -2};
const double THRESHOLDS[] = {-0.25, 2, -0.75};
const unsigned char STATES[] = {0, 1, 0, 1, 2, 2};

Neuron::Neuron(Brain* host)
{
	this->host = host;
	this->vel_in = Random::vector() * (Random::num() * 0.5 + 0.5);
	this->vel_out = Random::vector() * (Random::num() * 0.5 + 0.5);
}

Neuron::Neuron(Brain* host, Neuron* parent) : Neuron(host)
{
	this->pos_in += parent->pos_in;
	this->pos_out += parent->pos_in;
	this->vel_in += parent->vel_in;
	this->vel_out += parent->vel_in;
	this->repeats = parent->repeats;
	this->duration_last = parent->duration_last;
	this->duration_avg = parent->duration_avg;
	this->score = parent->score;
	this->life = parent->life;
}

void Neuron::update()
{
	int state = this->state;
	int off = state + (this->voltage > THRESHOLDS[state] ? 3 : 0);
	double set = SETPOINTS[off];

	this->state = STATES[off];
	this->voltage = (this->voltage - set) * VOLTAGE_MUL + set;
	this->pos_in += this->vel_in;
	this->pos_out += this->vel_out;
	this->vel_in *= DRAG_MUL;
	this->vel_out *= DRAG_MUL;
	this->duration_last += 1;

	if(this->repeats > 0 && Random::num() < this->repeats / 1024.0)
	{
		this->repeats -= 1;
		this->life *= LIFE_MUL;

		std::shared_ptr<Neuron> n1(new Neuron(this->host, this));
		std::shared_ptr<Neuron> n2(new Neuron(this->host, this));
		std::shared_ptr<Synapse> s1(new Synapse(n1, this->self));
		std::shared_ptr<Synapse> s2(new Synapse(this->self, n2));
		n1->s_out.push_back(s1);
		n2->s_in.push_back(s2);
		s_out.push_back(s2);
		s_in.push_back(s1);
		this->host->add(n1);
		this->host->add(n2);
	}

	if(this->voltage > 0)
	{
		if(this->duration_last > 2)
		{
			double duration_avg = this->duration_avg / 2 + std::sqrt(this->duration_last);
			double score = std::abs(this->duration_avg - duration_avg);
			this->duration_avg = duration_avg;

			if(score <= this->score)
			{
				this->life += 1;
			}

			this->life *= LIFE_MUL;
			this->score = score;
		}
			
		this->duration_last = 0;

		for(auto& s : this->s_out)
		{
			s->add(this->voltage);
		}
	}
}

void Neuron::absorb()
{
	double v = 0;

	for(auto& s : this->s_in)
	{
		v += s->get();
	}

	this->voltage += std::sqrt(v);
}

bool Neuron::alive()
{
	return (this->life >= 1);
}

void Neuron::render()
{
	double v = 0;
	double r = 0;

	if(this->voltage > 0)
	{
		v = 0.25;
		r = 1;
	}

	Display::Draw::colour(0.25+v, 0.5+v, 0.5+v);
	Display::Draw::line(this->pos_in, this->pos_out);
	Display::Draw::dot(this->pos_out, 2+r);

	Display::Draw::colour(0.5+v, 0.5+v, 0.25+v);

	for(auto& s : this->s_out)
	{
		std::shared_ptr<Neuron> n = s->out.lock();

		if(n)
		{
			Display::Draw::line(this->pos_out, n->pos_in);
		}
	}

	Display::Draw::dot(this->pos_in, 3+r);
}


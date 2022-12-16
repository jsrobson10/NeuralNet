
#include "food.hpp"
#include "random.hpp"
#include "display.hpp"
#include <cmath>

Food::Food(double radius)
{
	pos = Random::vector() * std::sqrt(Random::num()) * radius;
	eaten = false;
}

Food::Food() : Food(256) {}

void Food::render()
{
	Display::Draw::colour(0.25, 0.5, 0.25);
	Display::Draw::dot(pos, 1);
}

bool Food::alive()
{
	return !eaten;
}

void Food::eat()
{
	eaten = true;
}

void Food::update_out()
{
	pos += vel;
	vel *= Entity::M_DRAG;
}


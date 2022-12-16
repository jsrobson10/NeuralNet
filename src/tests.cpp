
#include "tests.hpp"
#include "brain.hpp"
#include "random.hpp"

Tests::Tests()
{
	sensor = std::make_shared<Sensor>();
	motor = std::make_shared<Motor>();
	at = 0;

	sensor->pos = Vector(-128, 0);
	motor->pos = Vector(128, 0);

	Brain::Current->add(sensor);
	Brain::Current->add(motor);
}

void Tests::update()
{
	at += 1;

	sensor->set(at % 16 == 15 ? 1 : 0);
//	sensor->set(Random::num() < 1.0/16.0 ? 1 : 0);
}


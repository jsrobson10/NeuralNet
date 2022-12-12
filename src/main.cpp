
#include "brain.hpp"
#include "display.hpp"
#include "time.hpp"
#include "sensor.hpp"
#include "random.hpp"

#include <iostream>

int main(int argc, char** argv)
{
	Brain brain;
	Display::init();

	int i = 0;
	Sensor flashyg1(Vector(-50, -50));
	Sensor flashyg2(Vector(50, 50));
	Sensor flashyb1(Vector(50, -50));
	Sensor flashyb2(Vector(-50, 50));
	brain.reg_sensor(&flashyg1);
	brain.reg_sensor(&flashyg2);
	brain.reg_sensor(&flashyb1);
	brain.reg_sensor(&flashyb2);

	while(!Display::shouldClose())
	{
		flashyg1.set(i++ % 25 == 10 ? 16 : 0);
		flashyg2.set(i % 25 == 20 ? 16 : 0);
		flashyb1.set(Random::num() < 1.0/64 ? 16 : 0);
		flashyb2.set(Random::num() < 1.0/64 ? 16 : 0);
		brain.update();
		Display::update(&brain);
		Time::sleep(10000);
	}

	Display::cleanup();
}


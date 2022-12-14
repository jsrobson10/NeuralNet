
#include "brain.hpp"
#include "display.hpp"
#include "time.hpp"
#include "sensor.hpp"
#include "random.hpp"
#include "pong.hpp"

#include <iostream>

int main(int argc, char** argv)
{
	Brain brain(256);
	Time::Tracker timer;
	Display::init();
	Pong game(brain);

	while(!Display::shouldClose())
	{
		game.update();
		brain.update();
		Display::update(&brain);
//		timer.sleep_next(10000);
	}

	Display::cleanup();
}


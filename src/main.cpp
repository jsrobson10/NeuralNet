
#include "brain.hpp"
#include "display.hpp"
#include "time.hpp"
#include "sensor.hpp"
#include "random.hpp"
#include "pong.hpp"
#include "tests.hpp"

#include <iostream>

int main(int argc, char** argv)
{
	Brain brain;
	Time::Tracker timer;
	Display::init();
	Pong game;
//	Tests game;

	while(!Display::shouldClose())
	{
		game.update();
		brain.update();
		Display::update(&brain);
//		timer.sleep_next(10000);
	}

	Display::cleanup();
}


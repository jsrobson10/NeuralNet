
#pragma once

class Tests;

#include "sensor.hpp"
#include "motor.hpp"

#include <memory>

class Tests
{
public:

	std::shared_ptr<Sensor> sensor;
	std::shared_ptr<Motor> motor;
	long at;

	Tests();
	void update();
};



#pragma once

#include "brain.hpp"
#include "sensor.hpp"
#include "motor.hpp"

#include <array>

class Pong
{
private:
	
	Brain* brain;
	Sensor s_paddle_l;
	Sensor s_paddle_r;
	Sensor s_ball;
	std::array<Sensor, 8> s_rewards;
	std::array<Motor, 2> m_paddle_l;
	std::array<Motor, 2> m_paddle_r;
	double paddle_l_p;
	double paddle_r_p;
	double paddle_l_v;
	double paddle_r_v;
	Vector ball_p;
	Vector ball_v;
	int state;
	int left;
	long at;

	double shock_nve();
	double shock_pve();
	void update_game();
	void update_nve();
	void update_pve();

public:
	
	Pong(Brain& brain);
	void update();

};


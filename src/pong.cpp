
#include "pong.hpp"
#include "random.hpp"

#include <iostream>

Pong::Pong(Brain& brain)
{
	this->paddle_l_p = 0;
	this->paddle_r_p = 0;
	this->paddle_l_v = 0;
	this->paddle_r_v = 0;
	this->ball_p = Vector(0, 0);
	this->ball_v = Vector(1, 0.5);
	this->brain = &brain;
	this->state = 0;
	this->left = 0;
	this->at = 0;

	//rewards pos
	for(int i = 0; i < s_rewards.size(); i++)
	{
		int x = i / 2;
		int y = i % 2;

		s_rewards[i] = std::make_shared<Sensor>();
		s_rewards[i]->pos = Vector(x * 64 - 96, y * 64 + 96);
		brain.add(s_rewards[i]);
	}

	s_ball = std::make_shared<Sensor>();
	s_paddle_l = std::make_shared<Sensor>();
	s_paddle_r = std::make_shared<Sensor>();
	m_paddle_l[0] = std::make_shared<Motor>();
	m_paddle_l[1] = std::make_shared<Motor>();
	m_paddle_r[0] = std::make_shared<Motor>();
	m_paddle_r[1] = std::make_shared<Motor>();
	m_paddle_l[0]->pos = Vector(-32 - 64, -192);
	m_paddle_l[1]->pos = Vector(-32 - 0, -192);
	m_paddle_r[1]->pos = Vector(32 + 0, -192);
	m_paddle_r[0]->pos = Vector(32 + 64, -192);

	brain.add(s_ball);
	brain.add(s_paddle_l);
	brain.add(s_paddle_r);
	brain.add(m_paddle_l[0]);
	brain.add(m_paddle_l[1]);
	brain.add(m_paddle_r[0]);
	brain.add(m_paddle_r[1]);
}

double Pong::shock_nve()
{
	return (Random::num() < 1.0/32.0) ? 4 : 0;
}

double Pong::shock_pve()
{
	double v = ((at % 16) == 0) ? 4 : 0;

	return v;
}

void Pong::update_game()
{
	ball_p += ball_v;

	if(ball_p.y > 48 || ball_p.y < -48)
	{
		ball_v.y *= -1;
	}

	if(ball_p.x > 176 || ball_p.x < -176)
	{
		if(ball_p.x > 176 && std::abs(ball_p.y - paddle_r_p) < 32)
		{
			left = 128;
			ball_v.x = -1;
			state = 2;
		}

		else if(ball_p.x < -176 && std::abs(ball_p.y - paddle_l_p) < 32)
		{
			left = 128;
			ball_v.x = 1;
			state = 2;
		}

		else
		{
			ball_p = Vector(0, 0);
			ball_v = Vector(Random::num() < 0.5 ? -1 : 1, Random::num() < 0.5 ? -0.5 : 0.5);
			paddle_l_v = 0;
			paddle_r_v = 0;
			paddle_l_p = 0;
			paddle_r_p = 0;
			left = 256;
			state = 1;
			return;
		}
	}

	if(m_paddle_r[0]->get() > m_paddle_r[1]->get())
	{
		paddle_r_v -= 1;
	}

	else
	{
		paddle_r_v += 1;
	}

	if(m_paddle_l[0]->get() > m_paddle_l[1]->get())
	{
		paddle_l_v -= 1;
	}

	else
	{
		paddle_l_v += 1;
	}

	if(paddle_r_p < -48 && paddle_r_v < 0)
	{
		paddle_r_v = 0;
	}

	if(paddle_l_p < -48 && paddle_l_v < 0)
	{
		paddle_l_v = 0;
	}

	if(paddle_r_p > 48 && paddle_r_v > 0)
	{
		paddle_r_v = 0;
	}

	if(paddle_l_p > 48 && paddle_l_v > 0)
	{
		paddle_l_v = 0;
	}

	paddle_l_p += paddle_l_v;
	paddle_r_p += paddle_r_v;
	paddle_l_v *= 15.0/16.0;
	paddle_r_v *= 15.0/16.0;
	s_paddle_l->pos = Vector(-192, paddle_l_p - 48);
	s_paddle_r->pos = Vector(192, paddle_r_p - 48);
	s_ball->pos = ball_p - Vector(0, 48);
	s_paddle_l->set(shock_pve());
	s_paddle_r->set(shock_pve());
	s_ball->set(shock_pve());
}

void Pong::update_nve()
{
	s_ball->set(0);
	s_paddle_l->set(shock_nve());
	s_paddle_r->set(shock_nve());

	for(auto& s : s_rewards)
	{
		s->set(shock_nve());
	}

	if(--left == 0)
	{
		state = 0;
	}
}

void Pong::update_pve()
{
	for(auto& s : s_rewards)
	{
		s->set(shock_pve());
	}

	if(--left == 0)
	{
		state = 0;
	}
}

void Pong::update()
{
	for(auto& s : s_rewards)
	{
		s->set(0);
	}

	switch(state)
	{
		case 0:
			update_game();
			break;
		case 1:
			update_nve();
			break;
		case 2:
			update_game();
			update_pve();
			break;
	}

	at += 1;
}


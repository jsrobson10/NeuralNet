
#pragma once

#include "brain.hpp"
#include "vector.hpp"

namespace Display
{
	void init();
	void update(Brain* b);
	bool shouldClose();
	void cleanup();

	namespace Draw
	{
		void colour(double r, double g, double b);
		void colour(double r, double g, double b, double a);
		void quad(Vector p1, Vector p2);
		void line(Vector p1, Vector p2);
		void dot(Vector p, double r);
	};
};


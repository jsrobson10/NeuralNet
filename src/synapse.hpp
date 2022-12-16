
#pragma once

class Synapse
{
private:

	double mul;
	double voltage;
	int time;
	int time_last;
	int diff_last;

public:

	Synapse() noexcept;
	bool alive;
	void update(double v) noexcept;
	void modify(double m) noexcept;
	double length() noexcept;
	double swap();
};



#pragma once

namespace Time
{
	class Tracker
	{
	private:

		unsigned long at;

	public:

		Tracker() noexcept;
		void sleep_next(unsigned long us);
	};
	
	unsigned long get_us() noexcept;
	void sleep(unsigned long us);
};


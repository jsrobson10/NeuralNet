
#include "time.hpp"

#include <sys/time.h>
#include <unistd.h>

unsigned long Time::get_us() noexcept
{
	timeval tv;
	gettimeofday(&tv, nullptr);
	return (unsigned long)tv.tv_usec + (unsigned long)tv.tv_sec * 1000000;
}

void Time::sleep(unsigned long us)
{
	usleep(us);
}

Time::Tracker::Tracker() noexcept
{
	at = get_us();
}

void Time::Tracker::sleep_next(unsigned long us)
{
	at += us;
	long diff = at - get_us();
	if(diff > 0) sleep(diff);
}


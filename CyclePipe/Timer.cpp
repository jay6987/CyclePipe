// Description:
//   Timer provides accurate time measurement

#include "Timer.h"
#include <thread>

using namespace std::chrono;

namespace CyclePipe
{

	Timer::Timer()
		: lastTic(high_resolution_clock::now())
	{
	}

	void Timer::Tic()
	{
		lastTic = high_resolution_clock::now();
	}

	double Timer::Toc() const
	{
		return duration_cast<duration<double>>(
			high_resolution_clock::now() - lastTic).count();
	}

	void Timer::Sleep(double seconds)
	{
		std::this_thread::sleep_for(duration<double>(seconds));
	}
}
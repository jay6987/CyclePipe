// Description:
//   Timer provides accurate time measurement and pause

#pragma once
#include <chrono>

namespace CyclePipe
{
	// The smallest value of high_resolution clock is nanosecond ( 1e-12 s)
	// a double float has 15-16 digit (decimal) effective digits
	// if the timer does not run several days (1 day = 8.64 e4 seconds)
	// it will not have any problem to use double to accumulate time span.

	class Timer
	{
	public:

		Timer();


		void Tic();

		// return time span from last time Tic() is call (or Timer is constructed) to now
		// measured in second
		double Toc() const;
		
		static void Sleep(double seconds);
		
	private:

		std::chrono::time_point<std::chrono::high_resolution_clock> lastTic;
	};
}


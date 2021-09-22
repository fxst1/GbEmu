#include "timing/Timing.hh"
#include <iostream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>

timing::HertzClock::HertzClock(float frequency):
	timing::Clock::Clock(),
	_frequency(frequency) {}
timing::HertzClock::~HertzClock(void){}


bool	timing::HertzClock::wait(void) {

	std::cout << "WAIT !!!!" << std::endl;
	double	freq_val = 1 / this->_frequency;
	auto now = std::chrono::steady_clock::now();
	auto old = this->_current;
	auto diff = std::chrono::duration_cast<std::chrono::microseconds>(now - old).count();

/*
	auto t_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now() - std::chrono::hours(24));
	    std::cout << "24 hours ago, the time was "
	              << std::put_time(std::localtime(&t_c), "%F %T") << std::endl;
*/
	if (diff * freq_val < 1) {
		this->_current = now;

/*
		std::cout
		  << "Frame wait " << diff * freq_val << std::dec
		  << std::chrono::duration_cast<std::chrono::microseconds>(now - old).count() << "µs ≈ "
		  << std::chrono::duration_cast<std::chrono::milliseconds>(now - old).count() << "ms ≈ "
		  << std::chrono::duration_cast<std::chrono::seconds>(now - old).count() << "s." << std::endl;
		  */
		//std::this_thread::sleep_for(std::chrono::milliseconds(diff));
	} else {
		//std::cout << " NO Frame wait " << diff * freq_val << std::endl;
	}
	return (true);
}

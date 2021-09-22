#include "timing/Timing.hh"

timing::Clock::Clock(void):
	_ticks() {}
timing::Clock::~Clock(void){}

/*********************************/

void	timing::Clock::add(timing::Tick &t) {
	this->_ticks.insert(&t);
}

void	timing::Clock::remove(timing::Tick &t) {
	this->_ticks.erase(&t);
}

/*********************************/

void	timing::Clock::sendTick(void) {
	if (this->wait()) {
		for (auto it = this->_ticks.begin(); it != this->_ticks.end(); ++it) {
			int n = (*it)->nTickPerFrame();
			for (int i = 1; i <= n; i++) {
				(*it)->tick();
			}
		}
	}
}

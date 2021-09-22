#ifndef TIMING_HH
# define TIMING_HH
# include <chrono>
# include <thread>
# include <unordered_set>

namespace	timing {

	class		Tick {
		public:
			virtual ~Tick(void) {}
			virtual void	tick(void) = 0;
			virtual int		nTickPerFrame(void) const {
				return (1);
			}
	};

	class		Clock {
		public:
			Clock(void);
			~Clock(void);

			void	add(timing::Tick& t);
			void	remove(timing::Tick& t);

			void	sendTick(void);

		protected:
			virtual bool		wait(void) = 0;

		private:
			std::unordered_set<Tick*>		_ticks;
	};

	class		HertzClock: public Clock {

		public:
			HertzClock(float frequency);
			~HertzClock(void);

		private:
			float												_frequency;
			std::chrono::time_point<std::chrono::steady_clock>	_current;

		protected:
			bool					wait(void);
	};

};

#endif

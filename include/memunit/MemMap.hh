#ifndef MEMMAP_HH
# define MEMMAP_HH
# include <list>

# include "memunit/Memory.hh"

namespace memunit {

	class	MemMap {

		public:

			MemMap(void);
			~MemMap(void);

			void insert(Memory *m) {
				auto it = _map.begin() ;
				for (; it != _map.end(); ++it) {
					Memory*	tmp = *it;

					if (tmp->base() >= m->base()) break;
				}

				if (it != _map.end()) _map.insert(it, m);
				else _map.push_back(m);
			}

			void erase(Memory *m) {
				_map.erase(m);
			}

		protected:

			Memory*		startFetch(void) {
				_cur = _map.begin();
			}

			Memory*		fetch(Memory &m) {
				while (_cur != _map.end()) {
					Memory*	tmp = *_cur;

					++_cur;

					if (tmp->base() <= m.base()
						&& m.base() + m.allocSize() <= tmp->base() + tmp->allocSize()) {
						return (tmp);
					}
				}
				return (nullptr);
			}

		private:

			std::list::iterator	_cur;
			std::list<Memory*>	_map;
	};

}



#endif

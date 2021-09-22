#pragma once
# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <sstream>

# include <algorithm>

# include <list>
# include <set>


namespace	container {

	template<typename Container>
	auto closest_element(Container& set, const typename Container::value_type& value)
	    -> decltype(set.begin())
	{
		const auto it1 = set.find(value);
		if (it1 != set.end()) {
			std::cout << "Find exactly" << std::endl;
			return (it1);
		}

	    const auto it = set.lower_bound(value);
	    if (it == set.begin()) {
			std::cout << "Find at 1st" << std::endl;
			return it;
		} else if (it == set.end()) {
			std::cout << "Find at end - 1" << std::endl;
			return std::prev(it);
		}
		std::cout << "Find at end - 1" << std::endl;
		return (std::prev(it));
///		return (value - *prev_it < value - *it) ? prev_it : it;
	}
}

namespace	memunit {

	class	MemoryDescriptor {
		public:
			virtual ~MemoryDescriptor(void) {}

			virtual std::size_t		allocSize(void) const = 0;
			virtual std::uintptr_t	base(void) const = 0;
			virtual bool			canRead(void) const = 0;
			virtual bool			canWrite(void) const = 0;
			virtual bool			canExecute(void) const = 0;
			virtual std::uint8_t*	data(void) const = 0;
	};

	class	Memory: public MemoryDescriptor {
		public:

			static const std::uint8_t	ReadOnly = 0x1;
			static const std::uint8_t	WriteOnly = 0x2;
			static const std::uint8_t	ReadWrite = 0x3;

			virtual ~Memory(void) {}

			virtual std::size_t		allocSize(void) const = 0;
			virtual std::uintptr_t	base(void) const = 0;

			virtual bool			canRead(void) const = 0;
			virtual bool			canWrite(void) const = 0;
			virtual bool			canExecute(void) const = 0;

			virtual std::size_t		write(std::uintptr_t at, const std::uint8_t *buf, std::size_t buflen);
			virtual std::size_t		read(std::uintptr_t at, std::uint8_t *buf, std::size_t buflen);
			virtual std::size_t		read(std::uintptr_t at, std::uint8_t *buf, std::size_t buflen) const;

			virtual std::uint8_t*	data(void) const = 0;

			void					printMemory(std::ostream& os, std::streamoff offset, std::size_t length) const;
			void					printMemory(std::ostream& os) const;
			static void				bufferPrintMemory(std::ostream &os, const std::uint8_t* data, size_t size, std::uintptr_t start = 0x0);
			static std::string		prettySize(std::size_t size);
	};

	class	MemoryHint: public Memory {

		private:

			std::uintptr_t	_base;

		public:

			MemoryHint(std::uintptr_t base);
			virtual ~MemoryHint(void);

			virtual std::size_t		allocSize(void) const;
			virtual std::uintptr_t	base(void) const;

			virtual bool			canRead(void) const;
			virtual bool			canWrite(void) const;
			virtual bool			canExecute(void) const;

			virtual std::uint8_t*	data(void) const;
	};

	/*****************************/

	class	Block: public Memory {

		private:

			std::uintptr_t		_start;
			std::streamoff		_pos;
			std::uint8_t		_permisions;
			std::size_t			_size;
			std::uint8_t*		_data;

		protected:

			std::size_t				writeBuffer(const std::uint8_t *buf, std::size_t buflen);
			std::size_t				readBuffer(std::uint8_t *buf, std::size_t buflen);

		public:

			Block(void);
			Block(std::uintptr_t base, std::size_t size, std::uint8_t perm = memunit::Memory::ReadWrite);
			Block(const std::uint8_t* data, std::uintptr_t base, std::size_t size, std::uint8_t perm = memunit::Memory::ReadWrite);
			Block(std::uint8_t* data, std::uintptr_t base, std::size_t size, std::uint8_t perm = memunit::Memory::ReadWrite);
			virtual ~Block(void);

			void					update(std::uint8_t *buffer, std::size_t length);

			virtual std::size_t		allocSize(void) const;
			virtual std::uintptr_t	base(void) const;

			virtual bool			canRead(void) const;
			virtual bool			canWrite(void) const;
			virtual bool			canExecute(void) const;

			virtual std::uint8_t*	data(void) const;
	};

	class	MemoryMap : public memunit::Memory {

		private:

			typedef struct hash {
				size_t				operator()(const Memory &a) const;
				size_t				operator()(const Memory *a) const;
			}						hash_t;

			typedef struct cmp {
				bool				operator()(const Memory &a, const Memory &b) const;
				bool				operator()(const Memory *a, const Memory *b) const;
			}						cmp_t;

//			std::set<Memory*, cmp_t>	_blocks;
			memunit::MemMap				_blocks;
			std::list<std::uint8_t*>	_garbage;

			Memory*					find(std::uintptr_t addr) const;
			Memory*					find(memunit::Memory *m) const;

		public:

			MemoryMap(void);
			~MemoryMap(void);

			void		show(void) const;

			//void		set(std::uintptr_t start, std::uintptr_t end, MemoryIO *callbacks = nullptr);
			//void		set(MemoryDescriptor const & description, MemoryIO *callbacks = nullptr);
			void		set(memunit::Memory *allocd);

			void		miror(std::uintptr_t start, std::uintptr_t tomiror);

			virtual std::size_t		allocSize(void) const;
			virtual std::uintptr_t	base(void) const;
			virtual std::uint8_t*	data(void) const;

			virtual bool			canRead(void) const;
			virtual bool			canWrite(void) const;
			virtual bool			canExecute(void) const;

			std::size_t		write(std::uintptr_t addr, const std::uint8_t* data, std::size_t len);
			std::size_t		read(std::uintptr_t addr, std::uint8_t* data, std::size_t len);

			void			clear(void);
			std::size_t		size(void) const;
	};

	std::ostream&	operator<<(std::ostream &os, Memory const &m);
};

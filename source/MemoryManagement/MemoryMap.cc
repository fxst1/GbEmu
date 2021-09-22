#include "memunit/MemUnit.hh"

/*
bool			memunit::Memory::operator==(const Memory &a) {
	return (a.base() <= this->base() ? a.base() + a.allocSize() >= this->base() + this->allocSize()
								: this->base() + this->allocSize() >= a.base() + a.allocSize());
}
*/

std::string		memunit::Memory::prettySize(std::size_t size) {

	static char	units[] = "_KMGTT";

	std::string	ret = "";
	std::size_t	unit = 0;

	while (unit < 5 && size > 1024) {
		size /= 1024;
		unit++;
	}

	std::stringstream	sstr;

	sstr << size;
	if (unit >= 1)
		sstr << units[ unit ];
	sstr << "b";
	return ( sstr.str() );
}

std::ostream&	memunit::operator<<(std::ostream &os, Memory const &m) {
	os << "[0x" << std::hex << m.base() << ", 0x" << std::hex << (std::uintptr_t)((m.base() + m.allocSize()) - 1) << ']';
	return (os);
}

void			memunit::Memory::printMemory(std::ostream &os) const {
	memunit::Memory::printMemory(os, 0, this->allocSize());
}

void			memunit::Memory::printMemory(std::ostream &os, std::streamoff start, size_t size) const {

	std::size_t	end = (start + size);
	if (end > this->base() + this->allocSize()) {
		end = this->base() + this->allocSize();
	}

	std::uintptr_t	zero_offset = 0;
	for (size_t index = start; index < end; index += 16) {

		std::size_t	j = 0;
/*
		while (j < 16 && j + index < end) {
			int	c = 0;
			this->read(this->base() + index + j, (std::uint8_t*)&c, sizeof(std::uint8_t));

			if (c != 0) {
				if (zero_offset != 0) {
					os << "0x" << std::setw(8) << std::setfill('0') << std::hex  << (int)(this->base() + zero_offset);
					os << ": *** FILLED BY ZERO ***" << std::endl;
				}
				zero_offset = 0;
				break;
			}
		}

		if (zero_offset > 0) {
			continue;
		}
*/
		os << "0x" << std::setw(8) << std::setfill('0') << std::hex  << (int)(this->base() + index);
		os << ":";

		j = 0;
		while (j < 16 && j + index < end) {
			int	c = 0;
			this->read(this->base() + index + j, (std::uint8_t*)&c, sizeof(std::uint8_t));

			os << ' ' << std::setw(2) << std::setfill('0') << std::hex << c;
			j++;
		}

		while (j < 16) {
			os << "   ";
			j++;
		}

		os << "     ";

		for (size_t j2 = 0; j2 < 16 && j2 + index < end; j2++) {
			int	c = 0;
			this->read(this->base() + index + j2, (std::uint8_t*)&c, sizeof(std::uint8_t));
			if (::isprint(c))
				os << (char)c;
			else
				os << '.';
		}

		os << std::endl;
	}
}


void			memunit::Memory::bufferPrintMemory(std::ostream &os, const std::uint8_t* data, size_t size, std::uintptr_t start) {

	for (size_t index = 0; index < size; index += 16) {

		os << "0x" << std::setw(8) << std::setfill('0') << std::hex  << (start+index);
		os << ":";

		size_t		j = 0;
		while (j < 16 && j + index < size) {
			int	c = *(data + index + j);

			os << ' ' << std::setw(2) << std::setfill('0') << std::hex << c;
			j++;
		}

		while (j < 16) {
			os << "   ";
			j++;
		}

		os << "     ";

		for (size_t j2 = 0; j2 < 16 && j2 + index < size; j2++) {
			int	c = *(data + index + j2);

			if (::isprint(c))
				os << (char)c;
			else
				os << '.';
		}

		os << std::endl;
	}
}

/******************************************************/

memunit::MemoryMap::MemoryMap(void):
	_blocks(),
	_garbage()
{}

memunit::MemoryMap::~MemoryMap(void) {
	this->clear();
}

void				memunit::MemoryMap::show(void) const {

	for (auto it = this->_blocks.begin(); it != this->_blocks.end(); it++) {
		std::cout << (**it) << std::endl;
	}
}

std::size_t			memunit::MemoryMap::write(std::uintptr_t addr, const std::uint8_t* data, size_t len) {

	size_t			writelen = 0;
	Memory*			tmp = nullptr;

	while (writelen < len) {

		tmp = this->find(addr + writelen);
		if (tmp != nullptr) {
			//std::cerr << "Write request at 0x" << std::hex << (int)(addr + writelen) << '(' << std::dec << (len - writelen) << ')' << std::endl;
			tmp->write(addr + writelen, data + writelen, len - writelen);
			writelen += tmp->allocSize() - (addr - tmp->base());
			addr += tmp->allocSize() - (addr - tmp->base());
		} else {
			std::cerr << "Write request at 0x" << std::hex << addr << " overflow !" << std::endl;
			break ;
		}
	}
	return (writelen);
}

std::size_t			memunit::MemoryMap::read(std::uintptr_t addr, std::uint8_t* data, size_t len) {
	size_t			readlen = 0;
	Memory*			tmp = nullptr;

	static int t = 0;
	while (readlen < len) {
		tmp = this->find(addr);
		if (tmp != nullptr) {
			tmp->read(addr + readlen, data + readlen, len - readlen);
			readlen += tmp->allocSize() - (addr - tmp->base());
			addr += tmp->allocSize() - (addr - tmp->base());

			if (addr == 0x134) {
				std::cout << "Read request at 0x" << std::hex << addr << std::endl;
				std::cout << "-> read length: 0x" << std::hex << (tmp->allocSize() - (addr - tmp->base())) << std::endl;
				std::cout << "-> allocsize: 0x" << std::hex << tmp->allocSize() << std::endl;
				std::cout << "-> base: 0x" << std::hex << tmp->base() << std::endl;
				std::cout << "-> length: 0x" << std::hex << len << std::endl;
				t++;
			}

			if (t >= 5) exit(0);

		} else {
			std::cerr << "Read request at 0x" << std::hex << addr << " overflow !" << std::endl;
			break ;
		}
	}
	return (readlen);
}

/******************************/

std::size_t					memunit::MemoryMap::allocSize(void) const {
	return (0xffff);
}

std::uintptr_t				memunit::MemoryMap::base(void) const {
	return (0);
}

bool						memunit::MemoryMap::canRead(void) const {
	return (true);
}
bool						memunit::MemoryMap::canWrite(void) const {
	return (true);
}
bool						memunit::MemoryMap::canExecute(void) const {
	return (true);
}

std::uint8_t*				memunit::MemoryMap::data(void) const {
	return (nullptr);
}

/******************************/


memunit::Memory*			memunit::MemoryMap::find(std::uintptr_t addr) const {
	memunit::MemoryHint		req = MemoryHint(addr);
	return (this->find(&req));
}

memunit::Memory*	memunit::MemoryMap::find(memunit::Memory *req) const {

/*
	auto it1 = _blocks.find(req);
	if (it1 != _blocks.end()) {
		//std::cout << "Find exactly" << std::endl;
		return (*it1);
	} else {
		auto it = _blocks.lower_bound(req);
		if (it == _blocks.begin()) {
			//std::cout << "Find at 1st" << std::endl;
			return *it;
		} else if (it == _blocks.end()) {
			auto ret = std::prev(it);
			//std::cout << "Find at end - 1" << std::endl;
			return (ret == _blocks.end() ? nullptr : *ret);
		} else {
			auto ret = std::prev(it);
			//std::cout << "Find at end - 1" << std::endl;
//			return (std::prev(it));
			return (ret == _blocks.end() ? nullptr : *ret);
		}
	}

//	auto			parent = container::closest_element(this->_blocks, req);
//	return (parent == this->_blocks.end() ? nullptr : *parent);

*/
	this->startFetch();

}

void				memunit::MemoryMap::set(memunit::Memory *allocd) {
	if (allocd == nullptr) {
		//std::uint8_t* t = nullptr;
		//*t = 0;
	} else {
		this->_blocks.insert(allocd);
	}
}

void				memunit::MemoryMap::miror(std::uintptr_t addr, std::uintptr_t mirror_addr) {
	memunit::Memory*		found;
	memunit::MemoryHint		req = MemoryHint(addr);

	this->startFetch();
	while ((found = this->fetch()) != nullptr) {
		memunit::Block* blk = new memunit::Block(found->data(), mirror_addr, found->allocSize());
		this->_blocks.insert( blk );
	}
}

/******************************/

void				memunit::MemoryMap::clear(void) {
	for (auto it = _garbage.begin(); it != _garbage.end(); it++) {
		delete (*it);
	}
	_blocks.clear();
	_garbage.clear();
}

std::size_t			memunit::MemoryMap::size(void) const {
	auto it = this->_blocks.begin();
	if (it == this->_blocks.end() || *it == nullptr) {
		return (0);
	}
	return ((*it)->base() + (*it)->allocSize());
}

/******************************/

bool				memunit::MemoryMap::cmp_t::operator()(const memunit::Memory &a, const memunit::Memory &b) const {
	return (a.base() < b.base());
}

bool				memunit::MemoryMap::cmp_t::operator()(const memunit::Memory *a, const memunit::Memory *b) const {
	if (a == nullptr || b == nullptr) return (false);
	return (memunit::MemoryMap::cmp_t::operator()(*a, *b));
}

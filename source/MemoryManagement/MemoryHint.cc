#include "memunit/MemUnit.hh"

memunit::MemoryHint::MemoryHint(std::uintptr_t base):
	_base(base)
{}

memunit::MemoryHint::~MemoryHint(void)
{}

/**********************/

bool			memunit::MemoryHint::canRead(void) const {
	return (false);
}
bool			memunit::MemoryHint::canWrite(void) const {
	return (false);
}
bool			memunit::MemoryHint::canExecute(void) const {
	return (false);
}
/*********************/

std::size_t		memunit::Memory::write(std::uintptr_t at, const std::uint8_t* data, std::size_t len) {
	std::size_t		writelen = 0;
	std::uint8_t*	buf = this->data();

	if (this->canWrite() && buf != nullptr && (this->base() <= at) && at + len <= this->base() + this->allocSize()) {

		std::size_t		offset = at - this->base();

		writelen = len;
/*

		writelen = len;

		if () {
			offset = this->base();
		}
		if (this->base() + this->allocSize() < offset + writelen) {
			writelen = (this->base() + this->allocSize()) - offset;
		}

		if (offset < this->base() || (this->base() + this->allocSize() < offset + writelen)) {
			writelen = 0;
		} else {
*/
			::memcpy(this->data() + offset, data, writelen);
//		}
	} else {
		std::cout << "Could not write at " << std::hex << (int)at << std::endl;
		std::cout << *this << std::endl;
		writelen = 0;
	}
	return (writelen);
}

std::size_t		memunit::Memory::read(std::uintptr_t at, std::uint8_t* data, std::size_t len) const {
	std::size_t		readlen = 0;
	std::uint8_t*	buf = this->data();

	if (this->canRead() && buf != nullptr && (this->base() <= at) && at + len <= this->base() + this->allocSize()) {
		std::size_t		offset = at - this->base();
		readlen = len;
/*
		std::size_t		offset = at;

		readlen = len;

		if (this->base() > at || offset > this->base() + this->allocSize()) {
			offset = this->base();
		}
		if (this->base() + this->allocSize() < offset + readlen) {
			readlen = (this->base() + this->allocSize()) - offset;
		}

		if (offset < this->base() || (this->base() + this->allocSize() < offset + readlen)) {
			readlen = 0;
		} else {*/
			::memcpy(data, this->data() + offset, readlen);
		//}
	} else {
		readlen = 0;
	}
	return (readlen);
}

std::size_t		memunit::Memory::read(std::uintptr_t at, std::uint8_t* data, std::size_t len) {
	std::size_t		readlen = 0;
	std::uint8_t*	buf = this->data();

	if (this->canRead() && buf != nullptr && at >= this->base()) {
		std::size_t		offset = 0;
		if (at + len <= this->base() + this->allocSize()) {
			readlen = len;
			offset = at - this->base();
		} else if (this->base() <= at) {
			readlen = this->allocSize() - len;
		}
		::memcpy(data, this->data() + offset, readlen);
	} else {
		readlen = 0;
	}
	return (readlen);
}

/********************/

size_t			memunit::MemoryHint::allocSize(void) const {
	return (0);
}

std::uint8_t*	memunit::MemoryHint::data(void) const {
	return (nullptr);
}

std::uintptr_t	memunit::MemoryHint::base(void) const {
	return (this->_base);
}

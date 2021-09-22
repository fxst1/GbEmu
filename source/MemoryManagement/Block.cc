#include "memunit/MemUnit.hh"

memunit::Block::Block(void):
	_start(0),
	_pos(0),
	_permisions(memunit::Memory::ReadWrite),
	_size(0),
	_data(nullptr)
{}

memunit::Block::Block(const std::uint8_t* data, std::uintptr_t base, std::size_t len, std::uint8_t perm):
	_start(base),
	_pos(0),
	_permisions(perm),
	_size(len),
	_data(new std::uint8_t[_size])
{
	::memcpy(_data, data, len);
}
memunit::Block::Block(std::uint8_t* data, std::uintptr_t base, std::size_t len, std::uint8_t perm):
	_start(base),
	_pos(0),
	_permisions(perm),
	_size(len),
	_data(data)
{}

memunit::Block::Block(std::uintptr_t base, std::size_t len, std::uint8_t perm):
	_start(base),
	_pos(0),
	_permisions(perm),
	_size(len),
	_data(new std::uint8_t[_size])
{
	::bzero(_data, _size);
}

memunit::Block::~Block(void)
{}

/**********************/

bool			memunit::Block::canRead(void) const {
	return (this->_data != nullptr && (this->_permisions & memunit::Memory::ReadOnly) != 0);
}
bool			memunit::Block::canWrite(void) const {
	return (this->_data != nullptr && (this->_permisions & memunit::Memory::WriteOnly) != 0);
}
bool			memunit::Block::canExecute(void) const {
	return (this->_data != nullptr && false);
}

/*********************/

void			memunit::Block::update(std::uint8_t *buffer, std::size_t length) {
	this->_data = buffer;
	this->_size = length;
}

std::size_t 	memunit::Block::readBuffer(uint8_t* data, size_t len) {

	size_t templen = 0;

	templen = (this->_pos + len > this->_size) ? this->_size : len;
	::memcpy(data, (this->_data + this->_pos), templen);
	this->printMemory(std::cout, this->_pos, len);
	this->_pos += templen;
	return (templen);
}

/********************/

size_t			memunit::Block::allocSize(void) const {
	return (this->_size);
}
std::uint8_t*	memunit::Block::data(void) const {
	return (this->_data);
}
std::uintptr_t	memunit::Block::base(void) const {
	return (this->_start);
}
/*
void	memunit::Block::bind(void*& tobind, size_t bindsize) const {
	tobind = ((void*)this->data());
	(void)bindsize;
}*/

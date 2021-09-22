#include "extra/Gb.hh"

gameboy::GbGameLoader::GbGameLoader(void):
	_input(),
	_head(),
	_filename(),
	_filesize(),
	_filedata(nullptr)
{}

gameboy::GbGameLoader::GbGameLoader(std::string const &filename):
	_input(filename),
	_head(),
	_filename(),
	_filesize(),
	_filedata(nullptr)
{ this->load(filename); }

gameboy::GbGameLoader::~GbGameLoader(void) {
	if (this->_filedata) delete[] this->_filedata;
}

std::fstream&			gameboy::GbGameLoader::file() {
	return (_input);
}


void					gameboy::GbGameLoader::loadBoot(std::string const &filename) {

	_input.open(filename, std::ios::in|std::ios::binary|std::ios::ate);
	if (!_input) {
		throw std::exception();
	}

	_input.seekg(0x0, std::ios_base::end);
	std::size_t	filesize = _input.tellg();

	_head.entrypoint = sizeof(_head) + 0x100;
	::memcpy(&_head.logo, gameboy::const_t::NintendoLogo, 48);
	::memcpy(_head.title, (const char*)"bootbootbootboot", sizeof(_head.title));
	::bzero(_head.manufacturer, sizeof(_head.manufacturer));
	_head.cgb = 0x0;
	_head.licence = 0x00;
	_head.sgb = 0x0;
	_head.carttype = 0x0;
	_head.romsize = 0x0;
	_head.ramsize = 0x0;
	_head.destcode = 0x0;
	_head.oldlicence = 0x0;
	_head.version = 0x0;
	_head.head_checksum = this->computeHeaderChecksum();
	_head.global_checksum = 0;

	std::uint8_t* filebuf = new std::uint8_t[filesize];
	if (filebuf == nullptr) {
		throw std::exception();
	}

	this->_filedata = filebuf;
	this->_filesize = filesize;
	this->_filename = filename;

	_input.seekg(0x0, std::ios::beg);
	_input.read((char*)filebuf, filesize);
	::memcpy(&_head, filebuf + 0x100, sizeof(_head));

	if (this->check()) {
		std::cout << *this;
	} else {
		std::cerr << "Rom error !" << std::endl;
	}

	_input.close();
}

void					gameboy::GbGameLoader::load(std::string const &filename) {

	_input.open(filename, std::ios::in|std::ios::binary|std::ios::ate);
	if (!_input) {
		throw std::exception();
	}

	_input.seekg(0x0, std::ios_base::end);
	std::size_t	filesize = _input.tellg();
	if (filesize < sizeof(_head) + 0x100) {
		throw std::exception();
	}

	std::uint8_t* filebuf = new std::uint8_t[filesize];
	if (filebuf == nullptr) {
		throw std::exception();
	}

	this->_filedata = filebuf;
	this->_filesize = filesize;
	this->_filename = filename;

	_input.seekg(0x0, std::ios::beg);
	_input.read((char*)filebuf, filesize);
	::memcpy(&_head, filebuf + 0x100, sizeof(_head));

	if (this->check()) {
		std::cout << *this;
	} else {
		std::cerr << "Rom error !" << std::endl;
	}

	_input.close();
}

/******************************/

std::string		gameboy::GbGameLoader::filename(void) const {
	return (this->_filename);
}
std::size_t		gameboy::GbGameLoader::filesize(void) const {
	return (this->_filesize);
}
std::uint8_t*	gameboy::GbGameLoader::filebuf(void) const {
	return (this->_filedata);
}

/******************************/

std::string		gameboy::GbGameLoader::getGameTitle(void) const {
	return (std::string(_head.title));
}
std::string		gameboy::GbGameLoader::getPublisher(void) const  {
	auto publisher = gameboy::const_t::Publishers.find( _head.licence );
	if (publisher == gameboy::const_t::Publishers.end())
		return ("<?>");
	else
		return (publisher->second);
}
std::string		gameboy::GbGameLoader::getManufacturer(void) const {
	return (std::string(_head.manufacturer));
}
std::size_t				gameboy::GbGameLoader::getOldLicence(void) const {
	return (_head.oldlicence);
}
std::size_t				gameboy::GbGameLoader::getVersion(void) const {
	return (_head.version);
}
std::size_t				gameboy::GbGameLoader::getHeaderChecksum(void) const {
	return (_head.head_checksum);
}
std::size_t				gameboy::GbGameLoader::getGlobalChecksum(void) const {
	return (_head.global_checksum);
}

gameboy::CartridgeType		gameboy::GbGameLoader::getCartridgeType(void) const {
	auto	ctype = gameboy::const_t::CartTypes.find( getCartridgeTypeFlag() );
	if (ctype == gameboy::const_t::CartTypes.end()) {
		gameboy::CartridgeType tmp = {gameboy::CartridgeType::Version::ROM, 0, 0, 0, 0, "<?>"};
		return (tmp);
	}
	return (ctype->second);
}
std::size_t				gameboy::GbGameLoader::getCartridgeTypeFlag(void) const {
	return (_head.carttype);
}

bool			gameboy::GbGameLoader::check(void) const {
	if (this->checkLogo() == false) {
		std::cerr << "Invalid Nintendo logo" << std::endl;
	}
	if (this->checkHeader() == false) {
		std::cerr << "Invalid Header Checksum" << std::endl;
	}
	return (true);
}

std::uint8_t	gameboy::GbGameLoader::computeHeaderChecksum(void) const {
	std::uint8_t		sum = 0;
	const std::uint8_t*	buf = reinterpret_cast<const std::uint8_t*>(&_head);

	for (std::uint8_t i = 0x34; i < 0x4d; i++) {
		sum += ~(*(buf + i));
	}
	return (sum);
}
bool			gameboy::GbGameLoader::checkHeader(void) const {
	return (this->computeHeaderChecksum() == _head.head_checksum);
}

bool			gameboy::GbGameLoader::checkLogo(void) const {
	return (::memcmp(_head.logo, gameboy::const_t::NintendoLogo, 48) == 0);
}

bool			gameboy::GbGameLoader::needColorSupport(void) const {
	return (_head.cgb == 0x80 || _head.cgb == 0xC0);
}
bool			gameboy::GbGameLoader::needSuperSupport(void) const {
	return (_head.sgb == 0x03);
}

std::size_t		gameboy::GbGameLoader::getRomSize(void) const {
	if (_head.romsize <= 0x08) return (0x8000 << _head.romsize);
	else if (_head.romsize == 0x52) return (0x4000 * 72);
	else if (_head.romsize == 0x53) return (0x4000 * 80);
	else if (_head.romsize == 0x54) return (0x4000 * 96);
	return (0);
}
std::size_t		gameboy::GbGameLoader::getRomFlag(void) const {
	return (_head.romsize);
}
std::size_t		gameboy::GbGameLoader::getRamSize(void) const {
	if (_head.ramsize == 0x0) return (0);
	else if (_head.ramsize <= 0x3) return (2048 << (_head.ramsize + 1));
	else if (_head.ramsize == 0x04) return (1024 * 128);
	else if (_head.ramsize == 0x05) return (1024 * 64);
	return (0);
}
std::size_t		gameboy::GbGameLoader::getRamFlag(void) const {
	return (_head.ramsize);
}
std::ostream&	gameboy::operator<<(std::ostream& os, gameboy::GbGameLoader const &loader) {
	os << "Gb Game Loader:" << std::endl;

	if (loader.check())
		os << " --- This is a valid file ---" << std::endl;
	else
		os << " !!! This is not a valid file !!!" << std::endl;

	os << " - Title           : " << loader.getGameTitle() << std::endl;
	os << " - Manufacturer    : " << loader.getManufacturer() << std::endl;
	os << " - Licence         : " << loader.getPublisher() << std::endl;
	os << " - Old Licence     : 0x" << std::hex << loader.getOldLicence() << std::endl;

	os << " - CGB             : " << (loader.needColorSupport() ? "Yes" : "No") << std::endl;
	os << " - SGB             : " << (loader.needSuperSupport() ? "Yes" : "No") << std::endl;

	os << " - Rom size        : 0x" << std::hex << loader.getRomFlag() << " - " << memunit::Memory::prettySize(loader.getRomSize()) << std::endl;
	os << " - Ram size        : 0x" << std::hex << loader.getRamFlag() << " - " << memunit::Memory::prettySize(loader.getRamSize()) << std::endl;

	gameboy::CartridgeType	type = loader.getCartridgeType();
	os << " - Cartridge Type  : 0x" << std::hex << loader.getCartridgeTypeFlag() << " - " << type.str << std::endl;
	os << " - Version #       : " << std::dec << loader.getVersion() << std::endl;
	os << " - Header Checksum : 0x" << std::hex << loader.getHeaderChecksum() << std::endl;
	os << " - Global Checksum : 0x" << std::hex << loader.getGlobalChecksum() << std::endl;
	return (os);
}

std::ostream&	gameboy::operator<<(std::ostream& os, struct gameboy::CartridgeHeader const &gbhead) {
	os << "GbHead:" << std::endl;
	os << " - Entry 0x" << std::hex << gbhead.entrypoint << std::endl;

	if (::memcmp(gbhead.logo, gameboy::const_t::NintendoLogo, 48) == 0)
		os << " - Logo OK" << std::endl;
	else
		os << " - Logo Error" << std::endl;

	os << " - Title " << gbhead.title << std::endl;
	os << " - Manufacturer " << gbhead.manufacturer << std::endl;

	if (gbhead.cgb == 0x80)
		os << " - CGB Support" << std::endl;
	else if (gbhead.cgb == 0xC0)
		os << " - CGB Only" << std::endl;
	else
		os << " - No CGB" << std::endl;

	auto publisher = gameboy::const_t::Publishers.find( gbhead.licence );
	if (publisher == gameboy::const_t::Publishers.end())
		os << " - New Licence Code <undefined>" << std::endl;
	else
		os << " - New Licence Code " << publisher->second << std::endl;

	if (gbhead.sgb == 0x03)
		os << " - SGB On" << std::endl;
	else
		os << " - No SGB" << std::endl;

	return (os);
}

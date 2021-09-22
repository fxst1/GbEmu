#include "extra/Gb.hh"
#include <iostream>

gameboy::States::States(void):emu::States(){}
gameboy::States::~States(void){}

std::uint8_t*	gameboy::States::programCounter(void) const {
	return ((std::uint8_t*)&this->_cpuregs->pc);
}
std::uint8_t*	gameboy::States::stackPointer(void) const {
	return ((std::uint8_t*)&this->_cpuregs->sp);
}

/************************/

gameboy::Gameboy::Gameboy(gameboy::States& states):
	_clock(4.194304 * 100000),
	_stat(states),
	_ppu(),
	_cpu(this->_stat),
	_loader(),
	_memmap()
{
//	this->_clock.add(_cpu);
//	this->_clock.add(_ppu);
}
gameboy::Gameboy::~Gameboy(void){
	std::cout << "* Gameboy Stopped" << std::endl;
}
gameboy::States&		gameboy::Gameboy::states(void) {
	return (_stat);
}
timing::Clock&			gameboy::Gameboy::clock(void) {
	return (_clock);
}
opcode::InstructionSet 	gameboy::Gameboy::instructionSet(void) {
	return (_cpu.opcodes());
}
memunit::MemoryMap* 	gameboy::Gameboy::memoryMap(void) {
	return (&_memmap);
}
gameboy::CpuRegisters* 	gameboy::Gameboy::cpuRegisters(void) {
	return (_cpu.regs());
}
gameboy::GbPpu*		 	gameboy::Gameboy::ppu(void) {
	return (&_ppu);
}

void	gameboy::Gameboy::load(std::string const &filename) {
	this->_loader.load(filename);
	this->_memmap.init( this->_loader );

	this->_stat._memmap = &this->_memmap;

	this->_cpu.setMemory(this->_memmap);
	this->_ppu.setMemory(this->_memmap);
	this->_ppu.setVram(this->_memmap.getVram());
}

void	gameboy::Gameboy::loadBoot(std::string const &filename) {

	this->_loader.loadBoot(filename);
	this->_memmap.init( this->_loader );

	this->_stat._memmap = &this->_memmap;

	this->_cpu.setMemory(this->_memmap);
	this->_ppu.setMemory(this->_memmap);
	this->_ppu.setVram(this->_memmap.getVram());
}

void	gameboy::Gameboy::setDisplayDriver(gameboy::GbPpu::LCDInterface* disp) {
	this->_ppu.setDriver(disp);
}

void	gameboy::Gameboy::onAfterCycle(void) {
/*
	SDL_Event	event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			this->stop();
	}
*/
}

void	gameboy::Gameboy::start(void) {
	this->_stat.setBooting(true);
	this->_stat.start();
}

void	gameboy::Gameboy::mainloop(void) {

	std::uint32_t	minimum_fps_delta_time = 1000 / 20;
	std::uint32_t	last_game_step = SDL_GetTicks();
	bool			exit_loop = false;

	std::uint32_t	next_game_step = last_game_step + minimum_fps_delta_time;
	while (!exit_loop) {

		std::uint32_t	now = SDL_GetTicks();

		if ((int)last_game_step <= (int)now) { // | vsync_enabled

			std::uint32_t	delta_time = now - last_game_step;
			if (delta_time > minimum_fps_delta_time) {
				delta_time = minimum_fps_delta_time; // slow down if the computer is too slow
			}

//			_engine._delta_time = delta_time;


			// Look on server
			if (this->_stat.isRunning() == false) {
				exit_loop = true;
			}

			this->tick();

			if (last_game_step >= next_game_step) {
				SDL_Event event;
				next_game_step = last_game_step + minimum_fps_delta_time;
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_QUIT) {
						exit_loop = true;
					}
				}
			}

			last_game_step = now;
		} else {
			SDL_Delay(1);
		}
	}
}

void	gameboy::Gameboy::tick(void) {

	this->_cpu.tick();
	this->_ppu.tick();
	this->_ppu.tick();

//	this->onAfterCycle();
}

void	gameboy::Gameboy::stop(void) {
	this->_stat.lock();
	this->_stat.stop();
	this->_stat.unlock();
}

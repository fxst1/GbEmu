#include "extra/GbCpu.hh"

void	gameboy::GbCpu::prepare(void) {
	opcode::Builder		builder(this->_opcodes);
	std::uint32_t		start = 0x00;

	auto				bytecode_binder_cond = [&start] (opcode::Placeholders const &holders) -> int {
		opcode::bytecode_t	bcode = 0;

		if (holders[0].name == "nz") bcode = start + 0x00;
		else if (holders[0].name == "z") bcode = start + 0x08;
		else if (holders[0].name == "nc") bcode = start + 0x10;
		else if (holders[0].name == "c") bcode = start + 0x18;

		return (bcode);
	};

	auto				bytecode_binder8_prefix_cb = [&start] (opcode::Placeholders const &holders) -> int {
		opcode::bytecode_t	bcode = 0;

		if (holders[1].name == "B") bcode = start + 0x00;
		else if (holders[1].name == "C") bcode = start + 0x01;
		else if (holders[1].name == "D") bcode = start + 0x02;
		else if (holders[1].name == "E") bcode = start + 0x03;
		else if (holders[1].name == "H") bcode = start + 0x04;
		else if (holders[1].name == "L") bcode = start + 0x05;
		else if (holders[1].name == "(HL)") bcode = start + 0x06;
		else if (holders[1].name == "A") bcode = start + 0x07;
		else if (holders[1].name == "F") bcode = start + 0x08;

		return (bcode + (*holders[0].data) * 0x08);
	};

	auto				bytecode_binder8_2_prefix_cb = [&start] (opcode::Placeholders const &holders) -> int {
		return (start + (*holders[0].data) * 0x08);
	};

	auto				bytecode_binder7_1 = [&start] (opcode::Placeholders const &holders) -> int {
		opcode::bytecode_t	bcode = 0;

		if (holders[0].name == "B") bcode = start + 0x00;
		else if (holders[0].name == "C") bcode = start + 0x08;
		else if (holders[0].name == "D") bcode = start + 0x10;
		else if (holders[0].name == "E") bcode = start + 0x18;
		else if (holders[0].name == "H") bcode = start + 0x20;
		else if (holders[0].name == "L") bcode = start + 0x28;
		else if (holders[0].name == "A") bcode = start + 0x38;

		return (bcode);
	};

	auto				bytecode_binder8_1 = [&start] (opcode::Placeholders const &holders) -> int {
		opcode::bytecode_t	bcode = 0;

		if (holders[0].name == "B") bcode = start + 0x00;
		else if (holders[0].name == "C") bcode = start + 0x08;
		else if (holders[0].name == "D") bcode = start + 0x10;
		else if (holders[0].name == "E") bcode = start + 0x18;
		else if (holders[0].name == "H") bcode = start + 0x20;
		else if (holders[0].name == "L") bcode = start + 0x28;
		else if (holders[0].name == "(HL)") bcode = start + 0x30;
		else if (holders[0].name == "A") bcode = start + 0x38;

		return (bcode);
	};

	auto				bytecode_binder8_inc = [&start] (opcode::Placeholders const &holders) -> int {
		opcode::bytecode_t	bcode = 0;

		if (holders[0].name == "B") bcode = start + 0x00;
		else if (holders[0].name == "C") bcode = start + 0x01;
		else if (holders[0].name == "D") bcode = start + 0x02;
		else if (holders[0].name == "E") bcode = start + 0x03;
		else if (holders[0].name == "H") bcode = start + 0x04;
		else if (holders[0].name == "L") bcode = start + 0x05;
		else if (holders[0].name == "(HL)") bcode = start + 0x06;
		else if (holders[0].name == "A") bcode = start + 0x07;
		else if (holders[0].name == "F") bcode = start + 0x08;

		return (bcode);
	};

	auto				bytecode_binder8_2 = [&start] (opcode::Placeholders const &holders) -> int {
		opcode::bytecode_t	bcode = 0;

		if (holders[0].name == "B") bcode = start + 0x00;
		else if (holders[0].name == "C") bcode = start + 0x08;
		else if (holders[0].name == "D") bcode = start + 0x10;
		else if (holders[0].name == "E") bcode = start + 0x18;
		else if (holders[0].name == "H") bcode = start + 0x20;
		else if (holders[0].name == "L") bcode = start + 0x28;
		else if (holders[0].name == "(HL)") bcode = start + 0x30;
		else if (holders[0].name == "A") bcode = start + 0x38;

		if (holders[1].name == "B") bcode += 0x00;
		else if (holders[1].name == "C") bcode += 0x01;
		else if (holders[1].name == "D") bcode += 0x02;
		else if (holders[1].name == "E") bcode += 0x03;
		else if (holders[1].name == "H") bcode += 0x04;
		else if (holders[1].name == "L") bcode += 0x05;
		else if (holders[1].name == "(HL)") bcode += 0x06;
		else if (holders[1].name == "A") bcode += 0x07;

		return (bcode);
	};

	auto				bytecode_binder16_1 = [&start] (opcode::Placeholders const &holders) -> int {
		opcode::bytecode_t	bcode = 0;

		if (holders[0].name == "BC") bcode = start + 0x00;
		else if (holders[0].name == "DE") bcode = start + 0x10;
		else if (holders[0].name == "HL") bcode = start + 0x20;
		else if (holders[0].name == "AF" || holders[0].name == "SP") bcode = start + 0x30;

		return (bcode);
	};

	static std::uint8_t		bytes_val_placeholders[] = {0, 1, 2, 3, 4, 5, 6, 7};
	std::map<std::string, std::uint8_t*>	bytes_placeholders = {
		{"0", &bytes_val_placeholders[0]},
		{"1", &bytes_val_placeholders[1]},
		{"2", &bytes_val_placeholders[2]},
		{"3", &bytes_val_placeholders[3]},
		{"4", &bytes_val_placeholders[4]},
		{"5", &bytes_val_placeholders[5]},
		{"6", &bytes_val_placeholders[6]},
		{"7", &bytes_val_placeholders[7]}
	};

	std::map<std::string, std::uint8_t*>	cond_placeholders = {
		{"nz", nullptr},
		{"z", nullptr},
		{"nc", nullptr},
		{"z", nullptr}
	};

	std::map<std::string, std::uint8_t*>	reg8_7_placeholders;
	reg8_7_placeholders["B"] = &this->_regs.b;
	reg8_7_placeholders["C"] = &this->_regs.c;
	reg8_7_placeholders["D"] = &this->_regs.d;
	reg8_7_placeholders["E"] = &this->_regs.e;
	reg8_7_placeholders["H"] = &this->_regs.h;
	reg8_7_placeholders["L"] = &this->_regs.l;
	reg8_7_placeholders["A"] = &this->_regs.a;

	std::map<std::string, std::uint8_t*>	reg16_1_placeholders;
	reg16_1_placeholders["BC"] = (std::uint8_t*)&this->_regs.bc;
	reg16_1_placeholders["DE"] = (std::uint8_t*)&this->_regs.de;
	reg16_1_placeholders["HL"] = (std::uint8_t*)&this->_regs.hl;
	reg16_1_placeholders["SP"] = (std::uint8_t*)&this->_regs.sp;

	std::map<std::string, std::uint8_t*>	reg16_2_placeholders;
	reg16_2_placeholders["AF"] = (std::uint8_t*)&this->_regs.af;
	reg16_2_placeholders["BC"] = (std::uint8_t*)&this->_regs.bc;
	reg16_2_placeholders["DE"] = (std::uint8_t*)&this->_regs.de;
	reg16_2_placeholders["HL"] = (std::uint8_t*)&this->_regs.hl;
//	reg16_placeholders["SP"] = (std::uint8_t*)&this->_regs.sp;
//	reg16_placeholders["PC"] = (std::uint8_t*)&this->_regs.pc;

	start = 0x40;
	builder
		.begin()
			.mnemonic("ld $r $r")
			.bytecode(bytecode_binder8_2)
			.placeholder("r", reg8_7_placeholders)
			.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
				::memcpy(holders[0].data, holders[1].data, sizeof(std::uint8_t));

				(void)op;
			})
			.cycles(4)
		.end();

	start = 0x06;
	builder
		.begin()
			.mnemonic("ld $r %8")
			.bytecode(bytecode_binder8_1, 2)
			.placeholder("r", reg8_7_placeholders)
			.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
				std::uint8_t	hint = 0;
				_memmap->read(_regs.pc + 1, &hint, sizeof(hint));
				::memcpy(holders[0].data, &hint, sizeof(hint));

				(void)op;
			})
			.cycles(8)
		.end();

	start = 0x46;
	builder
		.begin()
			.mnemonic("ld $r (HL)")
			.bytecode(bytecode_binder8_1)
			.placeholder("r", reg8_7_placeholders)
			.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
				std::uint8_t	hint = 0;
				_memmap->read(_regs.hl, &hint, sizeof(hint));
				::memcpy(holders[0].data, &hint, sizeof(hint));

				(void)op;
			})
			.cycles(8)
		.end();

	start = 0x70;
	builder
		.begin()
			.mnemonic("ld (HL) $r")
			.bytecode(bytecode_binder8_inc)
			.placeholder("r", reg8_7_placeholders)
			.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
				_memmap->write(_regs.hl, holders[0].data, sizeof(std::uint8_t));
				(void)op;
			})
			.cycles(8)
		.end()

		.begin()
			.mnemonic("ld (HL) %8")
			.bytecode(0x36, 2)
			.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
				std::uint8_t	hint;
				_memmap->read(_regs.pc + 1, &hint, sizeof(hint));
				_memmap->write(_regs.hl, &hint, sizeof(std::uint8_t));
				(void)holders;
				(void)op;
			})
			.cycles(12)
		.end()

		.begin()
			.mnemonic("ld A (BC)")
			.bytecode(0x0A)
			.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
				_memmap->read(_regs.bc, &_regs.a, sizeof(std::uint8_t));
				(void)holders;
				(void)op;
			})
			.cycles(8)
		.end()

		.begin()
			.mnemonic("ld A (DE)")
			.bytecode(0x1A)
			.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
				_memmap->read(_regs.de, &_regs.a, sizeof(std::uint8_t));
				(void)holders;
				(void)op;
			})
			.cycles(8)
		.end()

		.begin()
			.mnemonic("ld A (%16)")
			.bytecode(0xFA, 3)
			.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
				std::uint16_t	hint = 0;
				_memmap->read(_regs.pc + 1, (std::uint8_t*)&hint, sizeof(std::uint16_t));
				_memmap->read(hint, &_regs.a, sizeof(std::uint8_t));
				(void)holders;
				(void)op;
			})
			.cycles(16)
		.end()

		.begin()
			.mnemonic("ld (BC) A")
			.bytecode(0x02)
			.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
				_memmap->write(_regs.bc, &_regs.a, sizeof(std::uint8_t));
				(void)holders;
				(void)op;
			})
			.cycles(8)
		.end()

		.begin()
			.mnemonic("ld (DE) A")
			.bytecode(0x12)
			.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
				_memmap->write(_regs.de, &_regs.a, sizeof(std::uint8_t));
				(void)holders;
				(void)op;
			})
			.cycles(8)
		.end()

		.begin()
			.mnemonic("ld (%16) A")
			.bytecode(0xEA, 3)
			.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
				std::uint16_t	hint = 0;
				_memmap->read(_regs.pc + 1, (std::uint8_t*)&hint, sizeof(std::uint16_t));
				_memmap->write(hint, &_regs.a, sizeof(std::uint8_t));
				(void)holders;
				(void)op;
			})
			.cycles(8)
		.end()

		.begin()
			.mnemonic("ld (C) A")
			.bytecode(0xe2)
			.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
				_memmap->write(0xff00 | _regs.c, &_regs.a, sizeof(std::uint8_t));
				(void)holders;
				(void)op;
			})
			.cycles(8)
		.end()

		.begin()
			.mnemonic("ld A (C)")
			.bytecode(0xf2)
			.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
				_memmap->read(0xff00 | _regs.c, &_regs.a, sizeof(std::uint8_t));
				(void)holders;
				(void)op;
			})
			.cycles(8)
		.end()

		;


	builder.begin()
		.mnemonic("ld (%8) A")
		.bytecode(0xe0, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint8_t	addr;
			_memmap->read(_regs.pc + 1, &addr, sizeof(std::uint8_t));
			_memmap->write(addr + 0xff00, &_regs.a, sizeof(std::uint8_t));
			(void)holders;
			(void)op;
		})
		.cycles(12)
	.end();

	builder.begin()
		.mnemonic("ld A (%8)")
		.bytecode(0xf0, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint8_t	addr;
			_memmap->read(_regs.pc + 1, &addr, sizeof(std::uint8_t));
			_memmap->read(addr + 0xff00, &_regs.a, sizeof(std::uint8_t));
			(void)holders;
			(void)op;
		})
		.cycles(12)
	.end();

	start = 0x01;
	builder.begin()
		.mnemonic("ld $rr %16")
		.bytecode(bytecode_binder16_1, 3)
		.placeholder("rr", reg16_1_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			_memmap->read(_regs.pc + 1, holders[0].data, sizeof(std::uint16_t));
			(void)op;
		})
		.cycles(12)
	.end();
	builder.begin()
		.mnemonic("ld SP HL")
		.bytecode(0xF9)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			_regs.sp = _regs.hl;
			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end();

	start = 0xC5;
	builder.begin()
		.mnemonic("push $rr")
		.bytecode(bytecode_binder16_1)
		.placeholder("rr", reg16_2_placeholders)
		.call([&] (opcode::Placeholders const  &holders, opcode::Opcode const & op) {
			_memmap->write(_regs.sp, holders[0].data, 1);
			_regs.sp--;
			_memmap->write(_regs.sp, holders[0].data + 1, 1);
			_regs.sp--;

			(void)op;
		})
		.cycles(16)
	.end();

	start = 0xC1;
	builder.begin()
		.mnemonic("pop $rr")
		.bytecode(bytecode_binder16_1)
		.placeholder("rr", reg16_2_placeholders)
		.call([&] (opcode::Placeholders const  &holders, opcode::Opcode const & op) {
			_regs.sp++;
			_memmap->read(_regs.sp, holders[0].data + 1, 1);
			_regs.sp++;
			_memmap->read(_regs.sp, holders[0].data, 1);
			(void)op;
		})
		.cycles(12)
	.end();

	builder.begin()
		.mnemonic("ldi (HL), A")
		.bytecode(0x22)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			_memmap->write(_regs.hl, &_regs.a, sizeof(std::uint8_t));
			_regs.hl++;
			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end()
	.begin()
		.mnemonic("ldi A, (HL)")
		.bytecode(0x2A)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint8_t	hl;

			_memmap->read(_regs.hl, &hl, sizeof(hl));
			_regs.a = hl;
			_regs.hl++;
			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end()
	.begin()
		.mnemonic("ldd (HL), A")
		.bytecode(0x32)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			_memmap->write(_regs.hl, &_regs.a, sizeof(std::uint8_t));
			_regs.hl--;
			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end()
	.begin()
		.mnemonic("ldd A, (HL)")
		.bytecode(0x3A)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint8_t	hl;

			_memmap->read(_regs.hl, &hl, sizeof(hl));
			_regs.a = hl;
			_regs.hl--;
			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end()
	;

	/***************************************/

	start = 0x80;
	builder.begin()
		.mnemonic("add $r")
		.bytecode(bytecode_binder8_inc)
		.placeholder("r", reg8_7_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			_regs.a += (*holders[0].data);
			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, _regs.a >= 0x10);;
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, (_regs.a < *holders[0].data));
			(void)op;
		})
		.cycles(4)
	.end()
	.begin()
		.mnemonic("add (HL)")
		.bytecode(0x86)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t	hl = 0;
			_memmap->read(_regs.hl, &hl, sizeof(hl));
			_regs.a += hl;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, _regs.a >= 0x10);;
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, (_regs.a < hl));

			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end()
	.begin()
		.mnemonic("add %8")
		.bytecode(0xc6, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t	bcode = 0;
			_memmap->read(_regs.pc + 1, &bcode, sizeof(bcode));
			_regs.a += bcode;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, _regs.a >= 0x10);;
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, (_regs.a < bcode));

			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end();


	start = 0x88;
	builder.begin()
		.mnemonic("adc $r")
		.bytecode(bytecode_binder8_inc)
		.placeholder("r", reg8_7_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			_regs.a += (*holders[0].data) + memunit::bitval<CpuRegisters::carry>(_regs.f);;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, _regs.a >= 0x10);;
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, (_regs.a < *holders[0].data));
			(void)op;
		})
		.cycles(4)
	.end()
	.begin()
		.mnemonic("adc (HL)")
		.bytecode(0x8e)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t	hl = 0;
			_memmap->read(_regs.hl, &hl, sizeof(hl));
			_regs.a += hl + memunit::bitval<CpuRegisters::carry>(_regs.f);;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, _regs.a >= 0x10);;
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, (_regs.a < hl));

			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end()
	.begin()
		.mnemonic("adc %8")
		.bytecode(0xce, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t	bcode = 0;
			_memmap->read(_regs.pc + 1, &bcode, sizeof(bcode));
			_regs.a += bcode + memunit::bitval<CpuRegisters::carry>(_regs.f);;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, _regs.a >= 0x10);;
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, (_regs.a < bcode));

			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end();

	start = 0x90;
	builder.begin()
		.mnemonic("sub $r")
		.bytecode(bytecode_binder8_inc)
		.placeholder("r", reg8_7_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			_regs.a -= (*holders[0].data);

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, _regs.a >= 0x10);;
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, _regs.a > *holders[0].data);;
			(void)op;
		})
		.cycles(4)
	.end()
	.begin()
		.mnemonic("sub (HL)")
		.bytecode(0x96)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t	hl = 0;
			_memmap->read(_regs.hl, &hl, sizeof(hl));
			_regs.a -= hl;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, _regs.a >= 0x10);;
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, _regs.a > hl);

			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end()
	.begin()
		.mnemonic("sub %8")
		.bytecode(0xd6, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t	bcode = 0;
			_memmap->read(_regs.pc + 1, &bcode, sizeof(bcode));
			_regs.a -= bcode;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 1);;
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, _regs.a >= 0x10);;
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, _regs.a > bcode);;

			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end();

	start = 0x98;
	builder.begin()
		.mnemonic("sbc $r")
		.bytecode(bytecode_binder8_inc)
		.placeholder("r", reg8_7_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			_regs.a -= (*holders[0].data) - memunit::bitval<CpuRegisters::carry>(_regs.f);;
			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 1);;
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, _regs.a >= 0x10);;
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, _regs.a > *holders[0].data);;
			(void)op;
		})
		.cycles(4)
	.end()
	.begin()
		.mnemonic("sbc (HL)")
		.bytecode(0x9e)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t	hl = 0;
			_memmap->read(_regs.hl, &hl, sizeof(hl));
			_regs.a -= hl - memunit::bitval<CpuRegisters::carry>(_regs.f);;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 1);;
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, _regs.a >= 0x10);;
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, _regs.a > hl);;

			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end()
	.begin()
		.mnemonic("sbc %8")
		.bytecode(0xde, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t	bcode = 0;
			_memmap->read(_regs.pc + 1, &bcode, sizeof(bcode));
			_regs.a -= bcode - memunit::bitval<CpuRegisters::carry>(_regs.f);;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 1);;
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, _regs.a >= 0x10);;
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, _regs.a > bcode);;

			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end();


	start = 0x04;
	builder.begin()
		.mnemonic("inc $r")
		.bytecode(bytecode_binder8_1)
		.placeholder("r", reg8_7_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			(*holders[0].data)++;
			memunit::bitval_set<CpuRegisters::zero>(_regs.f, (*holders[0].data) == 0);;
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, (*holders[0].data) >= 0x10);
			(void)op;
		})
		.cycles(4)
	.end()
	.begin()
		.mnemonic("inc (HL)")
		.bytecode(0x34)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t	hl = 0;
			_memmap->read(_regs.hl, &hl, sizeof(hl));
			hl++;
			_memmap->write(_regs.hl, &hl, sizeof(hl));

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, (*holders[0].data) == 0);;
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, hl >= 0x10);

			(void)holders;
			(void)op;
		})
		.cycles(12)
	.end();
	start = 0x03;
	builder.begin()
		.mnemonic("inc $rr")
		.placeholder("rr", reg16_1_placeholders)
		.bytecode(bytecode_binder16_1, 1)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint16_t* tmp = (std::uint16_t*)holders[0].data;
			*tmp = (*tmp) + 1;
			(void)op;
		})
		.cycles(8)
	.end();


	start = 0x05;
	builder.begin()
		.mnemonic("dec $r")
		.bytecode(bytecode_binder7_1)
		.placeholder("r", reg8_7_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t		val = *(holders[0].data);
			val--;

			*holders[0].data = val;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, val == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 1);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, val >= 0x10);
			(void)op;
		})
		.cycles(4)
	.end()
	.begin()
		.mnemonic("dec (HL)")
		.bytecode(0x35)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t	hl = 0;
			_memmap->read(_regs.hl, &hl, sizeof(hl));
			hl--;
			_memmap->write(_regs.hl, &hl, sizeof(hl));

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, (*holders[0].data) == 0);;
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 1);;
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, hl >= 0x10);

			(void)holders;
			(void)op;
		})
		.cycles(12)
	.end();
	start = 0x0b;
	builder.begin()
		.mnemonic("dec $rr")
		.placeholder("rr", reg16_1_placeholders)
		.bytecode(bytecode_binder16_1, 1)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint16_t*	tmp = (std::uint16_t*)holders[0].data;
			(*tmp) = (*tmp) - 1;
			(void)op;
		})
		.cycles(8)
	.end();
	/***************************************/


	start = 0xA8;
	builder.begin()
		.mnemonic("xor $r")
		.bytecode(bytecode_binder8_inc)
		.placeholder("r", reg8_7_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			_regs.a = _regs.a ^ (*holders[0].data);
			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, 0);

			(void)op;
		})
		.cycles(4)
	.end()
	.begin()
		.mnemonic("xor (HL)")
		.bytecode(0xAE)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t	hl = 0;
			_memmap->read(_regs.hl, &hl, sizeof(hl));
			_regs.a = ~hl;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, 0);

			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end()
	.begin()
		.mnemonic("xor %8")
		.bytecode(0xEE, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t	bcode = 0;
			_memmap->read(_regs.pc + 1, &bcode, sizeof(bcode));
			_regs.a = ~bcode;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, 0);

			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end();

	start = 0xA0;
	builder.begin()
		.mnemonic("and $r")
		.bytecode(bytecode_binder8_inc)
		.placeholder("r", reg8_7_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			_regs.a &= (*holders[0].data);
			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, 1);
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, 0);

			(void)op;
		})
		.cycles(4)
	.end()
	.begin()
		.mnemonic("and (HL)")
		.bytecode(0xA6)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t	hl = 0;
			_memmap->read(_regs.hl, &hl, sizeof(hl));
			_regs.a &= hl;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, 1);
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, 0);

			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end()
	.begin()
		.mnemonic("and %8")
		.bytecode(0xe6, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t	bcode = 0;
			_memmap->read(_regs.pc + 1, &bcode, sizeof(bcode));
			_regs.a &= bcode;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, 1);
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, 0);

			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end();


	start = 0xB0;
	builder.begin()
		.mnemonic("or $r")
		.bytecode(bytecode_binder8_inc)
		.placeholder("r", reg8_7_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			_regs.a |= (*holders[0].data);
			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, 0);

			(void)op;
		})
		.cycles(4)
	.end()
	.begin()
		.mnemonic("or (HL)")
		.bytecode(0xB6)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t	hl = 0;
			_memmap->read(_regs.hl, &hl, sizeof(hl));
			_regs.a |= hl;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, 0);

			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end()
	.begin()
		.mnemonic("or %8")
		.bytecode(0xf6, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			std::uint8_t	bcode = 0;
			_memmap->read(_regs.pc + 1, &bcode, sizeof(bcode));
			_regs.a |= bcode;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, _regs.a == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, 0);

			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end();


	start = 0xB8;
	builder.begin()
		.mnemonic("cp $r")
		.bytecode(bytecode_binder8_inc)
		.placeholder("r", reg8_7_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			int test = _regs.a - (*holders[0].data);

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, test == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 1);;
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, test >= 0x10);
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, (*holders[0].data) > _regs.a);

			(void)op;
		})
		.cycles(4)
	.end()
	.begin()
		.mnemonic("cp (HL)")
		.bytecode(0xbe)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			int				test = 0;
			std::uint8_t	hl = 0;
			_memmap->read(_regs.hl, &hl, sizeof(hl));
			test = _regs.a - hl;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, test == 0);;
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 1);;
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, test >= 0x10);
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, hl > _regs.a);

			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end()
	.begin()
		.mnemonic("cp %8")
		.bytecode(0xfe, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			int				test = 0;
			std::uint8_t	bcode = 0;
			_memmap->read(_regs.pc + 1, &bcode, sizeof(bcode));
			test = _regs.a - bcode;

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, test == 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 1);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, test >= 0x1f);
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, bcode > _regs.a);

			(void)holders;
			(void)op;
		})
		.cycles(8)
	.end();


	builder.begin()
		.mnemonic("cpl")
		.bytecode(0x2f, 1)
		.placeholder("r", reg8_7_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			_regs.a = ~_regs.a;
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, 1);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 1);;

			(void)holders;
			(void)op;
		})
		.cycles(4)
	.end();

	builder.begin()
		.mnemonic("ccf")
		.bytecode(0x3f, 1)
		.placeholder("r", reg8_7_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			_regs.a = ~_regs.a;
			memunit::bitval_set<CpuRegisters::carry>(_regs.f,
				memunit::bitval<CpuRegisters::carry>(_regs.f)
			);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);

			(void)holders;
			(void)op;
		})
		.cycles(4)
	.end();

	/***************************************/

	start = 0xCB40;
	builder.begin()
		.mnemonic("bit $n $r")
		.bytecode(bytecode_binder8_prefix_cb, 2)
		.placeholder("n", bytes_placeholders)
		.placeholder("r", reg8_7_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			memunit::bitval_set<CpuRegisters::zero>(_regs.f, ((*holders[1].data) & (1 << (*holders[0].data))) != 0);;
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, 1);

			(void)op;
		})
		.cycles(8)
	.end();

	start = 0xCB46;
	builder.begin()
		.mnemonic("bit $n (HL)")
		.bytecode(bytecode_binder8_2_prefix_cb, 2)
		.placeholder("n", bytes_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint8_t hl = 0;
			_memmap->read(_regs.hl, &hl, sizeof(hl));

			memunit::bitval_set<CpuRegisters::zero>(_regs.f, (hl & (1 << (*holders[0].data))) != 0);
			memunit::bitval_set<CpuRegisters::add>(_regs.f, 0);
			memunit::bitval_set<CpuRegisters::half_carry>(_regs.f, 1);

			(void)op;
		})
		.cycles(16)
	.end();

	start = 0xCBC0;
	builder.begin()
		.mnemonic("set $n $r")
		.bytecode(bytecode_binder8_prefix_cb, 2)
		.placeholder("n", bytes_placeholders)
		.placeholder("r", reg8_7_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			*holders[1].data |= 1 << (*holders[0].data);

			(void)op;
		})
		.cycles(8)
	.end();

	start = 0xCBC6;
	builder.begin()
		.mnemonic("set $n (HL)")
		.bytecode(bytecode_binder8_2_prefix_cb, 2)
		.placeholder("n", bytes_placeholders)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint8_t hl = 0;
			_memmap->read(_regs.hl, &hl, sizeof(hl));
			hl |= 1 << (*holders[0].data);
			_memmap->write(_regs.hl, &hl, sizeof(hl));

			(void)op;
		})
		.cycles(16)
	.end();


	/********************************/

	builder.begin()
		.mnemonic("rlca")
		.bytecode(0x07)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, (_regs.a >> 7) & 0xff);
			_regs.a = _regs.a << 1;
			(void)holders;(void)op;
		})
		.cycles(4)
	.end()
	.begin()
		.mnemonic("rla")
		.bytecode(0x17)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint8_t tmpc = memunit::bitval<CpuRegisters::carry>(_regs.f);
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, (_regs.a >> 7) & 0xff);
			_regs.a = (_regs.a << 1) | (tmpc & 0x01);
			(void)holders;(void)op;
		})
		.cycles(4)
	.end()
	.begin()
		.mnemonic("rrca")
		.bytecode(0x0f)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, _regs.a & 0x01);
			_regs.a = (_regs.a >> 1);
			(void)holders;(void)op;
		})
		.cycles(4)
	.end()
	.begin()
		.mnemonic("rra")
		.bytecode(0x1f)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint8_t tmpc = memunit::bitval<CpuRegisters::carry>(_regs.f);
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, _regs.a & 0x01);
			_regs.a = (_regs.a >> 1) | (tmpc & 0x80);
			(void)holders;(void)op;
		})
		.cycles(4)
	.end();

	start = 0xcb00;
	builder.begin()
		.mnemonic("rlc $r")
		.placeholder("r", reg8_7_placeholders)
		.bytecode(bytecode_binder8_inc, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, (*holders[0].data) & 0x80);
			(*holders[0].data) = ((*holders[0].data) << 1);

			(void)op;
		})
		.cycles(8)
	.end()
	.begin()
		.mnemonic("rlc (HL)")
		.bytecode(0xcb06, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint8_t	hl;

			_memmap->read(_regs.hl, (std::uint8_t*)&hl, sizeof(hl));
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, hl & 0x80);
			hl = hl << 1;
			_memmap->write(_regs.hl, (std::uint8_t*)&hl, sizeof(hl));
			(void)holders;(void)op;
		})
		.cycles(16)
	.end();

	start = 0xcb10;
	builder.begin()
		.mnemonic("rl $r")
		.placeholder("r", reg8_7_placeholders)
		.bytecode(bytecode_binder8_inc, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint8_t tmpc = memunit::bitval<CpuRegisters::carry>(_regs.f);
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, (*holders[0].data) & 0x80);
			(*holders[0].data) = ((*holders[0].data) << 1) | (tmpc & 0x01);

			(void)op;
		})
		.cycles(8)
	.end()
	.begin()
		.mnemonic("rl (HL)")
		.bytecode(0xcb16, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint8_t	hl;

			_memmap->read(_regs.hl, (std::uint8_t*)&hl, sizeof(hl));
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, hl & 0x80);
			hl = hl << 1;
			_memmap->write(_regs.hl, (std::uint8_t*)&hl, sizeof(hl));
			(void)holders;(void)op;
		})
		.cycles(16)
	.end();

	start = 0xcb08;
	builder.begin()
		.mnemonic("rrc $r")
		.placeholder("r", reg8_7_placeholders)
		.bytecode(bytecode_binder8_inc, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, (*holders[0].data) & 0x01);
			(*holders[0].data) = ((*holders[0].data) >> 1);

			(void)op;
		})
		.cycles(8)
	.end()
	.begin()
		.mnemonic("rrc (HL)")
		.bytecode(0xcb0e, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint8_t	hl;

			_memmap->read(_regs.hl, (std::uint8_t*)&hl, sizeof(hl));
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, hl & 0x01);
			hl = hl >> 1;
			_memmap->write(_regs.hl, (std::uint8_t*)&hl, sizeof(hl));
			(void)holders;(void)op;
		})
		.cycles(16)
	.end();

	start = 0xcb18;
	builder.begin()
		.mnemonic("rr $r")
		.placeholder("r", reg8_7_placeholders)
		.bytecode(bytecode_binder8_inc, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint8_t	tmpc = memunit::bitval<CpuRegisters::carry>(_regs.f);
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, (*holders[0].data) & 0x01);
			(*holders[0].data) = ((*holders[0].data) >> 1) | (tmpc << 7);

			(void)op;
		})
		.cycles(8)
	.end()
	.begin()
		.mnemonic("rr (HL)")
		.bytecode(0xcb1e, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint8_t	hl;
			std::uint8_t	tmpc = memunit::bitval<CpuRegisters::carry>(_regs.f);

			_memmap->read(_regs.hl, (std::uint8_t*)&hl, sizeof(hl));
			memunit::bitval_set<CpuRegisters::carry>(_regs.f, hl & 0x01);
			hl = hl >> 1 | (tmpc << 7);
			_memmap->write(_regs.hl, (std::uint8_t*)&hl, sizeof(hl));
			(void)holders;(void)op;
		})
		.cycles(16)
	.end();

	/********************************/

	builder.begin()
		.mnemonic("jp %16")
		.bytecode(0xc3, 3, false)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			_memmap->read(_regs.pc + 1, (std::uint8_t*)&_regs.pc, sizeof(std::uint16_t));
			(void)holders;(void)op;
		})
		.cycles(16)
	.end();

	builder.begin()
		.mnemonic("jp HL")
		.bytecode(0xe9, 1, false)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			_regs.pc = _regs.hl;
			(void)holders;(void)op;
		})
		.cycles(4)
	.end();

	start = 0xC2;
	builder.begin()
		.mnemonic("jp $cc %16")
		.placeholder("cc", cond_placeholders)
		.bytecode(bytecode_binder_cond, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			bool	can_jump = false;

			if (holders[0].name == "NZ") can_jump = (memunit::bitval<CpuRegisters::zero>(_regs.f));
			else if (holders[0].name == "NC") can_jump = (memunit::bitval<CpuRegisters::carry>(_regs.f) == 0);
			else if (holders[0].name == "Z") can_jump = (memunit::bitval<CpuRegisters::zero>(_regs.f) == 1);
			else if (holders[0].name == "C") can_jump = (memunit::bitval<CpuRegisters::carry>(_regs.f) == 1);

			if (can_jump) {
				_memmap->read(_regs.pc + 1, (std::uint8_t*)&_regs.pc, sizeof(std::uint16_t));
			} else {
			}
			(void)holders;(void)op;
		})
		.cycles(16)
	.end();

	builder.begin()
		.mnemonic("jr %8")
		.bytecode(0x18, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			signed char rel = 0;

			_memmap->read(_regs.pc + 1, (std::uint8_t*)&rel, sizeof(rel));
			_regs.pc += op.bytecodeLength();
			_regs.pc += rel;

			(void)holders;(void)op;
		})
		.cycles(16)
	.end();

	start = 0x20;
	builder.begin()
		.mnemonic("jr $cc %8")
		.placeholder("cc", cond_placeholders)
		.bytecode(bytecode_binder_cond, 2)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			bool	can_jump = false;

			if (holders[0].name == "nz") can_jump = (memunit::bitval<CpuRegisters::zero>(_regs.f) == 0);
			else if (holders[0].name == "nc") can_jump = (memunit::bitval<CpuRegisters::carry>(_regs.f) == 0);
			else if (holders[0].name == "z") can_jump = (memunit::bitval<CpuRegisters::zero>(_regs.f) == 1);
			else if (holders[0].name == "c") can_jump = (memunit::bitval<CpuRegisters::carry>(_regs.f) == 1);

			//std::cout << "Jump test: " << holders[0].name << std::endl;
			if (can_jump) {
				signed char rel = 0;
				_memmap->read(_regs.pc + 1, (std::uint8_t*)&rel, sizeof(rel));
			//	std::cout << "JUMPING OF " << std::dec << ((int)rel) << std::endl;
				_regs.pc += op.bytecodeLength();
				_regs.pc += rel;
			}
			(void)holders;(void)op;
		})
		.cycles(16)
	.end();

	start = 0xc4;
	builder.begin()
		.mnemonic("call $cc %16")
		.placeholder("cc", cond_placeholders)
		.bytecode(bytecode_binder_cond, 3)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			bool	can_call = false;

			if (holders[0].name == "NZ") can_call = (memunit::bitval<CpuRegisters::zero>(_regs.f));
			else if (holders[0].name == "NC") can_call = (memunit::bitval<CpuRegisters::carry>(_regs.f) == 0);
			else if (holders[0].name == "Z") can_call = (memunit::bitval<CpuRegisters::zero>(_regs.f) == 1);
			else if (holders[0].name == "C") can_call = (memunit::bitval<CpuRegisters::carry>(_regs.f) == 1);

			if (can_call) {
				std::uint16_t addr = 0;
				_memmap->read(_regs.pc + 1, (std::uint8_t*)&addr, sizeof(addr));

				_memmap->write(_regs.sp, (std::uint8_t*)&_regs.pc, 1);
				_regs.sp--;
				_memmap->write(_regs.sp, ((std::uint8_t*)&_regs.pc) + 1, 1);
				_regs.sp--;

				_regs.pc = addr;
			}
			(void)holders;(void)op;
		})
		.cycles(24)
	.end();
	builder.begin()
		.mnemonic("call %16")
		.bytecode(0xcd, 3)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint16_t addr = 0;
			_memmap->read(_regs.pc + 1, (std::uint8_t*)&addr, sizeof(addr));

			_memmap->write(_regs.sp, (std::uint8_t*)&_regs.pc, 1);
			_regs.sp--;
			_memmap->write(_regs.sp, ((std::uint8_t*)&_regs.pc) + 1, 1);
			_regs.sp--;

			_regs.pc = addr;
			(void)holders;(void)op;
		})
		.cycles(24)
	.end();

	start = 0xc4;
	builder.begin()
		.mnemonic("ret $cc")
		.placeholder("cc", cond_placeholders)
		.bytecode(bytecode_binder_cond, 1)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			bool	can_ret = false;

			if (holders[0].name == "NZ") can_ret = (memunit::bitval<CpuRegisters::zero>(_regs.f));
			else if (holders[0].name == "NC") can_ret = (memunit::bitval<CpuRegisters::carry>(_regs.f) == 0);
			else if (holders[0].name == "Z") can_ret = (memunit::bitval<CpuRegisters::zero>(_regs.f) == 1);
			else if (holders[0].name == "C") can_ret = (memunit::bitval<CpuRegisters::carry>(_regs.f) == 1);

			if (can_ret) {
				_regs.sp++;
				_memmap->read(_regs.sp, ((std::uint8_t*)&_regs.pc) + 1, 1);
				_regs.sp++;
				_memmap->read(_regs.sp, (std::uint8_t*)&_regs.pc, 1);

				this->fetchNext();
			}
			(void)holders;(void)op;
		})
		.cycles(20)
	.end();
	builder.begin()
		.mnemonic("ret")
		.bytecode(0xc9)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {

			_regs.sp++;
			_memmap->read(_regs.sp, ((std::uint8_t*)&_regs.pc) + 1, 1);
			_regs.sp++;
			_memmap->read(_regs.sp, (std::uint8_t*)&_regs.pc, 1);

			this->fetchNext();
//			_regs.pc += op.bytecodeLength();

			(void)holders;
			(void)op;
		})
		.cycles(16)
	.end();
	builder.begin()
		.mnemonic("reti")
		.bytecode(0xd9)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			_memmap->read(_regs.sp, (std::uint8_t*)&_regs.pc, sizeof(_regs.pc));
			_regs.sp += 2;
			_regs.ime = true;
			(void)holders;(void)op;
		})
		.cycles(16)
	.end();
	builder.begin()
		.mnemonic("rst %8")
		.bytecode(0xd9)
		.call([&] (opcode::Placeholders const &holders, opcode::Opcode const & op) {
			std::uint8_t addr = 0;
			_memmap->read(_regs.pc + 1, (std::uint8_t*)&addr, sizeof(addr));
			_regs.sp -= 2;
			_memmap->write(_regs.sp, (std::uint8_t*)&_regs.pc, sizeof(_regs.pc));
			_regs.pc = addr;
			(void)holders;(void)op;
		})
		.cycles(16)
	.end();
}

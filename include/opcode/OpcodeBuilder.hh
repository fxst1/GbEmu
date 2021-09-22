#ifndef OPCODE_BUILDER_HH
# define OPCODE_BUILDER_HH
# include <string>
# include <cstdint>

# include <map>
# include <set>
# include <vector>

# include <functional>

# include "memunit/MemUnit.hh"

namespace	opcode {

	typedef std::uint32_t	bytecode_t;
	typedef std::size_t		bytecode_size_t;

	struct	Placeholder {
		public:
			std::string		name;
			std::uint8_t*	data;
	};
	using	Placeholders = std::vector<opcode::Placeholder>;

	class	Opcode {

		public:

			virtual ~Opcode(void) {}

			virtual void	operator()(void) const = 0;

			class	Comparator {
				public:
					virtual bool	operator()(Opcode const *a, Opcode const *b) const;
			};

			virtual bool				isFixedBytecodeLength(void) const {
				return (true);
			}

			virtual bytecode_t			bytecode(void) const = 0;
			virtual bytecode_size_t		bytecodeLength(void) const = 0;
			virtual std::string			mnemonic(void) const = 0;
			virtual std::string			mnemonic(memunit::Memory &m, std::uintptr_t start) const = 0;
			virtual int					cycles(void) const = 0;
	};

	using InstrSet = std::set<opcode::Opcode*, opcode::Opcode::Comparator>;

	class	InstructionSet {

		public:
			InstructionSet(void);
			InstructionSet(InstrSet const & set);
			~InstructionSet(void);

			opcode::Opcode*		find(opcode::bytecode_t bcode) const;
			InstrSet::iterator	begin(void);
			InstrSet::iterator	end(void);

			void 				dissas(std::uint16_t pc, memunit::MemoryMap& memmap, std::ostream &out) const;

			void				insert(opcode::Opcode*);

			void				dump(void) const;

		private:

			class	OpcodeHint : public Opcode {

				private:

					opcode::bytecode_t		_bytecode;

				public:

					OpcodeHint(opcode::bytecode_t bcode);
					~OpcodeHint(void);

					void						operator()(void) const;

					opcode::bytecode_t			bytecode(void) const;
					opcode::bytecode_size_t		bytecodeLength(void) const;
					std::string					mnemonic(void) const;
					std::string					mnemonic(memunit::Memory &m, std::uintptr_t start) const;

					Placeholders				placeholders(void) const;
					int							cycles(void) const;
			};

			InstrSet	_opcodes;
	};


	class	Builder {
		public:

			typedef std::function<void(opcode::Placeholders const &, opcode::Opcode const & op)> Executor;
			typedef std::function<opcode::bytecode_t(opcode::Placeholders const &)> BytecodeGen;

			Builder(opcode::InstructionSet& instrset);
			~Builder(void);

			opcode::Builder&	begin(void);
			opcode::Builder&	end(void);

			opcode::Builder&	mnemonic(std::string const &mne);

			opcode::Builder&	placeholder(std::string const &macro, opcode::Placeholder const &holder);
			opcode::Builder&	placeholder(std::string const &macro, opcode::Placeholders const &holders);
			opcode::Builder&	placeholder(std::string const &macro, std::map<std::string, std::uint8_t*>);

			opcode::Builder&	bytecode(opcode::bytecode_t bcode, opcode::bytecode_size_t bcode_len = 1, bool fixed_len = true);
			opcode::Builder&	bytecode(opcode::Builder::BytecodeGen const &bcodegen, opcode::bytecode_size_t bcode_len = 1, bool fixed_len = true);
			opcode::Builder&	call(opcode::Builder::Executor const &executor);

			opcode::Builder&	cycles(unsigned int ncycles);

			opcode::InstructionSet	getOpcodes(void) const;

		private:

			opcode::InstructionSet&			_opcodes;
			bool							_exec_callable;
			opcode::Builder::Executor		_exec_func;

			bool							_fixed_len;
			bool							_bcode_callable;
			opcode::Builder::BytecodeGen	_bcode_func;
			opcode::bytecode_t				_bcode_val;
			opcode::bytecode_size_t			_bcode_len;

			std::map<std::string, opcode::Placeholders>				_holdermap;
			opcode::InstrSet										_current;
			std::string												_mnemonic;
			int														_ncycles;

			opcode::Builder&	build(opcode::Placeholders const &holders, std::size_t pos);

			class	DraftOpcode : public Opcode {

				protected:

					bool				_fixed_len;
					bytecode_t			_bytecode;
					bytecode_size_t		_bytecode_size;
					int					_cycles;
					std::string			_mnemonic;
					Placeholders		_holders;
					Executor			_func;

				public:

					DraftOpcode(std::string const &mne, bytecode_t bcode, bytecode_size_t bcode_len, bool fixed_len, int cycles, Placeholders const &holders, Executor const &executor);
					~DraftOpcode(void);

					virtual void		operator()(void) const;

					bool				isFixedBytecodeLength(void) const;
					bytecode_t			bytecode(void) const;
					bytecode_size_t		bytecodeLength(void) const;
					std::string			mnemonic(void) const;
					std::string			mnemonic(memunit::Memory &m, std::uintptr_t start) const;
					Placeholders		placeholders(void) const;
					int					cycles(void) const;
			};
	};

};

#endif

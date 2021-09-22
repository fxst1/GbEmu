#ifndef ASSEMBLER_HH
# define ASSEMBLER_HH
# include <string>
# include <cstdint>
# include <fstream>
# include <istream>
# include "opcode/OpcodeBuilder.hh"

namespace	opcode {

	class	FileWriter {
		public:
			FileWriter(opcode::InstructionSet const &set);
			~FileWriter(void);

			void		decode(std::ostream &output, std::istream &input);
			void		decode(std::ostream &output, std::uint8_t *src, std::size_t len);

		private:
			opcode::InstructionSet	_opcodes;
	};

};

#endif

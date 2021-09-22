#pragma once
# include <cstdint>
# include <iostream>

namespace	memunit {

	template<std::size_t at_bit, std::size_t n_bit = 1>
	std::uint8_t	bitval(std::uint8_t const & v) {
		return ((v >> at_bit) & ~(1 << n_bit));
	}
	template<std::size_t at_bit>
	std::uint8_t	bitval_set(std::uint8_t & v, bool b) {
		v = (b ? v | (1 << at_bit) : v & ~(1 << at_bit));
		return (v);
	}

	class	BitOrder {
		public:
			BitOrder(void) {}
			virtual ~BitOrder(void) {}

			template<typename IntType>
			static IntType	littleEndian(std::uint8_t* buf) {
				if (sizeof(IntType) == sizeof(std::uint16_t)) {
					//std::cout << "Conv 16 BIT" << std::endl;
					std::uint16_t	a = *(std::uint16_t*)buf;
					a = ((a & 0xff00) >> 8) | ((a & 0x00ff) << 8);
					*(std::uint16_t*)buf = a;
					return (a);
				} else if (sizeof(IntType) == sizeof(std::uint8_t)) {
//					std::cout << "Conv 8 BIT" << std::endl;
				}
				return (*buf);
			}
	};
};

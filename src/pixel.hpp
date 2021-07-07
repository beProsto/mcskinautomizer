#include <stdint.h>

class Pixel {
public:
	uint32_t m_Data;

public:
	Pixel(uint32_t _p = 0) {
		this->m_Data = _p;
	}
	Pixel(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) {
		uint32_t r = _r;
		r <<= 24;

		uint32_t g = _g;
		g <<= 16;

		uint32_t b = _b;
		b <<= 8;

		uint32_t a = _a;

		this->m_Data = r | g | b | a;
	}

	uint8_t getRed() {
		return (this->m_Data >> 24 & 0xff)*1;	
	}
	uint32_t getG() {
		return this->m_Data >> 16 & 0xff;	
	}
	uint32_t getB() {
		return this->m_Data >> 8 & 0xff;	
	}
	uint32_t getA() {
		return this->m_Data & 0xff;	
	}

	uint32_t getData() {
		return this->m_Data;
	}
};
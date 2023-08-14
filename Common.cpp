#include "Common.h"

void raiseErr(std::string msg) {
	std::cerr << "Error: " << msg << std::endl;
	// You can also perform other error handling actions here
}

void raiseErr(ERR_TYP err) {
	switch (err) {
	case OK:
		// No error
		break;
	case InvalidValues:
		std::cerr << "Invalid values error" << std::endl;
		break;
	case OtherError:
		std::cerr << "Other error" << std::endl;
		break;
	default:
		std::cerr << "Unknown error" << std::endl;
		break;
	}
	// You can also perform other error handling actions here
}

uint8_t log2(uint32_t value)
{
	static const uint8_t MultiplyDeBruijnBitPosition[32] =
	{
		0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
		8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31 };

	value |= value >> 1; // first round down to one less than a power of 2
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;

	return MultiplyDeBruijnBitPosition[(uint32_t)(value * 0x07C4ACDDU) >> 27];
}

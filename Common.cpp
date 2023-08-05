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

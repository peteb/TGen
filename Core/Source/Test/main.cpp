#include <iostream>
#include <iomanip>
#include "tgen_core.h"

int main(int argc, char ** const argv) {
	std::cout << "TGen Core (debug binary: " << std::boolalpha << TGen::isCoreDebug() << ")" << std::endl;
	
	// 1.
	if (TGen::TGenDataType<float>().type == TGen::TypeFloat)
		std::cout << "1. OK" << std::endl;
	
	// 2.
	std::cout << "2. " << sizeof(float) << " == " << TGen::FormatTypeSize(TGen::TGenDataType<float>().type) << std::endl;
	
	// 3.
	#ifdef _PLATFORM_WINDOWS
	std::cout << "3. Windows" << std::endl;
	#elif defined(_PLATFORM_OSX)
	std::cout << "3. OS X" << std::endl;
	#else
	std::cout << "3. Unknown" << std::endl;
	#endif
	
	// 4.
	try {
		throw TGen::RuntimeException("Core", "working");
	}
	catch (const std::exception & e) {
		std::cout << "4. exception: " << e.what() << std::endl;
	}
	
	return EXIT_SUCCESS;
}

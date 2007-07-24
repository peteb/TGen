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
	std::cout << "   " << sizeof(long long) << " == " << TGen::FormatTypeSize(TGen::TGenDataType<long long>().type) << std::endl;
	std::cout << "   " << sizeof(int) << " == " << TGen::FormatTypeSize(TGen::TGenDataType<int>().type) << std::endl;
	std::cout << "   " << sizeof(short) << " == " << TGen::FormatTypeSize(TGen::TGenDataType<short>().type) << std::endl;
	std::cout << "   " << sizeof(char) << " == " << TGen::FormatTypeSize(TGen::TGenDataType<char>().type) << std::endl;
	std::cout << "   " << sizeof(bool) << " == " << TGen::FormatTypeSize(TGen::TGenDataType<bool>().type) << std::endl;

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
	
	#ifdef _PLATFORM_WINDOWS
	int hej;
	std::cin >> hej;
	#endif
	
	// 5.
	std::cout << "5. lexical_cast: " << TGen::lexical_cast<int>("12345") + TGen::lexical_cast<int>("54321") << " == " << 12345 + 54321 << std::endl;
	
	// 6.
	std::cout << "6. " << TGen::lexical_cast<bool>("false") << " == false, " << TGen::lexical_cast<bool>("true") << " == true, " << TGen::lexical_cast<std::string>(false) << " == false, " << TGen::lexical_cast<std::string>(true) << " == true" << std::endl;

	// 7.
	try {
		std::cout << TGen::lexical_cast<int>("hejsan") << std::endl;
	}
	catch (const std::exception & e) {
		std::cout << "7. bad_lexical_cast: " << e.what() << std::endl;
	}
	
	// 8.
	TGen::Time now = TGen::Time::Now();
	std::cout << "8. now: " << std::fixed << double(now) << " ";
	TGen::Sleep(TGen::Time(1.0));
	TGen::Time really = TGen::Time::Now();
	std::cout << "sleep diff: " << double(really - now) << std::endl;
	
	return EXIT_SUCCESS;
}

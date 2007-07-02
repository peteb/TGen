#include <iostream>
#include "tgen_graphics.h"

int main(int argc, char ** argv) {
	std::cout << "TGen Graphics (debug binary: " << std::boolalpha << TGen::isGraphicsDebug() << ")" << std::endl;
	
	// 1.
	std::cout << "1. Red: " << std::string(TGen::Color::Red) << std::endl;
	
	// 2.
	std::cout << "2. 640x480: " << std::string(TGen::Rectangle(640, 480)) << std::endl;
	
	return EXIT_SUCCESS;
}

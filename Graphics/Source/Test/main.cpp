#include <iostream>
#include "tgen_graphics.h"

int main(int argc, char ** argv) {
	std::cout << "TGen Graphics (debug binary: " << std::boolalpha << TGen::isGraphicsDebug() << ")" << std::endl;
	
	// 1.
	std::cout << "1. Red: " << std::string(TGen::Color::Red) << std::endl;
	
	// 2.
	std::cout << "2. 640x480: " << std::string(TGen::Rectangle(640, 480)) << std::endl;
		
	#ifdef _PLATFORM_WINDOWS
	int hej;
	std::cin >> hej;
	#endif

	return EXIT_SUCCESS;
}

// TODO: rendreraren ska komma ihåg staten och inte ändra den i ogl om det inte behövs
// TODO: render states ska kunna ha en override-state, så man kan sätta wireframe på front och back även fast materialet säger annat

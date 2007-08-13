#include <iostream>
#include "tgen_renderer.h"

int main(int argc, char ** argv) {
	std::cout << "TGen Renderer (debug binary: " << std::boolalpha << TGen::isRendererDebug() << ")" << std::endl;

	return EXIT_SUCCESS;
}

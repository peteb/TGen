#include <iostream>
#include <GLUT/GLUT.h>
#include "tgen_opengl.h"

int main(int argc, char ** argv) {
	std::cout << "TGen OpenGL (debug binary: " << std::boolalpha << TGen::isOpenGLDebug() << ")" << std::endl << std::endl;

	// We need a context...
	glutInit(&argc, argv);
	glutInitWindowSize(400, 300);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("TGen OpenGL");
	
	std::cout << "1. Creating renderer..." << std::endl;
	TGen::Renderer * renderer = new TGen::OpenGL::Renderer();
	if (!renderer) {
		std::cout << "   ... failed!" << std::endl;
		return EXIT_FAILURE;
	}
	else {
		std::cout << "   ...ok" << std::endl << std::endl;
	}
	
	std::cout << "2. Renderer capabilities: " << std::endl << std::string(renderer->getCaps()) << std::endl;
	
	delete renderer;
	
	#ifdef _PLATFORM_WINDOWS
	int hej;
	std::cin >> hej;
	#endif

	return EXIT_SUCCESS;
}

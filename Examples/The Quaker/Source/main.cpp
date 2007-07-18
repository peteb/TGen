#include <iostream>
#include <GLUT/GLUT.h>
#include <tgen_opengl.h>
#include "app.h"

extern App * gApp;

int main(int argc, char ** argv) {
	std::cout << "TGen 'The Quaker' demo (debug binary: " << std::boolalpha << TGen::isOpenGLDebug() << ")" << std::endl << std::endl;

	std::cout << "[glut]: initializing..." << std::endl;
	glutInit(&argc, argv);
	
	gApp = new App;
	std::cout << "======================== running ========================" << std::endl;
	
	while (gApp->isRunning()) {
		glutCheckLoop();
		gApp->Update();
	}
	
	std::cout << "===================== shutting down =====================" << std::endl;
	
	delete gApp;
	
	return EXIT_SUCCESS;
}

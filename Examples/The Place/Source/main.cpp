#include <iostream>
#include <tgen_opengl.h>
#include <GLUT/GLUT.h>

#include "app.h"

extern App * app;

int main(int argc, char ** argv) {
	std::cout << "TGen 'The Place' demo (debug binary: " << std::boolalpha << TGen::isOpenGLDebug() << ")" << std::endl << std::endl;

	glutInit(&argc, argv);
	
	app = new App;
	//glutMainLoop();
	std::cout << "======================== running ========================" << std::endl;
	
	while (app->isRunning()) {
		glutCheckLoop();
		app->Update();
	}
	
	std::cout << "===================== shutting down =====================" << std::endl;
	delete app;
	
	return EXIT_SUCCESS;
}

// TODO: MaterialParser -> MaterialLoader

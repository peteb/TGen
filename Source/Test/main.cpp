#include <iostream>
#include <sstream>
//#include <GL/glfw.h>
#include <GLUT/GLUT.h>
#include <iomanip>
#include "../tgen.h"
#include "../opengl.h"

using namespace TGen;



class TestApp {
public:
	static void Init(int argc, char ** argv) {
		glutInit(&argc, argv);
		
		glutInitWindowSize(windowSize.width, windowSize.height);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		glutCreateWindow("TGen test");
		glutDisplayFunc(&TestApp::Render);		
				
		renderer = new OpenGL::Renderer();
		renderer->setClearColor(Color::White);
	}
	
	static void Shutdown() {
		delete renderer;	
	}
	
	static void Render() {
		if (renderer) {
			renderer->Clear(ColorBuffer | DepthBuffer);
			renderer->setViewport(windowSize);
		
			glutSwapBuffers();
		}
	}
	
	static Rectangle windowSize;
	static Renderer * renderer;
};

Rectangle TestApp::windowSize;
Renderer * TestApp::renderer = NULL;

int main(int argc, char ** argv) {
	TestApp::windowSize = Rectangle(0.0f, 640, 480);
		
	std::cout << std::string(TestApp::windowSize) << std::endl;
	std::cout << std::string(Matrix3x3::Identity) << std::endl;
	
	TestApp::Init(argc, argv);
	glutMainLoop();
	TestApp::Shutdown();
	
	return EXIT_SUCCESS;
}

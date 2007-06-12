#include <iostream>
#include <sstream>
#include <GL/glfw.h>
#include <iomanip>
#include "../tgen.h"
#include "../opengl.h"

using namespace TGen;

int main() {
	int width = 640, height = 480, running, frames;
	double t, t0, fps;
	
	Rectangle windowSize(0.0f, 640, 480);
	
	std::cout << std::string(windowSize) << std::endl;
	std::cout << std::string(Matrix3x3::Identity) << std::endl;
	
	glfwInit();
	
	if (!glfwOpenWindow(int(windowSize.width), int(windowSize.height), 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwSwapInterval(1);
	
	running = GL_TRUE;
	frames = 0;
	t0 = glfwGetTime();
	
	TGen::Renderer * renderer = new TGen::OpenGL::Renderer();
	renderer->setClearColor(renderer->getClearColor());
	
	while (running) {
		t = glfwGetTime();
		
		if ((t - t0) > 1.0 || frames == 0) {
			fps = (double)frames / (t - t0);
			std::stringstream ss;
			ss << "TGen Test (" << std::fixed << std::setprecision(1) << fps << " FPS)";
			glfwSetWindowTitle(ss.str().c_str());

			t0 = t;
			frames = 0;
		}
		
		frames++;
		
		glfwGetWindowSize(&width, &height);
		height = height > 0 ? height : 1;
		renderer->Clear(TGen::ColorBuffer | TGen::DepthBuffer);
		
/*		// Set viewport
		glViewport( 0, 0, width, height );
		
		// Clear color buffer
		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT );
		
		// Select and setup the projection matrix
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective( 65.0f, (GLfloat)width/(GLfloat)height, 1.0f,
						100.0f );
		
		// Select and setup the modelview matrix
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		gluLookAt( 0.0f, 1.0f, 0.0f,    // Eye-position
                   0.0f, 20.0f, 0.0f,   // View-point
                   0.0f, 0.0f, 1.0f );  // Up-vector

*/
		
	
        // Swap buffers
		glfwSwapBuffers();
		
        // Check if the ESC key was pressed or the window was closed
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}
	
	delete renderer;
	
    // Close OpenGL window and terminate GLFW
	glfwTerminate();

	return EXIT_SUCCESS;
}

#include <iostream>
#include <sstream>
//#include <GL/glfw.h>
#include <GLUT/GLUT.h>
#include <iomanip>
#include "../tgen.h"
#include "../opengl.h"

using namespace TGen;

class Tja {
public:
	Tja(int ett, int tva) {}
	
};

class TestApp {
public:
	static void Init(int argc, char ** argv) {
		glutInit(&argc, argv);
		
		glutInitWindowSize(windowSize.width, windowSize.height);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		glutCreateWindow("TGen test");
		glutDisplayFunc(&Render);		
		glutReshapeFunc(&Reshape);
		glutIdleFunc(&Render);
		
		renderer = new OpenGL::Renderer();		// We're going to use OpenGL for rendering
		renderer->setClearColor(Color::White);	
		
		LoadData();
	}
	
	static void LoadData() {
		typedef JoinVertex3<Vertex3<float>, Color4<float>, TexCoord2<float, 0> > MyVertex;
		
		MyVertex::Type data[4] = {
			MyVertex::Type(Vector3(10.0f, 10.0f, 0.0f), Color(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f)),
			MyVertex::Type(Vector3(100.0f, 10.0f, 0.0f), Color(0.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f)),
			MyVertex::Type(Vector3(100.0f, 100.0f, 0.0f), Color(0.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f)),
			MyVertex::Type(Vector3(10.0f, 100.0f, 0.0f), Color(1.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f)),
		};

		object = renderer->CreateVertexBuffer(MyVertex(), sizeof(MyVertex::Type) * 4, UsageStatic);
		object->BufferData(data, sizeof(MyVertex::Type) * 4, 0);
	}
	
	static void Shutdown() {
		delete object;
		delete renderer;	
	}
	
	static void Reshape(int width, int height) {
		windowSize = Rectangle(width, height);
		
		if (renderer) {
			renderer->setViewport(windowSize);			
		}
	}
	
	static void Render() {
		if (renderer) {
			renderer->Clear(ColorBuffer | DepthBuffer);		// Clear color buffer and depth (z) buffer
			renderer->setViewport(windowSize);	
			renderer->setTransform(TransformProjection, Matrix4x4::OrthogonalProjection(windowSize));
			renderer->setTransform(TransformWorldView, Matrix4x4::Identity);
						
			renderer->setVertexBuffer(object);
			renderer->setColor(Color::Red);
			renderer->DrawPrimitive(PrimitiveQuads, 0, 4);
			
			glutSwapBuffers();	// Present our back buffer
		}
	}
	
	static Rectangle windowSize;
	static Renderer * renderer;
	static VertexBuffer * object;
};

Rectangle TestApp::windowSize;
Renderer * TestApp::renderer = NULL;
VertexBuffer * TestApp::object = NULL;

int main(int argc, char ** argv) {
	TestApp::windowSize = Rectangle(640, 480);
		
	std::cout << std::string(TestApp::windowSize) << std::endl;
	std::cout << std::string(Matrix3x3::Identity) << std::endl;
	
	TestApp::Init(argc, argv);
	glutMainLoop();
	TestApp::Shutdown();
	
	return EXIT_SUCCESS;
}

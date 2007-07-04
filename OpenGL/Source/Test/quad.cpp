#include <iostream>
#include <GLUT/GLUT.h>
#include "tgen_opengl.h"

using namespace TGen;

void Reshape(int width, int height);
void Render();

class App {
public:
	App(int argc, char ** argv) : renderer(NULL), vb(NULL), ib(NULL), time(0) {
		windowSize.width = 640;
		windowSize.height = 480;
		
		// We need a context...
		glutInit(&argc, argv);
		glutInitWindowSize(windowSize.width, windowSize.height);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		glutCreateWindow("TGen OpenGL");
		glutDisplayFunc(::Render);		
		glutReshapeFunc(::Reshape);
		glutIdleFunc(::Render);
		
		std::cout << "1. Creating renderer..." << std::endl;
		renderer = new OpenGL::Renderer();
		if (!renderer) {
			std::cout << "   ... failed!" << std::endl;
			throw RuntimeException("App::App", "failed to create renderer");
		}
		else {
			std::cout << "   ...ok" << std::endl << std::endl;
		}
		
		std::cout << "2. Loading data..." << std::endl;
		LoadData();
		std::cout << "   ... ok" << std::endl;
	}
	
	~App() {
		delete vb;
		delete ib;
		delete renderer;
	}

	void Reshape(const Rectangle & size) {
		renderer->setViewport(size);
		windowSize = size;
	}
	
	void Render() {
		renderer->setClearColor(Color::White);
		renderer->Clear(ColorBuffer | DepthBuffer);
		
		renderer->setVertexBuffer(vb);
		renderer->setIndexBuffer(ib);
		renderer->setColor(Color::Red);
		
		renderer->setTransform(TransformProjection, Matrix4x4::PerspectiveProjection(60.0f, windowSize.width / windowSize.height, 0.1f, 100.0f));
		renderer->setTransform(TransformWorldView, Matrix4x4::Translation(Vector3(0.0f, 0.0f, -1.0f)) * Matrix4x4::Rotation(Vector3(0.0f, 1.0f, 0.0f), Radian(time)));

		renderer->DrawIndexedPrimitive(PrimitiveTriangles, 0, 6);

		
		glutSwapBuffers();
	}
	
	void LoadData() {
		MyVertex::Type vertices[4] = {
			MyVertex::Type(Vector3(-1.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)),
			MyVertex::Type(Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)),
			MyVertex::Type(Vector3(1.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f)),
			MyVertex::Type(Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f)),
		};
		
		MyIndex::Type indicies[6] = {0, 1, 3, 1, 2, 3};
		
		vb = renderer->CreateVertexBuffer(MyVertex(), sizeof(MyVertex::Type) * 4, UsageStatic);
		vb->BufferData(vertices, sizeof(MyVertex::Type) * 4, 0);
		
		ib = renderer->CreateIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * 6, UsageStatic);
		ib->BufferData(indicies, sizeof(MyIndex::Type) * 6, 0);
	}
	
private:
	typedef Index<unsigned short> MyIndex;
	typedef JoinVertex2<Vertex3<float>, TexCoord2<float, 0> > MyVertex;
	
	Rectangle windowSize;
	Renderer * renderer;
	VertexBuffer * vb;
	IndexBuffer * ib;
	float time;
};

App * app = NULL;

void Reshape(int width, int height) {
	if (app)
		app->Reshape(Rectangle(width, height));
}

void Render() {
	if (app)
		app->Render();
}

int main(int argc, char ** argv) {
	std::cout << "TGen OpenGL (debug binary: " << std::boolalpha << isOpenGLDebug() << ")" << std::endl << std::endl;
	
	app = new App(argc, argv);
	glutMainLoop();
	
	delete app;
	app = NULL;
	
	return EXIT_SUCCESS;
}

#include <iostream>
#include <GLUT/GLUT.h>
#include "tgen_opengl.h"

using namespace TGen;

void Reshape(int width, int height);
void Render();

class App {
public:
	App(int argc, char ** argv) 
		: renderer(NULL)
		, vb(NULL)
		, ib(NULL)
		, time(0) 
		, windowSize(640, 480)
	{
		// We need a window environment, let GLUT create one
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
		
		std::cout << std::string(renderer->getCaps()) << std::endl;

		std::cout << "2. Loading data..." << std::endl;
		LoadData();
		std::cout << "   ... ok" << std::endl; // TODO: wrap ska vara per textur, filter också, cool
	}
	
	~App() {
		delete vb;
		delete ib;
		delete renderer;
	}

	void Reshape(const TGen::Rectangle & size) {
		renderer->setViewport(size);
		windowSize = size;
	}
	
	void Render() {
		time += 0.001;

		renderer->setClearColor(Color::White);
		renderer->Clear(ColorBuffer | DepthBuffer);
		
		Matrix4x4 view = Matrix4x4::Identity;
		view *= Matrix4x4::Translation(Vector3(0.0f, 0.0f, -3.0f));
		view *= Matrix4x4::Rotation(Vector3(0.0f, 1.0f, 0.0f), Radian(time));

		renderer->setTransform(TransformProjection, Matrix4x4::PerspectiveProjection(TGen::Degree(90.0f), windowSize.width / windowSize.height, 0.1f, 10.0f));
		renderer->setTransform(TransformWorldView, view);
		
		renderer->setVertexBuffer(vb);
		renderer->setIndexBuffer(ib);
		renderer->setColor(Color::Red);

		renderer->DrawIndexedPrimitive(PrimitiveTriangles, 0, 3);	// draw triangle #1
		renderer->DrawIndexedPrimitive(PrimitiveTriangles, 3, 3);	//     --|--     #2
		
		glutSwapBuffers();
	}
	
	void LoadData() {
		MyVertex::Type vertices[4] = {
			MyVertex::Type(Vector3(-1.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)),
			MyVertex::Type(Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)),
			MyVertex::Type(Vector3(1.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f)),
			MyVertex::Type(Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f)),
		};
		
		MyIndex::Type indices[6] = {0, 1, 3, 1, 2, 3};
		
		vb = renderer->CreateVertexBuffer(MyVertex(), sizeof(MyVertex::Type) * 4, UsageStatic);
		vb->BufferData(vertices, sizeof(MyVertex::Type) * 4, 0);
		
		ib = renderer->CreateIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * 6, UsageStatic);
		ib->BufferData(indices, sizeof(MyIndex::Type) * 6, 0);
	}
	
private:
	typedef Index<unsigned short> MyIndex;
	typedef JoinVertex2<Vertex3<float>, TexCoord2<float, 0> > MyVertex;
	
	TGen::Rectangle windowSize;
	TGen::Renderer * renderer;
	TGen::VertexBuffer * vb;
	TGen::IndexBuffer * ib;
	float time;
};

App * app = NULL;

void Reshape(int width, int height) {
	if (app)
		app->Reshape(TGen::Rectangle(width, height));
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

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
		glutDisplayFunc(&Render);		
		glutReshapeFunc(&Reshape);
		glutIdleFunc(&Render);
		//glutWMCloseFunc()
			
		renderer = new OpenGL::Renderer();		// We're going to use OpenGL for rendering
		renderer->setClearColor(Color::White);	
		
		std::cout << "opengl caps: " << std::endl << std::string(renderer->getCaps()) << std::endl;
		
		LoadData();
	}
	
	static void LoadData() {
		typedef JoinVertex2<Vertex3<float>, TexCoord2<float, 0> > MyVertex;
		
	/*	MyVertex::Type data[4] = {
			MyVertex::Type(Vector3(10.0f, 10.0f, 0.0f), Color(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f), 0),
			MyVertex::Type(Vector3(100.0f, 10.0f, 0.0f), Color(0.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f), 0),
			MyVertex::Type(Vector3(100.0f, 100.0f, 0.0f), Color(0.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), 0),
			MyVertex::Type(Vector3(10.0f, 100.0f, 0.0f), Color(1.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), 0),
		};
*/
		
		/*MyVertex::Type data[4] = {
			MyVertex::Type(Vector3(10.0f, 10.0f, 0.0f), Vector2(0.0f, 0.0f)),
			MyVertex::Type(Vector3(170.0f, 10.0f, 0.0f), Vector2(1.0f, 0.0f)),
			MyVertex::Type(Vector3(170.0f, 170.0f, 0.0f), Vector2(1.0f, 1.0f)),
			MyVertex::Type(Vector3(10.0f, 170.0f, 0.0f), Vector2(0.0f, 1.0f)),
		};*/
		
		MyVertex::Type data[4] = {
			MyVertex::Type(Vector3(-1.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)),
			MyVertex::Type(Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)),
			MyVertex::Type(Vector3(1.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f)),
			MyVertex::Type(Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f)),
		};
		
		typedef Index<unsigned short> MyIndex;
		MyIndex::Type indicies[6] = {0, 1, 3, 1, 2, 3};
		
		object = renderer->CreateVertexBuffer(MyVertex(), sizeof(MyVertex::Type) * 4, UsageStatic);
		object->BufferData(data, sizeof(MyVertex::Type) * 4, 0);
		
		ib = renderer->CreateIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * 6, UsageStatic);
		ib->BufferData(indicies, sizeof(MyIndex::Type) * 6, 0);
		
		//object2 = renderer->CreateVertexBuffer(MyVertex2(), sizeof(MyVertex2::Type) * 4, UsageStatic);
		//object2->BufferData(data2, sizeof(MyVertex2::Type) * 4, 0);
		
		TGen::Canvas myCanvas(TGen::Rectangle(16, 16));
		myCanvas.Fill(Color::Green);
		myCanvas.setPixel(Vector2(10, 10), Color::Red);
		myCanvas.setPixel(Vector2(3, 7), Color::Red);
		myCanvas.DrawLine(Vector2(1, 1), Vector2(15, 8), Color::Blue);
		myCanvas.DrawLine(Vector2(8, 6), Vector2(2, 15), Color::Blue);
		batch = new TGen::Batch<JoinVertex2<Vertex3<float>, TexCoord2<float, 0> >, 4>(renderer, 1000000, TGen::PrimitiveQuads, TGen::UsageStream);
			 
		tex = renderer->CreateTexture(myCanvas, TGen::RGB);
		renderTarget1 = renderer->CreateTexture(Rectangle(256, 256), RGB, TypeUnsignedByte);
		renderTarget2 = renderer->CreateTexture(Rectangle(256, 256), DEPTH, TypeFloat);
		fb = renderer->CreateFrameBuffer();
		fb->Attach(renderTarget1, ColorAttachment);
		fb->Attach(renderTarget2, DepthAttachment);
		
		std::cout << "--> " << TGen::Radian(TGen::Degree(180)).angle << std::endl;
		
		char shaderCode[] = 
			"#section global					  \n"
			"varying vec4 projPos;				  \n"
			"#section vertex                      \n"
			"void main() {                        \n"
			"gl_Position = ftransform();          \n"
			"projPos = ftransform();			  \n"
			"gl_TexCoord[0] = gl_MultiTexCoord0;  \n"
			"}                                    \n"
			
			"#section fragment                    \n"
			"uniform sampler2D tex1;			  \n"
			"void main() {                        \n"
			"float x = (projPos.x / projPos.w + 1.0) / 2.0;    \n"
			"float y = (projPos.y / projPos.w + 1.0) / 2.0;		\n"
			"if (x >= 0.05 && x <= 0.95 && y >= 0.05 && y <= 0.95)			\n"
			"   gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0) * 0.5 + texture2D(tex1, gl_TexCoord[0].st) * 0.5;  \n"
			"else       \n"
			"   gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);  \n"
			"}  \n";
					
		shaderProgram = renderer->CreateShaderProgram(shaderCode);
		
		
		/*Shader * myShader = renderer->CreateFragmentShader("uniform vec4 useColor; \n"
														   "uniform sampler2D texture; \n"
														   "void main() {\n"
														   "   gl_FragColor = useColor * 0.5 + 0.5 * texture2D(texture, gl_TexCoord[0].xy); \n"
														   "}");
		
		*/
		
		//shaderProgram->Attach(myShader);
		shaderProgram->Link();
	}
	
	// TODO: check for ARB_multitexture 
	
	static void Shutdown() {
		//delete object2;
		delete tex;
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
			static float animTimer = 0.0f;
			animTimer += 0.01f;
			
			//renderer->setTransform(TransformProjection, Matrix4x4::OrthogonalProjection(windowSize));
			renderer->setTransform(TransformProjection, Matrix4x4::PerspectiveProjection(60.0f, windowSize.width / windowSize.height, 0.1f, 100.0f));
			renderer->setTransform(TransformWorldView, Matrix4x4::Translation(Vector3(0.0f, 0.0f, -3.0f)) * Matrix4x4::Rotation(Vector3(0.0f, 1.0f, 0.0f), Radian(animTimer)));
						
			// TODO: texture, setFilter, etc
			renderer->Clear(ColorBuffer | DepthBuffer);		// Clear color buffer and depth (z) buffer
			
			renderer->setTexture(0, tex);
			renderer->setVertexBuffer(object);
			renderer->setIndexBuffer(ib);
			
			renderer->DrawIndexedPrimitive(PrimitiveTriangles, 0, 6);
			
			

						
			renderer->setVertexBuffer(object);
			renderer->setIndexBuffer(ib);
			renderer->setTexture(0, tex);
			renderer->setRenderTarget(fb);	// drawing to our frame buffer
			renderer->setViewport(Rectangle(256, 256));	
			renderer->setClearColor(Color::White);
			renderer->Clear(ColorBuffer | DepthBuffer);		// Clear color buffer and depth (z) buffer
			
			renderer->setShaderProgram(shaderProgram);
			
			//ShaderVariable & var = shaderProgram->getUniform("useColor");
			//var = Color::Blue;
			
			renderer->setTransform(TransformWorldView, renderer->getTransform(TransformWorldView) * Matrix4x4::Translation(Vector3(-5.0f * 3.0f, 0.0f, 0.0f)));

			for (int i = 0; i < 10; i++) {
				renderer->setTransform(TransformWorldView, renderer->getTransform(TransformWorldView) * Matrix4x4::Translation(Vector3(3.0f, 0.0f, 0.0f)));
				renderer->DrawIndexedPrimitive(PrimitiveTriangles, 0, 6);
			}
			renderer->setTransform(TransformWorldView, renderer->getTransform(TransformWorldView) * Matrix4x4::Translation(Vector3(-5.0f * 3.0f, 0.0f, -5.0f * 3.0f)));
			
			for (int i = 0; i < 10; i++) {
				renderer->setTransform(TransformWorldView, renderer->getTransform(TransformWorldView) * Matrix4x4::Translation(Vector3(0.0f, 0.0f, 3.0f)));
				renderer->DrawIndexedPrimitive(PrimitiveTriangles, 0, 6);
			}
			
			renderer->setTransform(TransformWorldView, Matrix4x4::Translation(Vector3(0.0f, 0.0f, -3.0f)));

			renderer->setRenderTarget(NULL);
			renderer->setViewport(windowSize);	// we are now drawing to screen
			
			renderer->setClearColor(Color::Black);

			renderer->Clear(ColorBuffer | DepthBuffer);		// Clear color buffer and depth (z) buffer
			
			renderer->setTransform(TransformWorldView, renderer->getTransform(TransformWorldView) * Matrix4x4::Translation(Vector3(3.0f, 0.0f, 0.0f)));
			renderer->setTexture(0, renderTarget1);
			renderer->setShaderProgram(NULL);
			renderer->DrawIndexedPrimitive(PrimitiveTriangles, 0, 6);

			renderer->setTransform(TransformWorldView, renderer->getTransform(TransformWorldView) * Matrix4x4::Translation(Vector3(-6.0f, 0.0f, 0.0f)));
			renderer->setTexture(0, renderTarget2);
			renderer->DrawIndexedPrimitive(PrimitiveTriangles, 0, 6);
			

			
			/*renderer->setVertexBuffer(object2);
			renderer->setColor(Color::Red);
			renderer->DrawPrimitive(PrimitiveQuads, 0, 4);
			*/
			
			glutSwapBuffers();	// Present our back buffer
		}
	}
	
	static Rectangle windowSize;
	static Renderer * renderer;
	static VertexBuffer * object, * object2;
	static IndexBuffer * ib;
	static Texture * tex, * renderTarget1, * renderTarget2;
	static FrameBuffer * fb;
	static Batch<JoinVertex2<Vertex3<float>, TexCoord2<float, 0> >, 4> * batch;
	static ShaderProgram * shaderProgram;
};

Rectangle TestApp::windowSize;
Renderer * TestApp::renderer = NULL;
VertexBuffer * TestApp::object = NULL;
VertexBuffer * TestApp::object2 = NULL;
IndexBuffer * TestApp::ib = NULL;
Texture * TestApp::tex = NULL;
Texture * TestApp::renderTarget1 = NULL;
Texture * TestApp::renderTarget2 = NULL;
FrameBuffer * TestApp::fb = NULL;
Batch<JoinVertex2<Vertex3<float>, TexCoord2<float, 0> >, 4> * TestApp::batch = NULL;
ShaderProgram * TestApp::shaderProgram = NULL;

int main(int argc, char ** argv) {
	TestApp::windowSize = Rectangle(640, 480);
		
	std::cout << std::string(TestApp::windowSize) << std::endl;
	std::cout << std::string(Matrix3x3::Identity) << std::endl;
	
	TestApp::Init(argc, argv);
	glutMainLoop();
	TestApp::Shutdown();
	
	return EXIT_SUCCESS;
}

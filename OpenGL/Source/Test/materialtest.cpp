#include <iostream>
#include <GLUT/GLUT.h>
#include "tgen_opengl.h"
#include <IL/IL.h>

using namespace TGen;

void Reshape(int width, int height);
void Render();


class Object : public TGen::Renderable {
public:
	Object() : vb(NULL), ib(NULL) {}
	~Object() {
		delete vb;
		delete ib;
	}
	
	void PrepareRender(TGen::Renderer & renderer) const {
		renderer.setVertexBuffer(vb);
		renderer.setIndexBuffer(ib);		
	}
	
	void Render(TGen::Renderer & renderer) const {
		renderer.DrawIndexedPrimitive(PrimitiveTriangles, 0, 6);
	}
	
	void LoadData(TGen::Renderer & renderer) {
		MyVertex::Type vertices[4] = {
			MyVertex::Type(Vector3(-1.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)),
			MyVertex::Type(Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)),
			MyVertex::Type(Vector3(1.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f)),
			MyVertex::Type(Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f)),
		};
		
		MyIndex::Type indicies[6] = {0, 1, 3, 1, 2, 3};
		
		vb = renderer.CreateVertexBuffer(MyVertex(), sizeof(MyVertex::Type) * 4, UsageStatic);
		vb->BufferData(vertices, sizeof(MyVertex::Type) * 4, 0);
		
		ib = renderer.CreateIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * 6, UsageStatic);
		ib->BufferData(indicies, sizeof(MyIndex::Type) * 6, 0);		
	}
	
private:
	typedef Index<unsigned short> MyIndex;
	typedef JoinVertex2<Vertex3<float>, TexCoord2<float, 0> > MyVertex;

	VertexBuffer * vb;
	IndexBuffer * ib;
};

class DevILImage : public TGen::Image {
public:
	DevILImage(ILuint imageName) : imageName(imageName) {}
	
	TGen::Rectangle getSize() const {
		ILuint width, height;
		ilBindImage(imageName);
		width = ilGetInteger(IL_IMAGE_WIDTH);
		height = ilGetInteger(IL_IMAGE_HEIGHT);
		
		return TGen::Rectangle(width, height);
	}
	
	TGen::ImageFormat getFormat() const {
		ilBindImage(imageName);

		ILuint type = ilGetInteger(IL_IMAGE_FORMAT);
		
		switch (type) {
			case IL_RGB:
				return TGen::RGB;
			
			case IL_RGBA:
				return TGen::RGBA;
				
			case IL_BGR:
				return TGen::BGR;
				
			case IL_BGRA:
				return TGen::BGRA;		
				
			default:
				std::cout << "format " << type << std::endl;
		}
		

		
		throw TGen::RuntimeException("DevILImage::getFormat", "unknown format!");
	}
	
	TGen::FormatType getComponentFormat() const {
		return TGen::TypeUnsignedByte;		
	}
	
	const void * getData() const {
		ilBindImage(imageName);
		return ilGetData();
	}
	

private:
	ILuint imageName;
};


class App : public TGen::MaterialLinkCallback {
public:
	App(int argc, char ** argv) : renderer(NULL), time(0), material(NULL) {
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
			std::cout << std::string(renderer->getCaps()) << std::endl;
		}
		
		std::cout << "2. Loading data..." << std::endl;
		LoadData();
		std::cout << "   ... ok" << std::endl;
	}
	
	~App() {
		delete material;
		delete renderer;
	}
	
	TGen::ShaderProgram * getShaderProgram(const std::string & name) {
		std::cout << "getting shader '" << name << "'" << std::endl;
		std::string data = ReadFile(name);
		
		TGen::ShaderProgram * ret = renderer->CreateShaderProgram(data.c_str());
		ret->Link();
		return ret;
	}
	
	TGen::Texture * getTexture(const std::string & name) {
		std::cout << "getting texture '" << name << "'" << std::endl;		
		
		if (name == "$lightmap")
			return NULL;
		
		ILuint imageName;
		ilGenImages(1, &imageName);
		ilBindImage(imageName);
		
		if (!ilLoadImage(name.c_str()))
			throw TGen::RuntimeException("App::getTexture", "image failed to load");
		
		TGen::Texture * newTexture = renderer->CreateTexture(DevILImage(imageName), TGen::RGBA);
		
		ilDeleteImages(1, &imageName);
		
		
		return newTexture;
	}
	
	int getTextureType(const std::string & name) {
		if (name == "$lightmap")
			return 1;
		
		return 0;
	}

	void Reshape(const Rectangle & size) {
		renderer->setViewport(size);
		windowSize = size;
	}
	
	void Render() {
		//time += 0.01f;
		renderer->setClearColor(Color::White);
		renderer->Clear(ColorBuffer | DepthBuffer);
		
		renderer->setTransform(TransformProjection, Matrix4x4::PerspectiveProjection(TGen::Degree(90.0f), windowSize.width / windowSize.height, 0.1f, 100.0f));
		renderer->setTransform(TransformWorldView, Matrix4x4::Translation(Vector3(0.0f, 0.0f, -2.0f)) * Matrix4x4::Rotation(Vector3(0.0f, 1.0f, 0.0f), Radian(time)));

		if (material) {
			TGen::Texture * textureTypes[] = {NULL, NULL};
			
			material->Update(scalar(glutGet(GLUT_ELAPSED_TIME)) / 1000.0);
			material->Render(*renderer, myObject, "default", 9, textureTypes);
		}
		
		glutSwapBuffers();
	}
	
	void LoadData() {
		myObject.LoadData(*renderer);

		std::list<Material *> materials;
		Q3MaterialParser parser;
		parser.Parse(ReadFile("test.shader").c_str(), materials); // material.mat
		
		for (std::list<Material *>::iterator iter = materials.begin(); iter != materials.end(); ++iter) {
			std::cout << (*iter)->getName() << ": " << std::endl;
			(*iter)->Link(*this);
			
			if ((*iter)->getName() == "textures/base_wall/metalfloor_wall_15ow") {
				material = *iter;
			}
			else {
				//delete *iter;
			}
		}
		
		if (!material)
			throw TGen::RuntimeException("App::LoadData", "material \"cool\" not defined");
		
		materials.clear();
	}
	
	std::string ReadFile(const std::string & name) {
		std::string ret;
		
		FILE * file = fopen(name.c_str(), "r");
		if (!file)
			throw TGen::RuntimeException("App::ReadFile", "file not found");
		
		char buffer[1024];
		while (!feof(file)) {
			int read = fread(buffer, 1, 1022, file);
			buffer[read] = 0;
			ret += buffer;
		}
		
		fclose(file);
		
		return ret;		
	}
	
	
private:
	Object myObject;
	Material * material;
	Rectangle windowSize;
	Renderer * renderer;
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
	ilInit();
	std::cout << "TGen OpenGL (debug binary: " << std::boolalpha << isOpenGLDebug() << ")" << std::endl << std::endl;
	
	app = new App(argc, argv);
	glutMainLoop();
	
	delete app;
	app = NULL;
	
	return EXIT_SUCCESS;
}

/*
 *  ogltest.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/17/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include <iostream>
#include <fstream>
#include <tgen_opengl.h>
#include <tgen_core.h>
#include <tgen_renderer.h>
#include <tgen_graphics.h>
#include <SDL/SDL.h>

class App;

App * gApp = NULL;

using namespace TGen;

class Cube : public TGen::StaticGeometry {
public:
	Cube(TGen::Renderer & renderer, scalar width, scalar height, scalar depth) 
		: width(width)
		, height(height)
		, depth(depth)
		, vb(NULL)
		, ib(NULL)
	{
		scalar halfWidth = width / 2.0f;
		scalar halfHeight = height / 2.0f;
		scalar halfDepth = depth / 2.0f;
			
		vb = renderer.createVertexBuffer(MyVertex(), sizeof(MyVertex::Type) * 8, TGen::UsageStatic);
		ib = renderer.createIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * 24, TGen::UsageStatic);
		
		MyVertex::Type vertices[8] = {
			MyVertex::Type(Vector3(-halfWidth, halfHeight, halfDepth), Vector2(0.0f, 0.0f)),		// 0
			MyVertex::Type(Vector3(halfWidth, halfHeight, halfDepth), Vector2(1.0f, 0.0f)),			// 1
			MyVertex::Type(Vector3(halfWidth, -halfHeight, halfDepth), Vector2(1.0f, 1.0f)),		// 2
			MyVertex::Type(Vector3(-halfWidth, -halfHeight, halfDepth), Vector2(0.0f, 1.0f)),		// 3
			
			MyVertex::Type(Vector3(-halfWidth, halfHeight, -halfDepth), Vector2(0.0f, 1.0f)),		// 4
			MyVertex::Type(Vector3(halfWidth, halfHeight, -halfDepth), Vector2(1.0f, 1.0f)),		// 5
			MyVertex::Type(Vector3(halfWidth, -halfHeight, -halfDepth), Vector2(1.0f, 0.0f)),		// 6
			MyVertex::Type(Vector3(-halfWidth, -halfHeight, -halfDepth), Vector2(0.0f, 0.0f)),		// 7
		};
		
		MyIndex::Type indicies[24] = {
			0, 1, 2, 3,		// FRONT
			7, 6, 5, 4,		// BACK
			1, 5, 6, 2,		// RIGHT
			4, 0, 3, 7,		// LEFT
			0, 4, 5, 1,		// TOP
			2, 6, 7, 3,		// BOTTOM
		};
		
		vb->bufferData(vertices, sizeof(MyVertex::Type) * 8, 0);
		ib->bufferData(indicies, sizeof(MyIndex::Type) * 24, 0);		
	}
	
	~Cube() {
		delete vb;
		delete ib;
	}
	
	void preRender(TGen::Renderer & renderer) const {
		renderer.setVertexBuffer(vb);
		renderer.setIndexBuffer(ib);			
	}
	
	void render(TGen::Renderer & renderer) const {
		renderer.drawIndexedPrimitive(TGen::PrimitiveQuads, 0, 24);		
	}
	
	TGen::Vector3 getMax() const {
		return TGen::Vector3(width / 2.0f, height / 2.0f, depth / 2.0f);
	}
	
	TGen::Vector3 getMin() const {
		return -TGen::Vector3(width / 2.0f, height / 2.0f, depth / 2.0f);
	}
	
	TGen::Vector3 getOrigin() const {
		return TGen::Vector3(0.0f, 0.0f, 0.0f);
	}
	
	
private:
	typedef TGen::Index<unsigned short> MyIndex;
	typedef TGen::JoinVertexElements2<TGen::Vertex3<float>, TGen::TexCoord2<float, 0> > MyVertex;

	scalar width, height, depth;
	TGen::VertexBuffer * vb;
	TGen::IndexBuffer * ib;
};

class ResourceManager : public TGen::MaterialSource, public TGen::MeshSource, public TGen::ModelInstantiator {
public:	
	ResourceManager(TGen::Renderer & renderer) {
		TGen::MaterialParser parser;
		parser.parse("    \n material myMat default 0 {					\n "
					 "   lod 9 {								\n "
					 "      pass fixed {						\n"
					 "         color 1 1 1						\n"
				//	 "         alpha 0.3						\n"
					 "         texunit 0 rocketl.jpg {			\n"
					 "         }								\n"
					 "      }								\n"
					 "   }								\n  "
					 "   lod 5 {                \n"
					 "      pass fixed {             \n"
					 "         color 0 0 1             \n"
					 "         //alpha 0.3				\n"
					 "      }               \n"
					 "   }        \n"
					 "   lod 0 {                \n"
					 "      pass fixed {             \n"
					 "         color 0 1 0             \n"
					 "          alpha 0.3			\n"
					 "      }               \n"
					 "   }        \n"
					 "}									\n  "
					 "    "
					 , materials);
		
		
		std::cout << "[res]: " << materials.size() << " materials created" << std::endl;
		
		
		
		
		
		std::ifstream rocketl;
		rocketl.open("railgun.md3", std::ios::binary | std::ios::in);
		if (!rocketl.is_open())
			throw TGen::RuntimeException("main", "failed to open file");
		
		TGen::IstreamAdaptor stream(rocketl);
		TGen::MD3::Parser modelParser;
		TGen::MD3::File * file = modelParser.parse(stream);
		rocketl.close();
		
		
		file->printInfo(std::cout);
		TGen::NewModel * newModel = file->createModel(renderer, 0.001);
		delete file;
		
		models.insert(std::map<std::string, TGen::NewModel *>::value_type("coolModel", newModel));
		
	//	if (newMesh)
		//	meshes.push_back(newMesh);

		
		
		/*std::ifstream testmd5;
		testmd5.open("zfat.md5mesh", std::ios::binary | std::ios::in);
		if (!testmd5.is_open())
			throw TGen::RuntimeException("main", "failed to open file");
		
		TGen::IstreamAdaptor stream2(testmd5);
		TGen::MD5::Parser modelParser2;
		TGen::MD5::File * file2 = modelParser2.parse(stream2);
		testmd5.close();
		*/
		/*
		 Varje animesh har ref till basen som innehåller skeleton och vertices med weights
		 Varje animesh kan ha en ref till en animation

		 Sparas bland modeller i resman:
			mesh:
				1 mesh_base = skeleton + vertices, no video, just RAM. MD5::File
				* mesh_anim = skeleton, no video, just RAM. MD5::Animation
		 
		 Sparas i scenegraphen, inte i resman:
			mesh = mesh_base + animation, video, only VRAM. MD5::Mesh
		 
		 När man instantierar en mesh av en mesh_base så skapas allt
		 */
		
		//file2->printInfo(std::cout);
		
		//TGen::Mesh * md5Mesh = file2->createMesh(renderer, "md5mod", 0.001);

		//if (md5Mesh)
		//	meshes.push_back(md5Mesh);
	}
	
	~ResourceManager() {
		for (std::list<TGen::Material *>::iterator iter = materials.begin(); iter != materials.end(); ++iter)
			delete *iter;
		
		for (std::list<TGen::Mesh *>::iterator iter = meshes.begin(); iter != meshes.end(); ++iter)
			delete *iter;
	}
	
	TGen::Material * getMaterial(const std::string & name) {
		for (std::list<TGen::Material *>::iterator iter = materials.begin(); iter != materials.end(); ++iter) {
			if ((*iter)->getName() == "myMat")
				return *iter;
		}
		
		throw TGen::RuntimeException("ResourceManager::getMaterial", "material '" + name + "' not loaded");
	}
	
	TGen::Mesh * getMesh(const std::string & name) {
		for (std::list<TGen::Mesh *>::iterator iter = meshes.begin(); iter != meshes.end(); ++iter) {
			if ((*iter)->getName() == name)
				return *iter;
		}
		
		throw TGen::RuntimeException("ResourceManager::getMesh", "mesh '" + name + "' not loaded");		
	}
	
	TGen::NewModelInstance * instantiateModel(const std::string & name) {
		std::map<std::string, TGen::NewModel *>::iterator iter = models.find(name);
		if (iter == models.end())
			throw TGen::RuntimeException("ResourceManger", "Model '" + name + "' not loaded");
		
		return iter->second->instantiate();
	}
	
private:
	std::list<TGen::Material *> materials;
	std::list<TGen::Mesh *> meshes;
	std::map<std::string, TGen::NewModel *> models;
};

class App {
public:
	App() 
		: running(true)
		, sceneRoot("root", TGen::Vector3(0.0f, 0.0f, 0.0f))
	{
		renderer = new TGen::OpenGL::Renderer;
		resources = new ResourceManager(*renderer);
		
		camera = new TGen::Camera("cam", TGen::Vector3(0.0f, 0.0f, -4.0f));	// TODO: bara +Z är korrekt..... måste inverse
		camera->setClip(0.1f, 10.0f);
		camera->setLod(0.0f, 20.0f);
		camera->setOrientation(TGen::Vector3(0.0f, 0.0f, 1.0f).normalize());
		
		sceneRoot.addChild(camera);
		/*sceneRoot.addChild(new TGen::SceneNode("cube1", TGen::Vector3(0.0f, 0.0f, 2.0f)));
		//sceneRoot.addChild(new TGen::SceneNode("cube2", TGen::Vector3(0.0f, 0.0f, 7.0f)));
		//sceneRoot.addChild(new TGen::SceneNode("cube3", TGen::Vector3(0.0f, 0.0f, 15.0f)));
		//sceneRoot.addChild(new TGen::SceneNode("cube4", TGen::Vector3(0.0f, 0.0f, 20.0f)));
		
		//sceneRoot.getChild("cube1")->addFace(TGen::Face(new Cube(*renderer, 1.0f, 1.0f, 1.0f), "myMat"));
		//sceneRoot.getChild("cube2")->addFace(TGen::Face(new Cube(*renderer, 1.0f, 1.0f, 1.0f), "myMat"));
		//sceneRoot.getChild("cube3")->addFace(TGen::Face(new Cube(*renderer, 1.0f, 1.0f, 1.0f), "myMat"));
		//sceneRoot.getChild("cube4")->addFace(TGen::Face(new Cube(*renderer, 1.0f, 1.0f, 1.0f), "myMat"));

		sceneRoot.addChild(new TGen::SceneNode("rocketlauncher", TGen::Vector3(0.0f, 0.0f, 0.0f)));
		sceneRoot.getChild("rocketlauncher")->addFace(TGen::Face(meshList.attach(new TGen::MeshGeometry("models/weapons2/railgun/railgun.md3")), "myMat"));
		
		sceneRoot.addChild(new TGen::SceneNode("rocketlauncher2", TGen::Vector3(0.0f, 2.0f, 0.0f)));
		sceneRoot.getChild("rocketlauncher2")->addFace(TGen::Face(meshList.attach(new TGen::MeshGeometry("models/weapons2/railgun/railgun.md3")), "myMat"));
		
		
		sceneRoot.getChild("rocketlauncher")->addChild(new TGen::SceneNode("rocketlauncher2", TGen::Vector3(2.0f, 0.0f, 0.0f)));
		sceneRoot.getChild("rocketlauncher")->getChild("rocketlauncher2")->addFace(TGen::Face(meshList.attach(new TGen::MeshGeometry("models/weapons2/railgun/railgun.md3")), "myMat"));

		sceneRoot.getChild("rocketlauncher")->getChild("rocketlauncher2")->addChild(new TGen::SceneNode("rocketlauncher2", TGen::Vector3(2.0f, 0.0f, 0.0f)));
		sceneRoot.getChild("rocketlauncher")->getChild("rocketlauncher2")->getChild("rocketlauncher2")->addFace(TGen::Face(meshList.attach(new TGen::MeshGeometry("models/weapons2/railgun/railgun.md3")), "myMat"));
		sceneRoot.getChild("rocketlauncher")->getChild("rocketlauncher2")->getChild("rocketlauncher2")->setOrientation(TGen::Vector3(0.0f, 0.0f, -1.0f));
		
		*/
		
		TGen::ModelInstantiatePool pool;
		
		sceneRoot.addModel(pool.attach(new TGen::ModelInstanceProxy("coolModel")));
		
		pool.instantiateAll(*resources);
		meshList.relink(*resources);
		
		sceneRoot.update();
		sceneRoot.traverse(TGen::FaceLinker(*resources));
		sceneRoot.traverse(TGen::ScenePrinter(std::cout));
		
		//exit(1);
	}
	// SceneNode ska ha en setDirection..
	// TODO: AABB-walker. MD3!!!!!!!! sen leka runt med olika rendreringstekniker
	
	~App() {
		delete camera;
		delete resources;
		delete renderer;
		
	}	
	// glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST)   i ortho
	// glTexSubImage2D or glTexCopyTexSubImage2D  istället för glTexImage2D om uppdatera ofta
	// TODO: kunna uppdatera en texturs data
	
	bool isRunning() const { return running; }
	void quit() {running = false; }
	
	void keyDown(const SDL_keysym & keysym) {	
		switch (keysym.sym) {
			case SDLK_ESCAPE:
				quit();
				break;
		}
	}
				
	
	void render() {
		static float time = 0.0f;
		renderer->clearBuffers(TGen::ColorBuffer | TGen::DepthBuffer);
		
		time += 0.01f;
		//sceneRoot.getChild("rocketlauncher")->setOrientation(TGen::Vector3(0.0f, 0.0f, 1.0f).getNormalized()); //TGen::Cos(TGen::Radian(time)), 0.0f, TGen::Sin(TGen::Radian(time))));
		sceneRoot.update();
		
		renderList.clear();
		sceneRoot.traverse(TGen::RenderFiller(renderList, *camera));
		renderList.sort(*camera, "default");
		//renderList.print();
		renderList.render(*renderer, *camera, "default");
		
		
		
		/*time += 0.001f;
		TGen::Vector3 orient;
		orient.x = TGen::Cos(TGen::Radian(time / 2.0f));
		orient.y = 0.0f;
		orient.z = TGen::Sin(TGen::Radian(time / 2.0f));
		orient.normalize();
		
		std::cout << "time : " << time << std::endl;
		
		
		sceneRoot.getChild("cube1")->setOrientation(orient);
		
		
		//sceneRoot.getChild("cube1")->setPosition(TGen::Vector3(0.0f, 0.0f, 5.0f - time));
		sceneRoot.update();
		*/
		
		SDL_GL_SwapBuffers();
		SDL_Delay(20);
	}
	
private:
	bool running;
	TGen::Renderer * renderer;
	ResourceManager * resources;
	TGen::BasicRenderList renderList;
	TGen::MeshGeometryLinkList meshList;
	
	TGen::SceneNode sceneRoot;
	TGen::Camera * camera;
};






//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////




int main(int argc, char ** argv) {
	std::cout << "TGen Renderer Graphics Demo (debug binary: " << std::boolalpha << TGen::isRendererDebug() << ")" << std::endl << std::endl;
	
	uint32 initflags = SDL_INIT_VIDEO;
	uint32 videoflags = SDL_HWSURFACE | SDL_OPENGL;// | SDL_FULLSCREEN;
	
	SDL_Surface * screen = NULL;
	uint8 video_bpp = 0;
	
	if (SDL_Init(initflags) < 0)
		throw TGen::RuntimeException("main", "failed to initialize SDL: ") << SDL_GetError();
	
	// SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 0 );
	SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
	SDL_WM_SetCaption("TGen Renderer Graphics Demo", NULL);
	//SDL_GrabMode(SDL_GRAB_ON);
	//SDL_WM_GrabInput(SDL_GRAB_ON);
	//SDL_ShowCursor(0);
	
	screen = SDL_SetVideoMode(800, 600, video_bpp, videoflags);
	if (!screen) {
		throw TGen::RuntimeException("main", "failed to set video mode: ") << SDL_GetError();
		//SDL_Quit();
	}
	
	
	//SDL_GetRelativeMouseState(NULL, NULL);
	
	SDL_Event event;
	gApp = new App;
	std::cout << "======================== running ========================" << std::endl;
	
	while (gApp->isRunning()) {
		int events = 0;
		
		while (SDL_PollEvent(&event) && events < 100) {
			switch (event.type) {
				case SDL_KEYDOWN:
					gApp->keyDown(event.key.keysym);
					break;
					
				case SDL_KEYUP:
					//gApp->KeyUp(event.key.keysym);					
					break;
					
				case SDL_MOUSEMOTION:
					//gApp->MouseMove(event.motion);
					break;
					
				case SDL_QUIT:
					gApp->quit();
					break;
					
			}
			
			events++;
		}
		
		//gApp->Update();
		gApp->render();
	}
	
	std::cout << "===================== shutting down =====================" << std::endl;
	
	delete gApp;
	
	SDL_Quit();
		
	return EXIT_SUCCESS;
}

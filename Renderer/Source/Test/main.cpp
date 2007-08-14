#include <iostream>
#include "tgen_renderer.h"

class ResourceManager : public TGen::MaterialSource {
public:
	ResourceManager() {
		TGen::Q3MaterialParser parser;
		std::list<TGen::Material *> materials;
		parser.parse("floormaterial {      \n "
					 "   {           \n "
					 "   }           \n  "
					 "}        \n     ", materials);
		
		std::cout << materials.size() << " materials created" << std::endl;
		for (std::list<TGen::Material *>::iterator iter = materials.begin(); iter != materials.end(); ++iter) {
			mat1 = *iter;
		}
		
		mat1->setSortLevel(TGen::MaterialSortTransparent);
	}
	
	TGen::Material * getMaterial(const std::string & name) {
		return mat1;
	}
	
	TGen::Material * mat1;
};

class Shubniggurath : public TGen::Geometry {
public:
	Shubniggurath(const TGen::Vector3 & origin)
		: origin(origin)
	{
	}
	
	void preRender(TGen::Renderer & renderer) const {
		std::cout << this << " preRender" << std::endl;
	}
	
	void render(TGen::Renderer & renderer) const {
		std::cout << this << " render" << std::endl;
	}
	
	void update(/*const TGen::Camera & camera,*/ const TGen::Vector3 & position) {
		std::cout << this << " update" << std::endl;
	}
	
	TGen::Vector3 getMax() const {
		return origin + TGen::Vector3(1.0f, 1.0f, 1.0f);
	}
	
	TGen::Vector3 getMin() const {
		return origin - TGen::Vector3(1.0f, 1.0f, 1.0f);
	}
	
	TGen::Vector3 getOrigin() const {
		return origin;
	}
	
private:
	TGen::Vector3 origin;
};

int main(int argc, char ** argv) {
	std::cout << "TGen Renderer (debug binary: " << std::boolalpha << TGen::isRendererDebug() << ")" << std::endl;

	ResourceManager resources;
	Shubniggurath * enemy1 = new Shubniggurath(TGen::Vector3(0.0f, 10.0f, 0.0f));
	Shubniggurath * enemy2 = new Shubniggurath(TGen::Vector3(0.0f, 15.0f, 0.0f));
	Shubniggurath * enemy3 = new Shubniggurath(TGen::Vector3(0.0f, 25.0f, 0.0f));
	Shubniggurath * enemy4 = new Shubniggurath(TGen::Vector3(0.0f, -1.0f, 0.0f));
	Shubniggurath * enemy5 = new Shubniggurath(TGen::Vector3(0.0f, 2.0f, 0.0f));
	
	TGen::Face * floorFace = new TGen::Face(enemy1, "floormaterial");
	TGen::Face * wallFace = new TGen::Face(enemy2, "wallmaterial");
	TGen::Face * wallFace2 = new TGen::Face(enemy3, "wallmaterial");
	TGen::Face * wallFace3 = new TGen::Face(enemy4, "wallmaterial");
	TGen::Face * wallFace4 = new TGen::Face(enemy5, "wallmaterial");
	
	TGen::SceneNode * root = new TGen::SceneNode("root", TGen::Vector3(0.0f, 0.0f, 5.0f));
	TGen::SceneNode * level = new TGen::SceneNode("level", TGen::Vector3(0.0f, 0.0f, 0.0f));
	TGen::SceneNode * player = new TGen::SceneNode("player", TGen::Vector3(0.0f, 0.0f, 10.0f));
	TGen::SceneNode * bsp = new TGen::SceneNode("bsp", TGen::Vector3(0.0f, 0.0f, 0.0f));
	
	root->addChild(level);
	root->addChild(player);
	level->addChild(bsp);
	bsp->addFace(floorFace);
	bsp->addFace(wallFace);
	bsp->addFace(wallFace2);
	bsp->addFace(wallFace3);
	bsp->addFace(wallFace4);
	
	TGen::Camera * camera = new TGen::Camera("cam", TGen::Vector3(0.0f, 0.0f, 0.0f));
	root->addChild(camera);
	
	root->traverse(TGen::FaceLinker(resources));
	root->traverse(TGen::ScenePrinter(std::cout));

	TGen::BasicRenderList renderList;
	root->traverse(TGen::RenderFiller(renderList, *camera));
	
	
	for (int i = 0; i < 5; ++i) {
		camera->setPosition(TGen::Vector3(0.0f, i * 5, 0.0f));
		root->update();
		std::cout << "cam position: " << std::string(camera->getWorldPosition()) << std::endl;
		renderList.sort(*camera);
		renderList.print();
		std::cout << std::endl;
	}
	
	delete bsp;
	delete player;
	delete level;	
	delete root;
	
	delete wallFace;
	delete floorFace;
	
	delete enemy1;
	delete enemy2;
	
	return EXIT_SUCCESS;
}

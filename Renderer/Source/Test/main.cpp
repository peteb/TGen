#include <iostream>
#include "tgen_renderer.h"

int main(int argc, char ** argv) {
	std::cout << "TGen Renderer (debug binary: " << std::boolalpha << TGen::isRendererDebug() << ")" << std::endl;

	TGen::SceneNode * root = new TGen::SceneNode("root", TGen::Vector3(0.0f, 0.0f, 0.0f));
	TGen::SceneNode * level = new TGen::SceneNode("level", TGen::Vector3(0.0f, 0.0f, 0.0f));
	TGen::SceneNode * player = new TGen::SceneNode("player", TGen::Vector3(0.0f, 0.0f, 10.0f));
	TGen::SceneNode * bsp = new TGen::SceneNode("bsp", TGen::Vector3(0.0f, 0.0f, 0.0f));
	
	root->addChild(level);
	root->addChild(player);
	level->addChild(bsp);
	
	root->traverse(TGen::ScenePrinter(std::cout));
	
	delete bsp;
	delete player;
	delete level;	
	delete root;
	
	return EXIT_SUCCESS;
}

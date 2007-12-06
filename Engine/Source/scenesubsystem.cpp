/*
 *  scenesubsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "scenesubsystem.h"
#include "scenenodecomponent.h"
#include "world.h"
#include "app.h"

TGen::Engine::SceneSubsystem::SceneSubsystem(TGen::Engine::World & world)
	: sceneRoot("root")
	, world(world)
{
	
}

TGen::Engine::SceneSubsystem::~SceneSubsystem() {
	
}

TGen::Engine::Component * TGen::Engine::SceneSubsystem::createComponent(TGen::Engine::Entity * entity, const TGen::PropertyTree & properties) {
	bool createComponent = (properties.hasProperty("model") || properties.hasProperty("position"));
	TGen::Engine::SceneNodeComponent * component;
	
	if (createComponent) {
		std::string name = properties.getProperty("name", "untitled");
		std::string origin = properties.getProperty("origin", "0 0 0");
		std::string treePosition = properties.getProperty("tree_position", "");
		std::string modelName = properties.getProperty("model", "");
		std::string materialName = properties.getProperty("material", "");
				
		TGen::SceneNode * sceneNode = new TGen::SceneNode(name, TGen::Vector3::Parse(origin));
		TGen::SceneNode * parentNode = sceneRoot.getNode(treePosition, true);
		
		if (parentNode)
			parentNode->addChild(sceneNode);
		else
			throw TGen::RuntimeException("SceneSubsystem::createComponent", "failed to get parent node");
		
		if (!modelName.empty()) {
			TGen::MeshGeometry * mesh = new TGen::MeshGeometry(modelName);
			meshList.attach(mesh);
			sceneNode->addFace(TGen::Face(mesh, materialName));
		}
		
		component = new TGen::Engine::SceneNodeComponent(sceneNode);
		world.app.logs.info["scene"] << "created component for node '" << name << "': origin: " << origin << " treePosition: " << treePosition << " model: " << modelName << TGen::endl;

		/*
		sceneRoot.addChild(new TGen::SceneNode("weapon", TGen::Vector3(0.0f, 0.0f, 1.0f)));
		sceneRoot.getChild("weapon")->addFace(TGen::Face(meshList.attach(new TGen::MeshGeometry("models/railgun.md3")), "railgunMaterial"));	// hur anger md3:an material?
		*/
	}
	
	return component;
}

void TGen::Engine::SceneSubsystem::link() {
	world.app.logs.info["scene"] << "*** LINKING SCENE ***" << TGen::endl;
	meshList.relink(world.app.globalResources);
	sceneRoot.traverse(TGen::FaceLinker(world.app.globalResources));
	sceneRoot.traverse(TGen::ScenePrinter(std::cout));	
}

TGen::SceneNode & TGen::Engine::SceneSubsystem::getSceneRoot() {
	return sceneRoot;
}
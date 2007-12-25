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
#include "entity.h"
#include "light.h"

TGen::Engine::SceneSubsystem::SceneSubsystem(TGen::Engine::World & world)
	: sceneRoot("root")
	, world(world)
{
	
}

TGen::Engine::SceneSubsystem::~SceneSubsystem() {

}

TGen::Engine::Component * TGen::Engine::SceneSubsystem::createComponent(TGen::Engine::Entity * entity, const TGen::PropertyTree & properties) {
	bool doCreateNode = properties.getName() == "sceneNode";
	bool doCreateCamera = properties.getName() == "sceneCamera";
	bool doCreateLight = properties.getName() == "sceneLight";
	
	std::string name = entity->getName();

	TGen::SceneNode * sceneNode = NULL;
	
	if (doCreateCamera)
		sceneNode = createCameraNode(name, properties);
	else if (doCreateLight)
		sceneNode = createLightNode(name, properties);
	else if (doCreateNode)
		sceneNode = createNode(name, properties);
	else
		throw TGen::RuntimeException("SceneSubsystem::createComponent", "subsystem can't handle component type '" + properties.getName() + "'");
	
	TGen::SceneNode * parentNode = sceneRoot.getNode(properties.getProperty("treePosition", ""), true);
	
	if (!parentNode)
		throw TGen::RuntimeException("SceneSubsystem::createComponent", "failed to get parent node");

	parentNode->addChild(sceneNode);

	TGen::Engine::SceneNodeComponent * newComponent = new TGen::Engine::SceneNodeComponent(sceneNode);
	if (components.find(entity->getName()) == components.end())
		components.insert(ComponentMap::value_type(entity->getName(), newComponent));
	
	return newComponent;
}

TGen::SceneNode * TGen::Engine::SceneSubsystem::createCameraNode(const std::string & name, const TGen::PropertyTree & properties) {
	TGen::Camera * camera = new TGen::Camera(name, TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0")));
	camera->setClip(0.1f, TGen::lexical_cast<float>(properties.getProperty("range", "300")));
	camera->setLod(0.0f, TGen::lexical_cast<float>(properties.getProperty("range", "300")));
	camera->setOrientation(TGen::Vector3::Parse(properties.getProperty("orientation", "0 0 1")).normalize());
	
	return camera;
}

TGen::SceneNode * TGen::Engine::SceneSubsystem::createLightNode(const std::string & name, const TGen::PropertyTree & properties) {
	TGen::Engine::Light * light = new TGen::Engine::Light(name, TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0")));
	
	light->setOrientation(TGen::Vector3::Parse(properties.getProperty("orientation", "0 0 1")).normalize());
	light->setMaterialName(properties.getProperty("material", ""));
	light->getLightProperties().diffuse = TGen::Color::Parse(properties.getProperty("diffuse", "0 0 0"));
	light->getLightProperties().specular = TGen::Color::Parse(properties.getProperty("specular", "0 0 0"));
	
	light->linkMaterial(world.app.globalResources);	// TODO: should probably be called somewhere else! like scenesubsystem.link!!
	
	return light;
}

TGen::Engine::SceneNodeComponent * TGen::Engine::SceneSubsystem::getComponent(const std::string & name) {
	ComponentMap::iterator iter = components.find(name);
	if (iter != components.end())
		return iter->second;
	
	return NULL;
}

TGen::SceneNode * TGen::Engine::SceneSubsystem::createNode(const std::string & name, const TGen::PropertyTree & properties) {
	TGen::SceneNode * node = new TGen::SceneNode(name, TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0")));

	node->setOrientation(TGen::Vector3::Parse(properties.getProperty("orientation", "0 0 1")).normalize());

	std::string modelName = properties.getProperty("model", "");
	
	if (!modelName.empty()) {
		TGen::MeshGeometry * mesh = new TGen::MeshGeometry(modelName);
		meshList.attach(mesh);
		node->addFace(TGen::Face(mesh, properties.getProperty("material", "")));
	}
	
	return node;
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

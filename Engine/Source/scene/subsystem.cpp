/*
 *  scenesubsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "scene/subsystem.h"
#include "scene/node.h"
#include "world.h"
#include "app.h"
#include "entity.h"
#include "light.h"
#include "map.h"
#include "maploader.h"

TGen::Engine::Scene::Subsystem::Subsystem(TGen::Engine::ResourceManager & resources, TGen::Engine::Filesystem & filesystem, TGen::Engine::StandardLogs & logs, TGen::VertexDataSource & dataSource)
	: sceneRoot("root")
	, resources(resources)
	, filesystem(filesystem)
	, logs(logs)
	, dataSource(dataSource)
{
	
}

TGen::Engine::Scene::Subsystem::~Subsystem() {

}

TGen::Engine::Component * TGen::Engine::Scene::Subsystem::createComponent(TGen::Engine::Entity * entity, const TGen::PropertyTree & properties) {
	bool doCreateNode = properties.getName() == "sceneNode";
	bool doCreateCamera = properties.getName() == "sceneCamera";
	bool doCreateLight = properties.getName() == "sceneLight";
	bool doCreateMap = properties.getName() == "sceneMap";
	
	std::string name = entity->getName();

	TGen::SceneNode * sceneNode = NULL;
	
	if (doCreateCamera)
		sceneNode = createCameraNode(name, properties);
	else if (doCreateLight)
		sceneNode = createLightNode(name, properties);
	else if (doCreateNode)
		sceneNode = createNode(name, properties);
	else if (doCreateMap)
		sceneNode = createMapNode(name, properties);
	else
		throw TGen::RuntimeException("SceneSubsystem::createComponent", "subsystem can't handle component type '" + properties.getName() + "'");
	
	TGen::SceneNode * parentNode = sceneRoot.getNode(properties.getProperty("treePosition", ""), true);
	
	if (!parentNode)
		throw TGen::RuntimeException("SceneSubsystem::createComponent", "failed to get parent node");

	parentNode->addChild(sceneNode);

	TGen::Engine::Scene::Node * newComponent = new TGen::Engine::Scene::Node("sceneNode", sceneNode);
	if (components.find(entity->getName()) == components.end())
		components.insert(ComponentMap::value_type(entity->getName(), newComponent));
	
	return newComponent;
}

TGen::SceneNode * TGen::Engine::Scene::Subsystem::createCameraNode(const std::string & name, const TGen::PropertyTree & properties) {
	TGen::Camera * camera = new TGen::Camera(name, TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0")), TGen::Rotation::Identity);
	camera->setClip(0.1f, TGen::lexical_cast<float>(properties.getProperty("range", "300")));
	camera->setLod(0.0f, TGen::lexical_cast<float>(properties.getProperty("range", "300")));
	
	TGen::Vector3 orientation = TGen::Vector3::Parse(properties.getProperty("orientation", "0 0 1")).normalize();
	TGen::Rotation rotation = TGen::Rotation::LookInDirection(orientation, TGen::Vector3(0.0f, 1.0f, 0.0f));
	
	camera->setOrientation(rotation);
	camera->update();
	
	return camera;
}

TGen::SceneNode * TGen::Engine::Scene::Subsystem::createLightNode(const std::string & name, const TGen::PropertyTree & properties) {
	TGen::Engine::Light * light = new TGen::Engine::Light(name, TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0")));
	
	
	TGen::Vector3 orientation = TGen::Vector3::Parse(properties.getProperty("orientation", "0 0 1")).normalize();
	TGen::Vector3 position;
		
	TGen::Rotation rotation = TGen::Rotation::LookInDirection(orientation, TGen::Vector3(0.0f, 1.0f, 0.0f));
	
	light->setOrientation(rotation);
	
	light->setMaterialName(properties.getProperty("material", ""));
	light->getLightProperties().diffuse = TGen::Color::Parse(properties.getProperty("diffuse", "0 0 0"));
	light->getLightProperties().specular = TGen::Color::Parse(properties.getProperty("specular", "0 0 0"));
	
	light->getLightProperties().constantAttenuation = TGen::lexical_cast<scalar>(properties.getProperty("constantAttenuation", "0"));
	light->getLightProperties().linearAttenuation = TGen::lexical_cast<scalar>(properties.getProperty("linearAttenuation", "0"));
	light->getLightProperties().quadraticAttenuation = TGen::lexical_cast<scalar>(properties.getProperty("quadraticAttenuation", "0"));
	
	std::string modelName = properties.getProperty("model", "");
	
	if (!modelName.empty())
		light->addModel(modelPool.attach(new TGen::ModelInstanceProxy(modelName, properties.getProperty("material", ""))));
	
	/*spotCutoff, spotExponent;
	TGen::Vector3 spotDirection;
	TGen::Vector4 position;
	TGen::Color diffuse, specular;*/
	
	light->linkMaterial(resources);	// TODO: should probably be called somewhere else! like scenesubsystem.link!!
	// TODO: resources -> materialsource
	
	return light;
}

TGen::SceneNode * TGen::Engine::Scene::Subsystem::createNode(const std::string & name, const TGen::PropertyTree & properties) {
	TGen::SceneNode * node = new TGen::SceneNode(name, TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0")));

	TGen::Vector3 orientation = TGen::Vector3::Parse(properties.getProperty("orientation", "0 0 1")).normalize();
	TGen::Rotation rotation = TGen::Rotation::LookInDirection(orientation, TGen::Vector3(0.0f, 1.0f, 0.0f));
	
	node->setOrientation(rotation);

	std::string modelName = properties.getProperty("model", "");
	
	if (!modelName.empty()) {
		//TGen::MeshGeometry * mesh = new TGen::MeshGeometry(modelName);
		//meshList.attach(mesh);
		//node->addFace(TGen::Face(mesh, properties.getProperty("material", "")));
		node->addModel(modelPool.attach(new TGen::ModelInstanceProxy(modelName, properties.getProperty("material", ""))));
	}
	
	return node;
}

TGen::SceneNode * TGen::Engine::Scene::Subsystem::createMapNode(const std::string & name, const TGen::PropertyTree & properties) {
	TGen::Engine::MapLoader loader(logs, filesystem);
	TGen::Engine::Map * map = loader.createMap(name, properties.getProperty("model", ""), TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0")));
	map->createVertexData(dataSource);
	
	return map;
}

TGen::Engine::Scene::Node * TGen::Engine::Scene::Subsystem::getComponent(const std::string & name) {
	ComponentMap::iterator iter = components.find(name);
	if (iter != components.end())
		return iter->second;
	
	throw TGen::RuntimeException("SceneSubsystem::getComponent", "entity '" + name + "' could not be found");
}

// TODO: refactor all world.app.logs-blabla-shit to logs

void TGen::Engine::Scene::Subsystem::link() {
	logs.info["scene"] << "*** LINKING SCENE ***" << TGen::endl;
	//meshList.relink(world.app.globalResources);
	modelPool.instantiateAll(resources);
	
	sceneRoot.traverse(TGen::FaceLinker(resources));
	//sceneRoot.setPosition(TGen::Vector3(0.0f, 100.0f, 0.0f));
	sceneRoot.update();

	sceneRoot.traverse(TGen::ScenePrinter(std::cout));	
}

TGen::SceneNode & TGen::Engine::Scene::Subsystem::getSceneRoot() {
	return sceneRoot;
}

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
#include "scene/transformnode.h"

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

TGen::Engine::Component * TGen::Engine::Scene::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	bool doCreateNode = properties.getName() == "sceneNode";
	bool doCreateCamera = properties.getName() == "sceneCamera";
	bool doCreateLight = properties.getName() == "sceneLight";
	bool doCreateMap = properties.getName() == "sceneMap";
	bool doCreateTransform = properties.getName() == "sceneTransform";
	
	// TODO: vad händer om man mergar med olika attribut på en node, blir fel!
	
	TGen::SceneNode * sceneNode = NULL;
	
	if (doCreateCamera)
		sceneNode = createCameraNode(entityName, properties);
	else if (doCreateLight)
		sceneNode = createLightNode(entityName, properties);
	else if (doCreateNode)
		sceneNode = createNode(entityName, properties);
	else if (doCreateMap)
		sceneNode = createMapNode(entityName, properties);
	else if (doCreateTransform)
		sceneNode = createTransformNode(entityName, properties);
	else
		throw TGen::RuntimeException("SceneSubsystem::createComponent", "subsystem can't handle component type '" + properties.getName() + "'");
	
	TGen::SceneNode * parentNode = NULL;
	
	std::string treePosition = properties.getProperty("treePosition", "");
	std::string autoTP = properties.getProperty("autoTP", "");
	
	if (!autoTP.empty()) {
		TGen::SceneNode * parent = sceneRoot.getNode(autoTP, true);
	
		if (!parent)
			throw TGen::RuntimeException("SceneSubsystem::createComponent", "failed to get parent node for autoTP");
		
		sceneNode->setAutoTP(parent);
		parentNode = parent->getNodeFromPoint(sceneNode->getLocalPosition());

		if (!parentNode) {
			logs.warning["scene"] << "no node for point " << std::string(sceneNode->getLocalPosition()) << " found! Using parent!" << TGen::endl;
			parentNode = parent;
		}
		
		if (parentNode == parent) {
			logs.warning["scene"] << "getNodeFromPoint returned parent!" << TGen::endl;
		}
	}
	else {
		parentNode = sceneRoot.getNode(treePosition);
	}
	
	if (!parentNode)
		throw TGen::RuntimeException("SceneSubsystem::createComponent", "failed to get parent node '" + treePosition + "'");

	parentNode->addChild(sceneNode);

	TGen::Engine::Scene::Node * newComponent = new TGen::Engine::Scene::Node(name, sceneNode);
	if (components.find(entityName) == components.end())
		components.insert(ComponentMap::value_type(entityName, newComponent));
	
	nodes.push_back(newComponent);
	
	return newComponent;
}

TGen::SceneNode * TGen::Engine::Scene::Subsystem::createCameraNode(const std::string & name, const TGen::PropertyTree & properties) {	
	TGen::Camera * camera = new TGen::Camera(properties.getProperty("globalName", name), 
														  TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0")), 
														  TGen::Rotation::Identity);
	camera->setClip(0.1f, TGen::lexical_cast<float>(properties.getProperty("range", "300")));
	camera->setLod(0.0f, TGen::lexical_cast<float>(properties.getProperty("range", "300")));
	camera->setFov(TGen::lexical_cast<float>(properties.getProperty("fov", "80")));
	
	TGen::Vector3 orientation = TGen::Vector3::Parse(properties.getProperty("orientation", "0 0 1")).normalize();
	TGen::Rotation rotation = TGen::Rotation::LookInDirection(orientation, TGen::Vector3(0.0f, 1.0f, 0.0f));
	
	camera->setOrientation(rotation);
	camera->update(0.0f);
	
	return camera;
}

TGen::SceneNode * TGen::Engine::Scene::Subsystem::createLightNode(const std::string & name, const TGen::PropertyTree & properties) {
	TGen::Engine::Light * light = new TGen::Engine::Light(properties.getProperty("globalName", name), 
																			TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0")));
	
	
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
	TGen::SceneNode * node = new TGen::SceneNode(properties.getProperty("globalName", name),
																TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0")));

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
	std::string modelName = properties.getProperty("model", "");
	TGen::Vector3 origin = TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0"));
	TGen::VertexOffsetter offsetter(origin);
	
	TGen::Engine::Map * map = loader.createMap(name, modelName, offsetter);
	map->instantiate(dataSource);
	map->linkMaterial(resources);
	
	//exit(1);
	
	return map;
}

TGen::SceneNode * TGen::Engine::Scene::Subsystem::createTransformNode(const std::string & name, const TGen::PropertyTree & properties) {
	std::auto_ptr<TGen::Engine::Scene::TransformNode> newTransformer(new TGen::Engine::Scene::TransformNode(properties.getProperty("globalName", name)));
	
	if (properties.hasNode("transPosition"))
		newTransformer->addPositionTransformer(properties.getNode("transPosition"));
	
	if (properties.hasNode("transOrientation"))
		newTransformer->addOrientationTransformer(properties.getNode("transOrientation"));
	
	return newTransformer.release();
}


TGen::Engine::Scene::Node * TGen::Engine::Scene::Subsystem::getComponent(const std::string & name) {
	ComponentMap::iterator iter = components.find(name);
	if (iter != components.end())
		return iter->second;
	
	throw TGen::RuntimeException("SceneSubsystem::getComponent", "entity '" + name + "' could not be found");
}

void TGen::Engine::Scene::Subsystem::link() {
	logs.info["scene"] << "*** LINKING SCENE ***" << TGen::endl;
	//meshList.relink(world.app.globalResources);
	modelPool.instantiateAll(resources);
	
	sceneRoot.traverse(TGen::FaceLinker(resources));
	//sceneRoot.setPosition(TGen::Vector3(0.0f, 100.0f, 0.0f));
	sceneRoot.update(0.0f);

	sceneRoot.traverse(TGen::ScenePrinter(std::cout));	
}

TGen::SceneNode & TGen::Engine::Scene::Subsystem::getSceneRoot() {
	return sceneRoot;
}

void TGen::Engine::Scene::Subsystem::update(scalar delta) {
	sceneRoot.update(delta);
	
	for (int i = 0; i < nodes.size(); ++i) {
		nodes[i]->update(delta);
	}
}

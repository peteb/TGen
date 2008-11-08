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
#include "scene/dummynode.h"
#include "scene/transformnode.h"
#include "scene/equipmentnode.h"
#include "scene/equipmentdata.h"

#include "world.h"
#include "app.h"
#include "entity.h"
#include "light.h"
#include "map.h"
#include "maploader.h"
#include "generateline.h"
#include "transformerfactory.h"

TGen::Engine::Scene::Subsystem::Subsystem(TGen::Engine::ResourceManager & resources, TGen::Engine::Filesystem & filesystem, TGen::Engine::StandardLogs & logs, TGen::VertexDataSource & dataSource)
	: sceneRoot("root")
	, resources(resources)
	, filesystem(filesystem)
	, logs(logs)
	, dataSource(dataSource)
{
	
}

TGen::Engine::Scene::Subsystem::~Subsystem() {
	try {
		sceneRoot.traverse(TGen::ScenePrinter(std::cout));	
	}
	catch (...) {
		
	}
}


TGen::Engine::Component * TGen::Engine::Scene::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	bool doCreateNode = properties.getName() == "sceneNode";
	bool doCreateCamera = properties.getName() == "sceneCamera";
	bool doCreateLight = properties.getName() == "sceneLight";
	bool doCreateMap = properties.getName() == "sceneMap";
	bool doCreateTransform = properties.getName() == "sceneTransform";
	bool doCreateEquipmentNode = properties.getName() == "sceneEqNode";
	
	// TODO: vad händer om man mergar med olika attribut på en node, blir fel!
	
	TGen::SceneNode * sceneNode = NULL;
	
	std::string useName = name;
	
	if (useName == properties.getName())
		useName = entityName;
	
	if (doCreateCamera)
		sceneNode = createCameraNode(useName, properties);
	else if (doCreateLight)
		sceneNode = createLightNode(useName, properties);
	else if (doCreateNode)
		sceneNode = createNode(useName, properties);
	else if (doCreateMap)
		sceneNode = createMapNode(useName, properties);
	else if (doCreateTransform)
		sceneNode = createTransformNode(useName, properties);
	else if (doCreateEquipmentNode)
		sceneNode = createEquipmentNode(useName, properties);
	else
		throw TGen::RuntimeException("SceneSubsystem::createComponent", "subsystem can't handle component type '" + properties.getName() + "'");
	
	std::string autoTP = properties.getProperty("autoParent", "");	
	std::string linkParent = properties.getProperty("relative", properties.getProperty("link", properties.getProperty("parent", "")));
	
	sceneRoot.addChild(sceneNode);
	
	TGen::Engine::Scene::Node * newComponent = new TGen::Engine::Scene::Node(name, sceneNode);

	addComponent(newComponent, name);
	
	newComponent->setLink(linkParent);
	newComponent->setAutoParent(autoTP);
	newComponent->setAttachComponent(properties.getProperty("attachComponent", ""));
	newComponent->setAttachJoint(properties.getProperty("attachJoint", ""));
	
	// TODO: NEJ!!!!!!! kör jointComponent, jointAttach... går inte att göra det där andra med animation, eller jo, går väl.. men blir komplicerat. kan dock fixa senare
	
	return newComponent;
}


void TGen::Engine::Scene::Subsystem::addComponent(TGen::Engine::Scene::Node * node, const std::string & name) {
	if (components.find(name) == components.end())
		components.insert(std::make_pair(name, node));
	
	nodes.push_back(node);
}


TGen::Engine::Scene::NodeRecipe * TGen::Engine::Scene::Subsystem::createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	TGen::SceneNode * prototypeNode = NULL;
	
	std::string useName = name + "_p";
	
	if (properties.getName() == "sceneNode")
		prototypeNode = createNode(useName, properties, true); // new TGen::SceneNode(name);
	else if (properties.getName() == "sceneTransform")
		prototypeNode = createTransformNode(useName, properties, true);
	else if (properties.getName() == "sceneLight")
		prototypeNode = createLightNode(useName, properties, true);
	else if (properties.getName() == "sceneCamera")
		prototypeNode = createCameraNode(useName, properties, true);
	else if (properties.getName() == "sceneEqNode")
		prototypeNode = createEquipmentNode(useName, properties, true);
	else
		throw TGen::RuntimeException("Scene::Subsystem::createComponentRecipe", "invalid component type: " + properties.getName());
	
	sceneRoot.addChild(prototypeNode);
	
	std::auto_ptr<TGen::Engine::Scene::NodeRecipe> newRecipe(new TGen::Engine::Scene::NodeRecipe(name, prototypeNode, *this));
		
	newRecipe->setLink(properties.getProperty("relative", properties.getProperty("link", "")));
	
	return newRecipe.release();
}


TGen::SceneNode * TGen::Engine::Scene::Subsystem::createCameraNode(const std::string & name, const TGen::PropertyTree & properties, bool dummy) {	
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


TGen::SceneNode * TGen::Engine::Scene::Subsystem::createLightNode(const std::string & name, const TGen::PropertyTree & properties, bool dummy) {
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


// per-model-data måste sparas per-node och skickas till modellens fillFaces
TGen::SceneNode * TGen::Engine::Scene::Subsystem::createNode(const std::string & name, const TGen::PropertyTree & properties, bool dummy) {
	TGen::SceneNode * node = NULL;
	
	if (!dummy)
		node = new TGen::SceneNode(properties.getProperty("globalName", name));
	else
		node = new TGen::Engine::Scene::DummyNode(properties.getProperty("globalName", name));
	
	TGen::Vector3 orientation = TGen::Vector3::Parse(properties.getProperty("orientation", "0 0 1")).normalize();
	TGen::Rotation rotation = TGen::Rotation::LookInDirection(orientation, TGen::Vector3(0.0f, 1.0f, 0.0f));
	
	node->setOrientation(rotation);
	node->setPosition(TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0")));
	
	if (properties.getProperty("modelFront", "ccw") == "cw")
		node->getRenderProperties().frontFaceDef = TGen::FaceWindingCW;
	
	std::string modelName = properties.getProperty("model", "");
	
	if (!modelName.empty()) {
		node->addModel(modelPool.attach(new TGen::ModelInstanceProxy(modelName, properties.getProperty("material", ""))));
	}
	
	return node;
}


TGen::SceneNode * TGen::Engine::Scene::Subsystem::createEquipmentNode(const std::string & name, const TGen::PropertyTree & properties, bool dummy) {
	TGen::Engine::Scene::EquipmentNode * newNode = new TGen::Engine::Scene::EquipmentNode(properties.getProperty("globalName", name));
	
	TGen::Vector3 origin = TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0"));
	TGen::Vector3 orientation = TGen::Vector3::Parse(properties.getProperty("orientation", "0 0 1")).normalize();
	TGen::Rotation rotation = TGen::Rotation::LookInDirection(orientation, TGen::Vector3(0.0f, 1.0f, 0.0f));
	
	newNode->setPosition(origin);
	newNode->setOrientation(rotation);
	newNode->setInitialChild(properties.getProperty("initial", ""));
	
	for (int i = 0; i < properties.getNumNodes(); ++i) {
		const TGen::PropertyTree & node = properties.getNode(i);
		
		if (node.getName() == "equipment") {
			TGen::Engine::Scene::EquipmentData * newEquipment = new TGen::Engine::Scene::EquipmentData(node.getAttribute(0));
			newEquipment->setWeapon(node.getProperty("weapon", ""));
			newEquipment->setRequires(node.getProperty("requires", ""));
			
			newNode->setEquipmentData(node.getAttribute(0), newEquipment);
		}
	}
	
	return newNode;
}


TGen::SceneNode * TGen::Engine::Scene::Subsystem::createMapNode(const std::string & name, const TGen::PropertyTree & properties, bool dummy) {
	TGen::Engine::MapLoader loader(logs, filesystem);
	
	TGen::Engine::GenerateLine line("gen:" + properties.getProperty("model", ""));

	std::string modelName = line.getName();
	TGen::Vector3 origin = TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0"));
	
	TGen::Engine::TransformerFactory transFactory;
	
	TGen::VertexTransformList transformers;
	//transformers.addTransformer(new TGen::VertexOffsetter(origin));
	transformers.addTransformer(transFactory.createTransformers(line));
	
	
	TGen::Engine::Map * map = loader.createMap(name, modelName, transformers);

	map->instantiate(dataSource);
	map->linkMaterial(resources);
	
	return map;
}


TGen::SceneNode * TGen::Engine::Scene::Subsystem::createTransformNode(const std::string & name, const TGen::PropertyTree & properties, bool dummy) {
	std::auto_ptr<TGen::Engine::Scene::TransformNode> newTransformer;
	
	if (!dummy)
		newTransformer.reset(new TGen::Engine::Scene::TransformNode(properties.getProperty("globalName", name)));
	else
		newTransformer.reset(new TGen::Engine::Scene::DummyTransformNode(properties.getProperty("globalName", name)));
		
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

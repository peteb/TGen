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
#include "scene/equipmentnode.h"

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
	
	//TGen::SceneNode * parentNode = NULL;
	
	//std::string treePosition = properties.getProperty("relative", "");
	std::string autoTP = properties.getProperty("autoParent", "");
	
	// fixa autoParent först, den använder en link till en component
	
	/*if (!autoTP.empty()) {
		TGen::SceneNode * parent = sceneRoot.getNode(autoTP, true);
		// TODO: treePosition ska sätta parent, autoTP sätter bara vad den ska kolla mot
		// treePosition borde även heta parent
		// nu dock: WorldObject ska göra allt i world coords! och ta bort spacetransform
		
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
	else {*/
	//}
	
	//parentNode = sceneRoot.getNode(treePosition);

	//if (!parentNode)
		//parentNode = &sceneRoot;
	
	std::string linkParent = properties.getProperty("relative", properties.getProperty("link", ""));
	
	sceneRoot.addChild(sceneNode);
	
	/*if (linkParent.empty()) {
		if (!parentNode)
			throw TGen::RuntimeException("SceneSubsystem::createComponent", "failed to get parent node '" + treePosition + "'");

		parentNode->addChild(sceneNode);
	}*/

	TGen::Engine::Scene::Node * newComponent = new TGen::Engine::Scene::Node(name, sceneNode);

	addComponent(newComponent, name);
	
	newComponent->setLinkWith(linkParent);
	newComponent->setAutoParent(autoTP);
	
	
	return newComponent;
}

void TGen::Engine::Scene::Subsystem::addComponent(TGen::Engine::Scene::Node * node, const std::string & name) {
	if (components.find(name) == components.end())
		components.insert(std::make_pair(name, node));
	
	nodes.push_back(node);
}

TGen::Engine::Scene::NodeRecipe * TGen::Engine::Scene::Subsystem::createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	TGen::SceneNode * prototypeNode = createNode(name + "_prototype", properties); // new TGen::SceneNode(name);
	sceneRoot.addChild(prototypeNode);
	
	std::auto_ptr<TGen::Engine::Scene::NodeRecipe> newRecipe(new TGen::Engine::Scene::NodeRecipe(name, prototypeNode, *this));
	
	std::cout << "hey" << std::endl;

	/*
	 TGen::SceneNode * node = new TGen::SceneNode(properties.getProperty("globalName", name),
	 TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0")));
	 
	 TGen::Vector3 orientation = TGen::Vector3::Parse(properties.getProperty("orientation", "0 0 1")).normalize();
	 TGen::Rotation rotation = TGen::Rotation::LookInDirection(orientation, TGen::Vector3(0.0f, 1.0f, 0.0f));
	 
	 node->setOrientation(rotation);
	 
	 std::string modelName = properties.getProperty("model", "");
	 
	 if (!modelName.empty()) {
	 node->addModel(modelPool.attach(new TGen::ModelInstanceProxy(modelName, properties.getProperty("material", ""))));
	 }
	
	 */
		
	newRecipe->setLinkWith(properties.getProperty("relative", properties.getProperty("link", "")));
	
	return newRecipe.release();
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
		node->addModel(modelPool.attach(new TGen::ModelInstanceProxy(modelName, properties.getProperty("material", ""))));
	}
	
	return node;
}

TGen::SceneNode * TGen::Engine::Scene::Subsystem::createEquipmentNode(const std::string & name, const TGen::PropertyTree & properties) {
	TGen::Engine::Scene::EquipmentNode * node = new TGen::Engine::Scene::EquipmentNode(properties.getProperty("globalName", name));
	
	TGen::Vector3 origin = TGen::Vector3::Parse(properties.getProperty("origin", "0 0 0"));
	TGen::Vector3 orientation = TGen::Vector3::Parse(properties.getProperty("orientation", "0 0 1")).normalize();
	TGen::Rotation rotation = TGen::Rotation::LookInDirection(orientation, TGen::Vector3(0.0f, 1.0f, 0.0f));
	
	node->setPosition(origin);
	node->setOrientation(rotation);
	node->setInitialChild(properties.getProperty("initial", ""));
	
	return node;
}


TGen::SceneNode * TGen::Engine::Scene::Subsystem::createMapNode(const std::string & name, const TGen::PropertyTree & properties) {
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

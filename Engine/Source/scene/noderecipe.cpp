/*
 *  noderecipe.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "noderecipe.h"
#include "entityrecipe.h"
#include "entitylist.h"
#include "scene/subsystem.h"
#include <tgen_renderer.h>

TGen::Engine::Scene::NodeRecipe::NodeRecipe(const std::string & name, TGen::SceneNode * prototypeNode, TGen::Engine::Scene::Subsystem & subsystem) 
	: TGen::Engine::ComponentRecipe(name)
	, parent(NULL)
	, prototypeNode(prototypeNode)
	, subsystem(subsystem)
{
	
}

TGen::Engine::Scene::NodeRecipe::~NodeRecipe() {
	//delete prototypeNode;  // TODO: remove prototype node when recipe is removed
}

TGen::Engine::Scene::Node * TGen::Engine::Scene::NodeRecipe::createComponent(const TGen::Engine::EntityRecipe & entity) {
	TGen::SceneNode * newNode = new TGen::SceneNode(*prototypeNode);
	//newNode->setOrientation(orientation);
	//newNode->setPosition(origin);
	
	TGen::Engine::Scene::Node * newComponent = new TGen::Engine::Scene::Node(name, newNode);
	
	subsystem.addComponent(newComponent, entity.getName());
	
	return newComponent;
}

void TGen::Engine::Scene::NodeRecipe::setOrigin(const TGen::Vector3 & origin) {
	this->origin = origin;
}

void TGen::Engine::Scene::NodeRecipe::setOrientation(const TGen::Rotation & orientation) {
	this->orientation = orientation;
}

// TODO: link component when created, multiple components makes tree structure fucked up

void TGen::Engine::Scene::NodeRecipe::setModel(const std::string & name, const std::string & materialName) {
	this->modelName = name;
	this->materialName = materialName;
}

void TGen::Engine::Scene::NodeRecipe::setLinkWith(const std::string & linkWith) {
	this->linkWith = linkWith;
}

TGen::SceneNode * TGen::Engine::Scene::NodeRecipe::getPrototypeNode() {
	return prototypeNode;
}

void TGen::Engine::Scene::NodeRecipe::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::EntityRecipe & entity) {
	if (!linkWith.empty()) {
		TGen::Engine::Scene::NodeRecipe * parent = dynamic_cast<TGen::Engine::Scene::NodeRecipe *>(entity.getComponentRecipe(linkWith));
		
		if (!parent)
			throw TGen::RuntimeException("NodeRecipe::linkGlobally", "failed to link with " + linkWith);
		
		prototypeNode->moveTo(parent->getPrototypeNode(), false);
		
		//changed = true;
	}	
	//exit(1);
}

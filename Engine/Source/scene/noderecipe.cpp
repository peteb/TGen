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
#include "entity.h"
#include "scene/subsystem.h"
#include "componentlinker.h"

#include <tgen_renderer.h>

TGen::Engine::Scene::NodeRecipe::NodeRecipe(const std::string & name, TGen::SceneNode * prototypeNode, TGen::Engine::Scene::Subsystem & subsystem) 
	: TGen::Engine::ComponentRecipe(name)
	, parent(NULL)
	, prototypeNode(prototypeNode)
	, subsystem(subsystem)
	, refersSelfEntity(false)
{
	
}


TGen::Engine::Scene::NodeRecipe::~NodeRecipe() {
	//delete prototypeNode;  // probably removed by the scene subsystem because it's in the scene graph
}


TGen::Engine::Scene::Node * TGen::Engine::Scene::NodeRecipe::createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing) {
	TGen::SceneNode * newNode = prototypeNode->clone();
	
	if (!refersSelfEntity) {
		prototypeNode->getParent()->addChild(newNode);
	}
	else {
		TGen::Engine::Scene::Node & parent = dynamic_cast<TGen::Engine::Scene::Node &>(constructing.getComponent(linkName));

		parent.getSceneNode()->addChild(newNode);
	}
	
	//newNode->update(0.0f);
	
	TGen::Engine::Scene::Node * newComponent = new TGen::Engine::Scene::Node(name, newNode);	
	subsystem.addComponent(newComponent, entity.getName());
	
	return newComponent;
}


void TGen::Engine::Scene::NodeRecipe::setLink(const std::string & linkName) {
	this->linkName = linkName;
}


TGen::SceneNode * TGen::Engine::Scene::NodeRecipe::getPrototypeNode() {
	return prototypeNode;
}


void TGen::Engine::Scene::NodeRecipe::prelink(const TGen::Engine::ComponentLinker & linker) {
	// called when the global entitylist is linked, entities created at runtime are not linked
	// TODO: fix: autoParent
	
	if (!linkName.empty()) {
		TGen::Engine::Scene::NodeRecipe * parent = dynamic_cast<TGen::Engine::Scene::NodeRecipe *>(linker.getEntityRecipe()->getComponentRecipe(linkName, std::nothrow));
		
		if (!parent) {
			TGen::Engine::Scene::Node * parent = dynamic_cast<TGen::Engine::Scene::Node *>(linker.getComponent(linkName));

			if (parent)
				prototypeNode->moveTo(parent->getSceneNode(), false);
		}
		else {
			refersSelfEntity = true;
			prototypeNode->moveTo(parent->getPrototypeNode(), false);
		}
		
		//changed = true;
	}	
}

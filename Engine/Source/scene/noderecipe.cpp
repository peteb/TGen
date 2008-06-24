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
	//delete prototypeNode;  // TODO: remove prototype node when recipe is removed
}

TGen::Engine::Scene::Node * TGen::Engine::Scene::NodeRecipe::createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing) {
	TGen::SceneNode * newNode = prototypeNode->clone();
	
	// TODO: SceneNode::clone! special case for transform, camera, etc.
	// TODO: why doesn't multiple sound sources work?
	
	if (!refersSelfEntity) {
		prototypeNode->getParent()->addChild(newNode);
	}
	else {
		TGen::Engine::Scene::Node * parent = dynamic_cast<TGen::Engine::Scene::Node *>(constructing.getComponent(linkWith));
		if (!parent)
			throw TGen::RuntimeException("Scene::NodeRecipe::createComponent", "can't find parent component: " + linkWith);
		
		parent->getSceneNode()->addChild(newNode);
	}
	
	//newNode->update(0.0f);
	
	TGen::Engine::Scene::Node * newComponent = new TGen::Engine::Scene::Node(name, newNode);	
	subsystem.addComponent(newComponent, entity.getName());
	
	return newComponent;
}

void TGen::Engine::Scene::NodeRecipe::setLinkWith(const std::string & linkWith) {
	this->linkWith = linkWith;
}

TGen::SceneNode * TGen::Engine::Scene::NodeRecipe::getPrototypeNode() {
	return prototypeNode;
}

void TGen::Engine::Scene::NodeRecipe::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::EntityRecipe & entity) {
	// called when the global entitylist is linked, entities created at runtime are not linked
	// TODO: fix: autoParent, copy model instances
	
	if (!linkWith.empty()) {
		TGen::Engine::Scene::NodeRecipe * parent = dynamic_cast<TGen::Engine::Scene::NodeRecipe *>(entity.getComponentRecipe(linkWith));
		
		if (!parent) {
			TGen::Engine::Scene::Node * parent = dynamic_cast<TGen::Engine::Scene::Node *>(entities.getComponent(linkWith));
			
			if (!parent)
				throw TGen::RuntimeException("NodeRecipe::linkGlobally", "failed to link with " + linkWith);
			else
				prototypeNode->moveTo(parent->getSceneNode(), false);
		}
		else {
			refersSelfEntity = true;
			prototypeNode->moveTo(parent->getPrototypeNode(), false);
		}
		
		//changed = true;
	}	
}

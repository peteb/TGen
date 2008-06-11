/*
 *  equipmentnode.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "equipmentnode.h"

TGen::Engine::Scene::EquipmentNode::EquipmentNode(const std::string & name) 
	: TGen::SceneNode(name)
	, visibleNode(NULL)
{
	
}

TGen::Engine::Scene::EquipmentNode::~EquipmentNode() {
	
}

void TGen::Engine::Scene::EquipmentNode::linkGlobally(TGen::Engine::EntityList & list, TGen::Engine::Entity & entity) {
	visibleNode = getChild(initialChild);
}

void TGen::Engine::Scene::EquipmentNode::setInitialChild(const std::string & initialChild) {
	this->initialChild = initialChild;
}

void TGen::Engine::Scene::EquipmentNode::traverse(const TGen::SceneNode::Walker & walker) {
	if (visibleNode)
		visibleNode->traverse(walker);	
}

/*
 *  node.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "scene/node.h"
#include "scene/equipmentnode.h"

#include "entitylist.h"
#include "entity.h"
#include <tgen_renderer.h>

TGen::Engine::Scene::Node::Node(const std::string & name, TGen::SceneNode * sceneNode) 
	: TGen::Engine::Component(name)
	, sceneNode(sceneNode)
	, changed(true)
	, parentInverseTransform(TGen::Matrix4x4::Identity)
{
	
}

TGen::Engine::Scene::Node::~Node() {
	
}

TGen::SceneNode * TGen::Engine::Scene::Node::getSceneNode() {
	return sceneNode;
}

TGen::Vector3 TGen::Engine::Scene::Node::getPosition() const {
	return sceneNode->getWorldPosition();
}

TGen::Rotation TGen::Engine::Scene::Node::getOrientation() const {
	return sceneNode->getWorldOrientation();
}

TGen::Vector3 TGen::Engine::Scene::Node::getVelocity() const {
	return velocity;
}


void TGen::Engine::Scene::Node::setPosition(const TGen::Vector3 & pos) {
	sceneNode->setPosition(getParentInverseTransform() * pos);
}

void TGen::Engine::Scene::Node::setOrientation(const TGen::Rotation & orientation) {
	sceneNode->setOrientation(getParentInverseTransform() * orientation);
}


void TGen::Engine::Scene::Node::update(scalar dt) {
	velocity = (sceneNode->getWorldPosition() - lastPos) / dt;
	lastPos = sceneNode->getWorldPosition();
	
	if (sceneNode->checkChanged())
		changed = true;
}

const TGen::Matrix4x4 & TGen::Engine::Scene::Node::getParentInverseTransform() {
	if (changed) {
		if (sceneNode->getParent())
			parentInverseTransform = sceneNode->getParent()->getTransform().getInverse();
		else
			parentInverseTransform = TGen::Matrix4x4::Identity;
		
		changed = false;
	}
		
	return parentInverseTransform;
}

void TGen::Engine::Scene::Node::setLinkWith(const std::string & linkWith) {
	this->linkWith = linkWith;
}

void TGen::Engine::Scene::Node::setAutoParent(const std::string & autoParent) {
	this->autoParent = autoParent;
}

void TGen::Engine::Scene::Node::linkGlobally(TGen::Engine::EntityList & list, TGen::Engine::Entity & entity) {
	if (!linkWith.empty()) {
		TGen::Engine::Scene::Node * parent = dynamic_cast<TGen::Engine::Scene::Node *>(list.getComponent(linkWith, entity));
		
		sceneNode->moveTo(parent->getSceneNode(), false);
		
		changed = true;
	}
	
	if (!autoParent.empty()) {
		TGen::Engine::Scene::Node * parent = dynamic_cast<TGen::Engine::Scene::Node *>(list.getComponent(autoParent, entity));
		if (!parent)
			throw TGen::RuntimeException("Scene::Node::linkGlobally", "no node called '" + autoParent + "' for autoParent");
		
		sceneNode->setAutoTP(parent->getSceneNode());		
		changed = true;
	}
	
	TGen::Engine::Scene::EquipmentNode * equipmentNode = dynamic_cast<TGen::Engine::Scene::EquipmentNode *>(sceneNode);
	
	if (equipmentNode) {	// much easier than to subclass this class just for eq node
		equipmentNode->linkGlobally(list, entity);
	}
}


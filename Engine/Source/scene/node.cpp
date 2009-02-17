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
#include "componentlinker.h"

#include <tgen_renderer.h>

TGen::Engine::Scene::Node::Node(const std::string & name, TGen::SceneNode * sceneNode, TGen::Engine::Entity & entity) 
	: TGen::Engine::Component(name)
	, sceneNode(sceneNode)
	, changed(true)
	, parentInverseTransform(TGen::Matrix4x4::Identity)
	, scriptInterface(name, entity.getScriptInterface())
{
	
}

TGen::Engine::Scene::Node::~Node() {
	
}

TGen::SceneNode * TGen::Engine::Scene::Node::getSceneNode() {
	return sceneNode;
}

TGen::Vector3 TGen::Engine::Scene::Node::getPosition() const {
	TGenAssert(sceneNode);

	return sceneNode->getWorldPosition();
}

TGen::Rotation TGen::Engine::Scene::Node::getOrientation() const {
	TGenAssert(sceneNode);
	
	return sceneNode->getWorldOrientation();
}

TGen::Vector3 TGen::Engine::Scene::Node::getVelocity() const {
	TGenAssert(sceneNode);
	
	return velocity;
}


void TGen::Engine::Scene::Node::setPosition(const TGen::Vector3 & pos) {
	sceneNode->setPosition(getParentInverseTransform() * pos);
	bo = getParentInverseTransform() * pos;
}

void TGen::Engine::Scene::Node::setOrientation(const TGen::Rotation & orientation) {
	sceneNode->setOrientation(getParentInverseTransform() * orientation);
}


void TGen::Engine::Scene::Node::update(scalar dt) {
	if (attachComponent) {
		//std::cout << "hey " << attachJoint << std::endl;
		
	}
	
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

void TGen::Engine::Scene::Node::setLink(const std::string & linkName) {
	this->linkName = linkName;
}

void TGen::Engine::Scene::Node::setAutoParent(const std::string & autoParent) {
	this->autoParent = autoParent;
}

// TODO: autoTP ska gå mot ett interface som kan positionera sceneNodes, inte direkt mot en sceneNode typ

void TGen::Engine::Scene::Node::link(const TGen::Engine::ComponentLinker & linker) {
	if (!linkName.empty()) {
		TGen::Engine::Scene::Node * parent = dynamic_cast<TGen::Engine::Scene::Node *>(linker.getComponent(linkName));
		
		if (parent) {
			if (!attachJoint.empty())
				sceneNode->moveTo(parent->getSceneNode()->getChild(attachJoint), false);
			else
				sceneNode->moveTo(parent->getSceneNode(), false);

			
			
		}
		
		
		changed = true;
	}
	
	if (!autoParent.empty()) {
		TGen::Engine::Scene::Node * parent = dynamic_cast<TGen::Engine::Scene::Node *>(linker.getComponent(autoParent));
		
		sceneNode->setAutoTP(parent->getSceneNode());		
		changed = true;
	}
	
	TGen::Engine::Scene::EquipmentNode * equipmentNode = dynamic_cast<TGen::Engine::Scene::EquipmentNode *>(sceneNode);
	
	if (equipmentNode) {	// much easier than to subclass this class just for eq node
		equipmentNode->link(linker);
	}
	
	attachComponent.link(linker);
}

void TGen::Engine::Scene::Node::setEnabled(bool enabled) {
	std::cout << "NODE ENABLED SET TO: " << TGen::lexical_cast<std::string>(enabled) << std::endl;
	if (sceneNode)
		sceneNode->setRender(enabled);
}

void TGen::Engine::Scene::Node::setAttachComponent(const std::string & componentName) {
	attachComponent.set(componentName);
}

void TGen::Engine::Scene::Node::setAttachJoint(const std::string & jointName) {
	this->attachJoint = jointName;
}



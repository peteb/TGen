/*
 *  noderecipe.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "noderecipe.h"
#include <tgen_renderer.h>

TGen::Engine::Scene::NodeRecipe::NodeRecipe(const std::string & name, TGen::SceneNode * prototypeNode) 
	: TGen::Engine::ComponentRecipe(name)
	, parent(NULL)
	, prototypeNode(prototypeNode)
{
	
}

TGen::Engine::Scene::NodeRecipe::~NodeRecipe() {
	//delete prototypeNode;  // TODO: remove prototype node when recipe is removed
}

TGen::Engine::Scene::Node * TGen::Engine::Scene::NodeRecipe::createComponent() {
	TGen::SceneNode * newNode = new TGen::SceneNode(*prototypeNode);
	//newNode->setOrientation(orientation);
	//newNode->setPosition(origin);
	
	return new TGen::Engine::Scene::Node(name, newNode);
}

void TGen::Engine::Scene::NodeRecipe::setOrigin(const TGen::Vector3 & origin) {
	this->origin = origin;
}

void TGen::Engine::Scene::NodeRecipe::setOrientation(const TGen::Rotation & orientation) {
	this->orientation = orientation;
}

void TGen::Engine::Scene::NodeRecipe::setModel(const std::string & name, const std::string & materialName) {
	this->modelName = name;
	this->materialName = materialName;
}

void TGen::Engine::Scene::NodeRecipe::setLinkWith(const std::string & linkWith) {
	this->linkWith = linkWith;
}

void TGen::Engine::Scene::NodeRecipe::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::EntityRecipe & entity) {
	std::cout << "LINK RECIPE" << std::endl;
	
	//exit(1);
}

/*
 *  transformnode.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "scene/transformnode.h"

TGen::Engine::Scene::TransformNode::TransformNode(const std::string & name)
	: TGen::SceneNode(name)
	, tim(0.0)
{
}


void TGen::Engine::Scene::TransformNode::addPositionTransformer(const TGen::PropertyTree & properties) {

}

void TGen::Engine::Scene::TransformNode::addOrientationTransformer(const TGen::PropertyTree & properties) {

}

void TGen::Engine::Scene::TransformNode::update() {
	setPosition(TGen::Vector3(0.0f, sin(tim), 0.0f));
	
	tim += 0.1f;
	TGen::SceneNode::update();
}

/*
 *  bodycomponent.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "bodycomponent.h"
#include "scenenodecomponent.h"
#include "entity.h"
#include <tgen_renderer.h>

TGen::Engine::BodyComponent::BodyComponent(const std::string & name, float mass, const TGen::Vector3 & position) 
	: TGen::Engine::Component(name)
	, mass(mass)
	, position(position)
	, sceneNodeComponent(NULL)
{
	
}

TGen::Engine::BodyComponent::~BodyComponent() {
	
}

void TGen::Engine::BodyComponent::update(float dt) {
	position -= TGen::Vector3(0.0f, 1.0f, 0.0f) * 0.1f * dt;
	
	if (sceneNodeComponent)
		sceneNodeComponent->getSceneNode()->setPosition(position);

}

void TGen::Engine::BodyComponent::link(TGen::Engine::Entity & entity) {
	sceneNodeComponent = dynamic_cast<TGen::Engine::SceneNodeComponent *>(entity.getComponent("sceneNode"));
	
	position = sceneNodeComponent->getSceneNode()->getLocalPosition();
}

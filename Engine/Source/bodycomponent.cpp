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

TGen::Engine::BodyComponent::BodyComponent(const std::string & name, dBodyID bodyId) 
	: TGen::Engine::Component(name)
	, bodyId(bodyId)
	, sceneNodeComponent(NULL)
{
	
}

TGen::Engine::BodyComponent::~BodyComponent() {
	dBodyDestroy(bodyId);
}

void TGen::Engine::BodyComponent::preStep() {
	if (sceneNodeComponent)
		setPosition(sceneNodeComponent->getSceneNode()->getLocalPosition());
}

void TGen::Engine::BodyComponent::postStep() {
	//position -= TGen::Vector3(0.0f, 1.0f, 0.0f) * 1.0f * dt;
	
	if (sceneNodeComponent)
		sceneNodeComponent->getSceneNode()->setPosition(getPosition());

}

void TGen::Engine::BodyComponent::linkLocally(TGen::Engine::Entity & entity) {
	sceneNodeComponent = dynamic_cast<TGen::Engine::SceneNodeComponent *>(entity.getComponent("sceneNode"));
	
	
	setPosition(sceneNodeComponent->getSceneNode()->getLocalPosition());
}

void TGen::Engine::BodyComponent::setPosition(const TGen::Vector3 & position) {
	dBodySetPosition(bodyId, position.x, position.y, position.z);
}

TGen::Vector3 TGen::Engine::BodyComponent::getPosition() const {
	const dReal * vec = dBodyGetPosition(bodyId);
	
	return TGen::Vector3(vec[0], vec[1], vec[2]);
}

dBodyID TGen::Engine::BodyComponent::getBodyId() const {
	return bodyId;
}

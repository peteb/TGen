/*
 *  firstpersoncontroller.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 4/25/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "controller/firstpersoncontroller.h"
#include "scene/node.h"
#include "entity.h"
#include "gameinputmapper.h"
#include <tgen_renderer.h>

TGen::Engine::Controller::FirstPerson::FirstPerson(const std::string & name)
	: TGen::Engine::PlayerController(name)
	, node(NULL)
	, orientX(0.0f)
	, orientY(0.0f)
{
	
}

TGen::Engine::Controller::FirstPerson::~FirstPerson() {
	
}


void TGen::Engine::Controller::FirstPerson::linkLocally(TGen::Engine::Entity & entity) {
	TGen::Engine::PlayerController::linkLocally(entity);
	
	TGen::Engine::Scene::Node * playNode = dynamic_cast<TGen::Engine::Scene::Node *>(entity.getComponent("sceneNode"));
	// kanske pga arvet?
	
	// såhär: om man inte anger namn på en component så heter den samma som typen, dvs sceneNode. Det heter den i entitetens komponentlista
	//        men i subsystemet kan den heta något annat..... förmodligen entitetens namn
	
	// TODO: använd värdena camera och control i controller-component
	
	this->node = dynamic_cast<TGen::SceneNode *>(playNode->getSceneNode());
}

void TGen::Engine::Controller::FirstPerson::update(scalar dt) {
	if (!node)
		return;
	
	if (checkEvent(EventJump))
		node->setPosition(node->getLocalPosition() + node->getLocalOrientation().getY() * dt * 8.0);
	
	if (checkEvent(EventForward))
		node->setPosition(node->getLocalPosition() - node->getLocalOrientation().getZ() * dt * 8.0);

	if (checkEvent(EventBackward))
		node->setPosition(node->getLocalPosition() + node->getLocalOrientation().getZ() * dt * 8.0);

	if (checkEvent(EventStrafeLeft))
		node->setPosition(node->getLocalPosition() - node->getLocalOrientation().getX() * dt * 8.0);

	if (checkEvent(EventStrafeRight))
		node->setPosition(node->getLocalPosition() + node->getLocalOrientation().getX() * dt * 8.0);

	
	TGen::Vector3 viewDelta = checkViewDelta() * 0.002;

	orientX += viewDelta.x;
	orientY += viewDelta.y;
	
	
	orientY = TGen::Clamp(orientY, -TGen::HALF_PI, TGen::HALF_PI);
	
	TGen::Vector3 up(0.0f, 1.0f, 0.0f);
	TGen::Vector3 look(0.0f, 0.0f, -1.0f);
	TGen::Vector3 right(1.0f, 0.0f, 0.0f);
	
	TGen::Matrix4x4 rot = TGen::Matrix4x4::Rotation(right, TGen::Radian(orientY));
	
	look = rot * look;
	up = rot * up;
	right = TGen::Vector3::CrossProduct(look, up);
	
	rot = TGen::Matrix4x4::RotationY(TGen::Radian(orientX));
	
	look = rot * look;
	up = rot * up;
	
	right = TGen::Vector3::CrossProduct(look, up);
	
	
	TGen::Matrix4x4 rotation(right.getNormalized(), up.getNormalized(), look.getNormalized());
	
	node->setOrientation(rotation);
}


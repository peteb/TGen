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
#include "physics/body.h"
#include <tgen_renderer.h>

TGen::Engine::Controller::FirstPerson::FirstPerson(const std::string & name, const std::string & control, const std::string & view, bool usePhysics, scalar deltaPlane, scalar deltaJump)
	: TGen::Engine::PlayerController(name)
	, node(NULL)
	, viewNode(NULL)
	, controlBody(NULL)
	, orientX(0.0f)
	, orientY(0.0f)
	, control(control)
	, view(view)
	, usePhysics(usePhysics)
	, deltaPlane(deltaPlane)
	, deltaJump(deltaJump)
{
	
}

TGen::Engine::Controller::FirstPerson::~FirstPerson() {
	
}


void TGen::Engine::Controller::FirstPerson::linkLocally(TGen::Engine::Entity & entity) {
	TGen::Engine::PlayerController::linkLocally(entity);
	
	if (!usePhysics) {
		TGen::Engine::Scene::Node * playNode = dynamic_cast<TGen::Engine::Scene::Node *>(entity.getComponent(control));
		this->node = dynamic_cast<TGen::SceneNode *>(playNode->getSceneNode());
	}
	else {
		controlBody = dynamic_cast<TGen::Engine::Physics::Body *>(entity.getComponent(control));
	}
	
	TGen::Engine::Scene::Node * viewNode = dynamic_cast<TGen::Engine::Scene::Node *>(entity.getComponent(view));
	this->viewNode = dynamic_cast<TGen::SceneNode *>(viewNode->getSceneNode());


}

void TGen::Engine::Controller::FirstPerson::update(scalar dt) {
	TGen::Vector3 moveDelta;
	bool moveEvent = false;
	bool jump = false;
	
	if (checkEvent(EventJump)) {
		moveEvent = true;
		jump = true;
		//moveDelta += TGen::Vector3(0.0f, 1.0f, 0.0f);
	}
	
	if (checkEvent(EventForward)) {
		moveEvent = true;
		moveDelta += TGen::Vector3(0.0f, 0.0f, 1.0f);
	}
	
	if (checkEvent(EventBackward)) {
		moveEvent = true;
		moveDelta += TGen::Vector3(0.0f, 0.0f, -1.0f);
	}
	
	if (checkEvent(EventStrafeLeft)) {
		moveEvent = true;
		moveDelta += TGen::Vector3(-1.0f, 0.0f, 0.0f);
	}
	
	if (checkEvent(EventStrafeRight)) {
		moveEvent = true;
		moveDelta += TGen::Vector3(1.0f, 0.0f, 0.0f);
	}
	
	if (moveEvent) {
		moveDelta.normalize();
	
		if (viewNode) {
			TGen::Matrix4x4 rot = TGen::Matrix4x4::RotationY(TGen::Radian(orientX)); //viewNode->getLocalOrientation();
		//	rot.invert();
			
			moveDelta = rot * moveDelta;
			moveDelta.y = 0.0f;
			moveDelta.normalize();
			
			if (jump)
				moveDelta.y = deltaJump;
		}
		
		moveDelta *= dt;
	
		if (node && !usePhysics) {
			node->setPosition(node->getLocalPosition() + moveDelta * deltaPlane);
		}
		else if (controlBody) {
			std::cout << controlBody->getLinearVelocity().getMagnitude() << std::endl;
			
			//scalar maxSpeed = 10000.0;
			
			//if (controlBody->getLinearVelocity().getMagnitude() < 10.0)
			//controlBody->addForce(moveDelta * maxSpeed * (10.0 - controlBody->getLinearVelocity().getMagnitude()));
			controlBody->addForce(moveDelta * deltaPlane);
			
		}
		else {
			throw TGen::RuntimeException("Controller::FirstPerson::update", "no phys body");
		}
	}
	
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
	
	if (viewNode)
		viewNode->setOrientation(rotation);
}

void TGen::Engine::Controller::FirstPerson::setDeltaPlane(scalar speed) {
	deltaPlane = speed;
}

void TGen::Engine::Controller::FirstPerson::setDeltaJump(scalar speed) {
	deltaJump = speed;
}



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
#include "scene/equipmentnode.h"

#include "entity.h"
#include "entitylist.h"
#include "gameinputmapper.h"
#include "physics/body.h"
#include "componentinterfaces.h"

#include <tgen_renderer.h>

TGen::Engine::Controller::FirstPerson::FirstPerson(const std::string & name, scalar deltaPlane, scalar jumpForce, scalar jumpTime)
	: TGen::Engine::PlayerController(name)
	, node(NULL)
	, viewNode(NULL)
	, controlBody(NULL)
	, orientX(0.0f)
	, orientY(0.0f)
	, usePhysics(false)
	, deltaPlane(deltaPlane)
	, airTime(0.0)
	, jumpForce(jumpForce)
	, jumpTime(jumpTime)
	, airControl(0.7)
	, weapon(NULL)
	, primaryFire(false)
	, secondaryFire(false)
	, equipment(NULL)
{
	
}

TGen::Engine::Controller::FirstPerson::~FirstPerson() {
	
}


void TGen::Engine::Controller::FirstPerson::link(const TGen::Engine::ComponentLinker & linker) {
	TGen::Engine::PlayerController::link(linker);
	
	if (!usePhysics) {
		node = dynamic_cast<TGen::Engine::Scene::Node *>(linker.getComponent(controlName));
	}
	else {
		controlBody = dynamic_cast<TGen::Engine::Physics::Body *>(linker.getComponent(controlName));
	}
	
	TGen::Engine::Scene::Node * viewNode = dynamic_cast<TGen::Engine::Scene::Node *>(linker.getComponent(viewName));
	
	if (viewNode)
		this->viewNode = dynamic_cast<TGen::SceneNode *>(viewNode->getSceneNode());
	
	linkGlobally(linker);
}

void TGen::Engine::Controller::FirstPerson::linkGlobally(const TGen::Engine::ComponentLinker & linker) {
	if (!equipmentName.empty()) {
		TGen::Engine::Scene::Node * sceneNode = dynamic_cast<TGen::Engine::Scene::Node *>(linker.getComponent(equipmentName));
		if (sceneNode)
			equipment = dynamic_cast<TGen::Engine::Scene::EquipmentNode *>(sceneNode->getSceneNode());
	}
	
	if (!weaponName.empty()) {
		weapon = dynamic_cast<TGen::Engine::WeaponInterface *>(linker.getComponent(weaponName));
		
		if (!weapon) {
			TGen::Engine::Scene::Node * sceneNode = dynamic_cast<TGen::Engine::Scene::Node *>(linker.getComponent(weaponName));
			if (sceneNode)
				weapon = dynamic_cast<TGen::Engine::WeaponInterface *>(sceneNode->getSceneNode());
			
			if (!weapon)
				throw TGen::RuntimeException("FirstPerson::link", "failed to get weapon '" + weaponName + "'");
		}
	}
}

void TGen::Engine::Controller::FirstPerson::setWeaponLink(const std::string & weaponName) {
	this->weaponName = weaponName;
}

void TGen::Engine::Controller::FirstPerson::setEquipment(const std::string & equipmentName) {
	this->equipmentName = equipmentName;
}

void TGen::Engine::Controller::FirstPerson::setUsePhysics(bool usePhysics) {
	this->usePhysics = usePhysics;
}

void TGen::Engine::Controller::FirstPerson::setAirControl(scalar airControl) {
	this->airControl = airControl;
}

void TGen::Engine::Controller::FirstPerson::setView(const std::string & viewName) {
	this->viewName = viewName;
}

void TGen::Engine::Controller::FirstPerson::setControl(const std::string & controlName) {
	this->controlName = controlName;
}

void TGen::Engine::Controller::FirstPerson::update(scalar dt) {
	//std::cout << "SPEED: " << controlBody->getLinearVelocity().getMagnitude() << std::endl;
	
	TGen::Vector3 moveDelta;
	bool moveEvent = false;
	bool jump = false;
	bool initialJump = false;
	
	if (checkEvent(EventNextWeapon) && equipment)
		equipment->changeEquipmentRelative(-1);
	
	if (checkEvent(EventPreviousWeapon) && equipment)
		equipment->changeEquipmentRelative(1);
	
	static float sinceJumpPressed = 4.0f;		// TODO: fix this
	
	sinceJumpPressed += dt;
	
	if (isEventInitial(EventJump)) {
		setEventRead(EventJump);
		
		if (controlBody && controlBody->isOnFloor()) {
			airTime = jumpTime;
		
		//moveEvent = true;
	//	jump = true;
//		initialJump = true;
			sinceJumpPressed = 100.0;
			controlBody->addForce(TGen::Vector3(0.0f, jumpForce * 0.01, 0.0f));
		}
		else {
			sinceJumpPressed = 0.0;
		}
		//moveDelta += TGen::Vector3(0.0f, 1.0f, 0.0f);
	}
	else if (sinceJumpPressed < 0.1 && controlBody && controlBody->isOnFloor()) {
		controlBody->addForce(TGen::Vector3(0.0f, jumpForce * 0.01, 0.0f));
		sinceJumpPressed = 100.0;
	}
	
	/*else if (checkEvent(EventJump)) {
		
		moveEvent = true;
		jump = true;
	}
	
	if (jump)
		airTime -= dt * 1.0;
	else
		airTime -= dt * 1.3;
	*/
	//if (controlBody)
	//	std::cout << "speed: " << controlBody->getLinearVelocity().getMagnitude() << std::endl;
	
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
	
	if (checkEvent(EventPrimaryFire)) {
		if (!primaryFire) {
			primaryFire = true;
			
			if (weapon)
				weapon->beginFire(0);
		}
	}
	else {
		if (primaryFire) {
			primaryFire = false;
			
			if (weapon)
				weapon->endFire(0);
		}
	}
	
	//if (moveEvent) {
		moveDelta.normalize();
		
		if (viewNode) {
			TGen::Matrix4x4 rot = TGen::Matrix4x4::RotationY(TGen::Radian(orientX)); //viewNode->getLocalOrientation();
			
			if (!usePhysics) {
				rot = TGen::Matrix4x4(viewNode->getWorldOrientation());
				
				if (checkEvent(EventJump))
					moveDelta += TGen::Vector3(0.0f, 1.0f, 0.0f);
				
				if (checkEvent(EventCrouch))
					moveDelta += TGen::Vector3(0.0f, -1.0f, 0.0f);
				
				moveDelta.z = -moveDelta.z;
				moveDelta = rot * moveDelta;
			}
			else {
				moveDelta = rot * moveDelta;
				moveDelta.y = 0.0f;
			}

			moveDelta.normalize();
			
		//	rot.invert();
			
			
			//if (jump)
			//	moveDelta.y = deltaJump;
		}
	
		//moveDelta *= dt;
	
		if (node && !usePhysics) {
			node->getSceneNode()->setPosition(node->getSceneNode()->getLocalPosition() + moveDelta * dt * deltaPlane);
		}
		else if (controlBody) {

			//std::cout << controlBody->getLinearVelocity().getMagnitude() << std::endl;
			//if (controlBody->isOnFloor())
			//	std::cout << "on floor!" << std::endl;
			//scalar dp = controlBody->getSlope();
			
			//TGen::Degree slope(TGen::Radian(acos(dp)));

			//std::cout << slope.angle << std::endl;
			//std::cout << airTime << std::endl;
			
			TGen::Vector3 up = controlBody->getGroundNormal();

			up *= TGen::Vector3::DotProduct(up, TGen::Vector3(0.0f, 1.0f, 0.0f));
			// den här kan man göra en if (<0) up = -up
			
			TGen::Vector3 look(0.0f, 0.0f, 1.0f);
			TGen::Vector3 right(1.0f, 0.0f, 0.0f);
			look = TGen::Vector3::CrossProduct(right, up);
			right = TGen::Vector3::CrossProduct(up, look);
			
			TGen::Matrix4x4 mat(right, up, look);
			
			moveDelta = mat * moveDelta;
						
			scalar slope = TGen::Vector3::DotProduct(moveDelta, TGen::Vector3(0.0f, 1.0f, 0.0f));
			
			//moveDelta *= (1.0 + TGen::Vector3::DotProduct(moveDelta, TGen::Vector3(0.0f, 1.0f, 0.0f)) * 0.8);
			//moveDelta *= 1.0 + slope;
			if (slope < 0.0f)
				slope = 0.0f;
			
			moveDelta += TGen::Vector3(0.0f, slope, 0.0f);
			moveDelta *= (1.0 + slope) * dt;
			
			
			if (controlBody->isOnFloor()) {
				controlBody->addForce(moveDelta * deltaPlane);
				controlBody->setLinearDamping(0.12);
			}
			else {
				moveDelta.y = 0.0f;
				controlBody->setLinearDamping(0.0004);
				controlBody->addForce(moveDelta * deltaPlane * airControl);				
			}
			
			//if (jump) {
			scalar force = std::max(0.0f, float(airTime));
			//std::cout << force << std::endl;
				
			//if (force > 0.0)
			//	controlBody->addForce(TGen::Vector3(0.0f, jumpForce * force, 0.0f));
			//}
			
			//scalar maxSpeed = 10000.0;
			
			//if (controlBody->getLinearVelocity().getMagnitude() < 10.0)
			//controlBody->addForce(moveDelta * maxSpeed * (10.0 - controlBody->getLinearVelocity().getMagnitude()));
			
		}
		else {
			throw TGen::RuntimeException("Controller::FirstPerson::update", "no phys body, support should be fixed later");
		}
//	}
	
	TGen::Vector3 viewDelta = checkViewDelta() * 0.002;	// TODO: sensitivity = 0.002

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

TGen::Vector3 TGen::Engine::Controller::FirstPerson::getVelocity() const {
	if (controlBody)
		return controlBody->getVelocity();
	else if (node)
		return node->getVelocity();
	
	return TGen::Vector3::Zero;
}

TGen::Vector3 TGen::Engine::Controller::FirstPerson::getPosition() const {
	if (controlBody)
		return controlBody->getPosition();
	else if (node)
		return node->getPosition();
	
	return TGen::Vector3::Zero;
}

TGen::Rotation TGen::Engine::Controller::FirstPerson::getOrientation() const {
	if (controlBody)
		return controlBody->getOrientation();
	else if (node)
		return node->getOrientation();
	
	return TGen::Rotation::Identity;
}

void TGen::Engine::Controller::FirstPerson::setOrientation(const TGen::Rotation & orientation) {
	std::cout << "FP SET ORIENTATION" << std::endl;
}

void TGen::Engine::Controller::FirstPerson::trigger(TGen::Engine::TriggerContext & context, TriggerMode mode) {
	TGen::Engine::PlayerController::trigger(context, mode);
}


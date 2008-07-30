/*
 *  firemode.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/28/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "inventory/firemode.h"
#include "inventory/inventory.h"
#include "entitylist.h"
#include "entity.h"
#include "entityrecipe.h"
#include "componentinterfaces.h"
#include "component.h"
#include "componentlinker.h"

TGen::Engine::Inventory::FireMode::FireMode()
	: doFire(false)
	, timeSinceFire(100.0)
	, projectile(NULL)
	, spawnOrigin(NULL)
	, inventory(NULL)
{
}

void TGen::Engine::Inventory::FireMode::beginFire() {
	doFire = true;
	
	update(0.0);
}

void TGen::Engine::Inventory::FireMode::endFire() {
	doFire = false;
}

void TGen::Engine::Inventory::FireMode::update(scalar dt) {
	timeSinceFire += dt;
	
	if (doFire && timeSinceFire >= frequency) {
		fire();
		timeSinceFire = 0.0;
	}	
}

void TGen::Engine::Inventory::FireMode::link(const TGen::Engine::ComponentLinker & linker) {
	if (linker.getEntityList())
		projectile = linker.getEntityList()->getPrototype(projectileName, std::nothrow);
	
	spawnOrigin = dynamic_cast<TGen::Engine::WorldObject *>(linker.getComponent(originName));
	inventory = dynamic_cast<TGen::Engine::Inventory::Inventory *>(linker.getComponent("inventory"));
}

void TGen::Engine::Inventory::FireMode::fire() {
	std::cout << "FIRE" << std::endl;
	
	int ammoCount = -1;
	
	if (inventory)
		ammoCount = inventory->getItemValue(requiresName);
	
	if (ammoCount == 0) {
		std::cout << "OUT OF AMMO" << std::endl;
		return;
	}
	else if (inventory && ammoCount > 0) {
		inventory->addItemValue(requiresName, -1);
	}
	
	TGen::Engine::Entity * newEntity = projectile->createEntity();
	TGen::Engine::WorldObject & object = dynamic_cast<TGen::Engine::WorldObject &>(newEntity->getComponent(projectile->getWorldInterfaceIndex()));
	
	if (!spawnOrigin) {
		std::cout << "failed to create object or spawn it at origin spawn" << std::endl;
		return;
	}
	
	TGen::Vector2 randDir(scalar(rand()) / RAND_MAX, scalar(rand()) / RAND_MAX);
	randDir *= spread;
	randDir -= spread / 2.0f;
	
	TGen::Rotation direction = spawnOrigin->getOrientation();
	direction *= TGen::Rotation::RotationY(TGen::Degree(randDir.x));
	direction *= TGen::Rotation::RotationZ(TGen::Degree(randDir.y));
	
	object.setPosition(spawnOrigin->getPosition());
	object.setOrientation(direction);
}

void TGen::Engine::Inventory::FireMode::setOrigin(const std::string & originName) {
	this->originName = originName;
}

void TGen::Engine::Inventory::FireMode::setProjectile(const std::string & projectileName) {
	this->projectileName = projectileName;
}

void TGen::Engine::Inventory::FireMode::setRequires(const std::string & requiresName) {
	this->requiresName = requiresName;
}

void TGen::Engine::Inventory::FireMode::setRate(scalar rate) {
	this->frequency = 1.0 / rate;
}

void TGen::Engine::Inventory::FireMode::setSpread(const TGen::Vector2 & spread) {
	this->spread = spread;
}



/*
 *  weapon.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/13/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "weapon.h"
#include "entitylist.h"
#include "entity.h"
#include "entityrecipe.h"

TGen::Engine::Inventory::Weapon::Weapon(const std::string & name)
	: TGen::Engine::Component(name)
	, prototype(NULL)
	, ammoSpawn(NULL)
{
}

void TGen::Engine::Inventory::Weapon::beginFire(int mode) {
	// TEST cloning of prototype
	
	std::cout << "FIRE" << std::endl;
	
	if (prototype) {
		std::cout << "HAS PROTOTYPE" << std::endl;
		
		TGen::Engine::Entity * newEntity = prototype->createEntity();
		TGen::Engine::WorldObject * object = dynamic_cast<TGen::Engine::WorldObject *>(newEntity->getComponent(prototype->getWorldInterface()));
		
		if (!object)
			throw TGen::RuntimeException("blabla", "blaaaaaaa");
		
		if (!ammoSpawn)
			throw TGen::RuntimeException("blaaa", "no ammo spawn defined");
		
		object->setPosition(ammoSpawn->getPosition());
		object->setOrientation(ammoSpawn->getOrientation());
	}
	else {
		std::cout << "NO PROTOTYPE" << std::endl;
	}
	
}

void TGen::Engine::Inventory::Weapon::endFire(int mode) {

}

void TGen::Engine::Inventory::Weapon::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	prototype = entities.getPrototype("bullet");
	ammoSpawn = dynamic_cast<TGen::Engine::WorldObject *>(entities.getComponent(ammoSpawnName, entity));
}

void TGen::Engine::Inventory::Weapon::setAmmoSpawn(const std::string & ammoSpawnName) {
	this->ammoSpawnName = ammoSpawnName;
}

/*
 *  weapon.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/13/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "weapon.h"

#include "inventory/firemode.h"

TGen::Engine::Inventory::Weapon::Weapon(const std::string & name)
	: TGen::Engine::Component(name)
{
	fireModes.reserve(10);
}

TGen::Engine::Inventory::Weapon::~Weapon() {
	for (int i = 0; i < fireModes.size(); ++i)
		delete fireModes[i];
}

void TGen::Engine::Inventory::Weapon::beginFire(int mode) {
	if (fireModes.size() <= mode) {
		std::cout << "can't fire, no firing mode" << std::endl;
	}
	else {
		if (fireModes[mode])
			fireModes[mode]->beginFire();
		else
			std::cout << "can't fire, firing mode not set" << std::endl;
	}
	
}

void TGen::Engine::Inventory::Weapon::endFire(int mode) {
	if (fireModes.size() <= mode) {
		std::cout << "can't fire, no firing mode" << std::endl;
	}
	else {
		if (fireModes[mode])
			fireModes[mode]->endFire();
		else
			std::cout << "can't fire, firing mode not set" << std::endl;
	}
}

void TGen::Engine::Inventory::Weapon::update(scalar dt) {
	for (int i = 0; i < fireModes.size(); ++i) {
		if (fireModes[i])
			fireModes[i]->update(dt);
	}
}

void TGen::Engine::Inventory::Weapon::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	for (int i = 0; i < fireModes.size(); ++i) {
		if (fireModes[i])
			fireModes[i]->linkGlobally(entities, entity);
	}
}

void TGen::Engine::Inventory::Weapon::setFireMode(int num, TGen::Engine::Inventory::FireMode * mode) {
	if (num >= fireModes.size()) {
		for (int i = fireModes.size(); i <= num; ++i)
			fireModes.push_back(NULL);
		
		fireModes.at(num) = mode;
	}
	else {
		if (fireModes.at(num))
			throw TGen::RuntimeException("Inventory::Weapon::setFireMode", "fire mode already set");
		
		fireModes.at(num) = mode;
	}
}

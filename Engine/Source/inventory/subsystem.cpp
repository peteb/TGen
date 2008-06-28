/*
 *  subsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "inventory/subsystem.h"
#include "inventory/inventory.h"
#include "inventory/weapon.h"
#include "inventory/firemode.h"

TGen::Engine::Inventory::Subsystem::Subsystem() {
	
}

TGen::Engine::Inventory::Subsystem::~Subsystem() {
	
}

TGen::Engine::Component * TGen::Engine::Inventory::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	TGen::Engine::Component * ret = NULL;
	
	std::string type = properties.getName();
	
	if (type == "inventory") {
		std::auto_ptr<TGen::Engine::Inventory::Inventory> newInventory(new TGen::Engine::Inventory::Inventory(name));
		
		for (int i = 0; i < properties.getNumNodes(); ++i) {
			TGen::Engine::Inventory::Item newItem = createItem(properties.getNode(i));
			newInventory->addItem(properties.getNode(i).getName(), newItem);
		}
		
		ret = newInventory.release();
	}
	else if (type == "weapon") {
		TGen::Engine::Inventory::Weapon * newWeapon = new TGen::Engine::Inventory::Weapon(name);
		
		for (int i = 0; i < properties.getNumNodes(); ++i) {
			const TGen::PropertyTree & node = properties.getNode(i);
			
			if (node.getName() == "fireMode") {
				int modeNum = TGen::lexical_cast<int>(node.getAttribute(0));
				
				std::cout << "FIRE MODE " << modeNum << std::endl;
				
				TGen::Engine::Inventory::FireMode * newFireMode = createFireMode(node);
				newWeapon->setFireMode(modeNum, newFireMode);
			}
		}
		
	//	exit(1);
		weapons.push_back(newWeapon);
		
		ret = newWeapon;
	}
	else {
		throw TGen::RuntimeException("Inventory::Subsystem::createComponent", "invalid component type: '" + name + "'");
	}

	return ret;
}

TGen::Engine::Inventory::FireMode * TGen::Engine::Inventory::Subsystem::createFireMode(const TGen::PropertyTree & properties) {
	TGen::Engine::Inventory::FireMode * newFireMode = new TGen::Engine::Inventory::FireMode;

	newFireMode->setOrigin(properties.getProperty("origin", ""));
	newFireMode->setProjectile(properties.getProperty("projectile", ""));
	newFireMode->setRequires(properties.getProperty("requires", ""));
	newFireMode->setRate(TGen::lexical_cast<scalar>(properties.getProperty("rate", "1.0")));
	
	return newFireMode;
}

TGen::Engine::Inventory::Item TGen::Engine::Inventory::Subsystem::createItem(const TGen::PropertyTree & properties) {
	TGen::Engine::Inventory::Item ret;
	
	ret.value = TGen::lexical_cast<int>(properties.getProperty("value", "0"));
	ret.maxValue = TGen::lexical_cast<int>(properties.getProperty("max", "1"));
	
	return ret;
}

void TGen::Engine::Inventory::Subsystem::update(scalar dt) {
	for (int i = 0; i < weapons.size(); ++i)
		weapons[i]->update(dt);
}

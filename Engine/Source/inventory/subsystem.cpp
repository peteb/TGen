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

TGen::Engine::Inventory::Subsystem::Subsystem() {
	
}

TGen::Engine::Inventory::Subsystem::~Subsystem() {
	
}

TGen::Engine::Component * TGen::Engine::Inventory::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	TGen::Engine::Component * ret = NULL;
	
	if (name == "inventory") {
		std::auto_ptr<TGen::Engine::Inventory::Inventory> newInventory(new TGen::Engine::Inventory::Inventory(entityName));
		
		for (int i = 0; i < properties.getNumNodes(); ++i) {
			TGen::Engine::Inventory::Item newItem = createItem(properties.getNode(i));
			newInventory->addItem(properties.getNode(i).getName(), newItem);
		}
		
		ret = newInventory.release();
	}
	else {
		throw TGen::RuntimeException("Inventory::Subsystem::createComponent", "invalid component type: '" + name + "'");
	}

	return ret;
}

TGen::Engine::Inventory::Item TGen::Engine::Inventory::Subsystem::createItem(const TGen::PropertyTree & properties) {
	TGen::Engine::Inventory::Item ret;
	
	ret.value = TGen::lexical_cast<int>(properties.getProperty("value", "0"));
	ret.maxValue = TGen::lexical_cast<int>(properties.getProperty("max", "1"));
	
	return ret;
}

/*
 *  subsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "inventory/subsystem.h"

TGen::Engine::Inventory::Subsystem::Subsystem() {
	
}

TGen::Engine::Inventory::Subsystem::~Subsystem() {
	
}

TGen::Engine::Component * TGen::Engine::Inventory::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	
	return NULL;
}

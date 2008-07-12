/*
 *  itemvalueop.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "itemvalueop.h"
#include "inventory/inventory.h"
#include "entity.h"

TGen::Engine::Script::ItemValueOperation::ItemValueOperation() 
	: item(NULL)
	, inventory(NULL)
{
	
}


void TGen::Engine::Script::ItemValueOperation::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	TGen::Engine::Script::EventOperation::linkGlobally(entities, entity);
	
	inventory = dynamic_cast<TGen::Engine::Inventory::Inventory *>(&entity.getComponent("inventory"));
	if (!inventory)
		throw TGen::RuntimeException("Script::ItemValueOperation::linkGlobally", "no inventory in entity!");

	item = inventory->getItem(itemName);
}


void TGen::Engine::Script::ItemValueOperation::setItem(const std::string & item) {
	this->itemName = item;
}

void TGen::Engine::Script::ItemValueOperation::setParameter(int para) {
	this->parameter = para;
}

void TGen::Engine::Script::ItemValueOperation::trigger(void ** argv, int argc) {
	scalar value = *reinterpret_cast<scalar *>(argv[parameter]);
	
	if (item) {
		std::cout << "INCREASE WITH " << value << std::endl;
	
		inventory->increaseValue(item, value);
	}
}


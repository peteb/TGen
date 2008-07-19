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

void TGen::Engine::Script::ItemValueOperation::setRegister(int regId) {
	this->regId = regId;
}

void TGen::Engine::Script::ItemValueOperation::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	if (item) {
		if (!load) {
			if (!intOp) {
				scalar value = *context.getRegister<scalar *>(regId);
		
				if (relative)
					inventory->increaseValue(item, value);
				else
					inventory->setValue(item, value);
			}
			else {
				int value = *context.getRegister<int *>(regId);
			
				if (relative)
					inventory->increaseValue(item, value);
				else
					inventory->setValue(item, value);		
			}
		}
		else {
			if (!intOp)
				*context.getRegister<scalar *>(regId) = item->value;
			else
				*context.getRegister<int *>(regId) = item->value;				
		}
	}
}

void TGen::Engine::Script::ItemValueOperation::setLoad(bool load) {
	this->load = load;
}

void TGen::Engine::Script::ItemValueOperation::setIntOp(bool intOp) {
	this->intOp = intOp;
}

void TGen::Engine::Script::ItemValueOperation::setRelative(bool relative) {
	this->relative = relative;
}

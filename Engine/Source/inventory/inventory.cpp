/*
 *  inventory.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include <tgen_core.h>
#include "inventory/inventory.h"

TGen::Engine::Inventory::Inventory::Inventory(const std::string & name)
	: TGen::Engine::Component(name)
{

}

TGen::Engine::Inventory::Inventory::~Inventory() {
	for (ItemMap::iterator iter = items.begin(); iter != items.end(); ++iter)
		delete iter->second;
}

void TGen::Engine::Inventory::Inventory::addItem(const std::string & name, TGen::Engine::Inventory::Item * item) {
	if (items.find(name) != items.end())
		throw TGen::RuntimeException("Inventory::addItem", "item '" + name + "' already defined");
	
	items.insert(std::make_pair(name, item));
}

int TGen::Engine::Inventory::Inventory::getItemValue(const std::string & name) {
	ItemMap::iterator iter = items.find(name);
	if (iter != items.end())
		return iter->second->value;
	
	return 0;
}

void TGen::Engine::Inventory::Inventory::addItemValue(const std::string & name, int value) {
	ItemMap::iterator iter = items.find(name);
	if (iter == items.end())
		throw TGen::RuntimeException("Inventory::addItemValue", "item '" + name + "' not defined!");
	
	iter->second->value += value;
}

TGen::Engine::Inventory::Item * TGen::Engine::Inventory::Inventory::getItem(const std::string & name) {
	ItemMap::iterator iter = items.find(name);
	if (iter == items.end())
		throw TGen::RuntimeException("Inventory::addItemValue", "item '" + name + "' not defined!");

	return iter->second;
}

void TGen::Engine::Inventory::Inventory::increaseValue(TGen::Engine::Inventory::Item * item, int value) {
	if (!item)
		return;
	
	int preValue = item->value;
	item->increaseValue(value);
	int postValue = item->value;
	
	std::cout << "PRE: " << preValue << " POST: " << postValue << std::endl;
}

void TGen::Engine::Inventory::Inventory::setValue(TGen::Engine::Inventory::Item * item, int value) {
	if (!item)
		return;
	
	int preValue = item->value;
	item->setValue(value);
	int postValue = item->value;
	
	std::cout << "PRE: " << preValue << " POST: " << postValue << std::endl;	
}

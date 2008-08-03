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

// TODO: rename inventory -> itempool

#define INVENTORY TGen::Engine::Inventory::Inventory

TGen::Engine::Symbol INVENTORY::symbolSetItemValue = TGen::Engine::getUniqueSymbol("setItemValue:");
TGen::Engine::Symbol INVENTORY::symbolGetItemValue = TGen::Engine::getUniqueSymbol("getItemValue");
TGen::Engine::Symbol INVENTORY::symbolIncreaseItemValue = TGen::Engine::getUniqueSymbol("increaseItem:with:");


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
	
	itemSymbols.insert(std::make_pair(TGen::Engine::getUniqueSymbol(name), item));
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

void TGen::Engine::Inventory::Inventory::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	TGen::Engine::Symbol symbolNum = context.getFunctionSymbol();
	
	// component.getOwner, if (owner == this || owner == null) delete;
	// prototype med script skapar eventsen i den prototypen, men sätter owner till this. owner default är NULL
	// initialize körs INTE på prototyp
	/*
		En LinkRef-klass borde finnas, templateas. LinkRef<PhysGeom> linkRef; linkRef.setTarget("string"); linkRef.setTarget(1);
		Nja...... men ComponentLink! DET SKA FINNAS, templatead
	 
	 */
	
	if (symbolNum == symbolSetItemValue) {
		TGen::Engine::Symbol itemSymbol = *context.getParameter<TGen::Engine::Symbol *>(0);
		int itemValue = *context.getParameter<scalar *>(1);
		
		ItemSymbolMap::iterator iter = itemSymbols.find(itemSymbol);
		if (iter != itemSymbols.end())
			iter->second->setValue(itemValue);
		else
			context.setRegister<int>(0, -1);
	}
	else if (symbolNum == symbolGetItemValue) {
		TGen::Engine::Symbol itemSymbol = *context.getParameter<TGen::Engine::Symbol *>(0);
		
		ItemSymbolMap::iterator iter = itemSymbols.find(itemSymbol);
		if (iter != itemSymbols.end())
			context.setRegister<scalar>(context.getReturnRegister(), scalar(iter->second->value));
		else
			context.setRegister<int>(0, -1);
	}
	else if (symbolNum == symbolIncreaseItemValue) {
		TGen::Engine::Symbol itemSymbol = *context.getParameter<TGen::Engine::Symbol *>(0);
		int itemValue = *context.getParameter<scalar *>(1);

		ItemSymbolMap::iterator iter = itemSymbols.find(itemSymbol);
		if (iter != itemSymbols.end())
			iter->second->increaseValue(itemValue);
		else
			context.setRegister<int>(0, -1);		
	}
}


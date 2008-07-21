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
#include "script/subsystem.h"

TGen::Engine::Inventory::Inventory::Inventory(const std::string & name)
	: TGen::Engine::Component(name)
	, setItemValueSymbol(-1)
	, getItemValueSymbol(-1)
	, increaseItemValueSymbol(-1)
{

	setItemValueSymbol = TGen::Engine::Script::Subsystem::symbols["setItemValue"];
	getItemValueSymbol = TGen::Engine::Script::Subsystem::symbols["getItemValue"];
	increaseItemValueSymbol = TGen::Engine::Script::Subsystem::symbols["increaseItemValue"];
}

TGen::Engine::Inventory::Inventory::~Inventory() {
	for (ItemMap::iterator iter = items.begin(); iter != items.end(); ++iter)
		delete iter->second;
}

void TGen::Engine::Inventory::Inventory::addItem(const std::string & name, TGen::Engine::Inventory::Item * item) {
	if (items.find(name) != items.end())
		throw TGen::RuntimeException("Inventory::addItem", "item '" + name + "' already defined");
	
	itemSymbols.insert(std::make_pair(TGen::Engine::Script::Subsystem::symbols[name], item));
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
	int symbolNum = context.getFunctionSymbol();
	
	// component.getOwner, if (owner == this || owner == null) delete;
	// prototype med script skapar eventsen i den prototypen, men sätter owner till this. owner default är NULL
	// initialize körs INTE på prototyp
	/*
		En LinkRef-klass borde finnas, templateas. LinkRef<PhysGeom> linkRef; linkRef.setTarget("string"); linkRef.setTarget(1);
		Nja...... men ComponentLink! DET SKA FINNAS, templatead
	 
	 */
	
	if (symbolNum == setItemValueSymbol) {
		int itemSymbol = *context.getRegister<int *>(2);
		int itemValue = *context.getRegister<scalar *>(3);
		
		ItemSymbolMap::iterator iter = itemSymbols.find(itemSymbol);
		if (iter != itemSymbols.end())
			iter->second->setValue(itemValue);
		else
			context.setRegister<int>(0, -1);
	}
	else if (symbolNum == getItemValueSymbol) {
		int itemSymbol = *context.getRegister<int *>(2);
		
		ItemSymbolMap::iterator iter = itemSymbols.find(itemSymbol);
		if (iter != itemSymbols.end())
			context.setRegister<scalar>(context.getReturnRegister(), scalar(iter->second->value));
		else
			context.setRegister<int>(0, -1);
	}
	else if (symbolNum == increaseItemValueSymbol) {
		int itemSymbol = *context.getRegister<int *>(2);
		int itemValue = *context.getRegister<scalar *>(3);

		ItemSymbolMap::iterator iter = itemSymbols.find(itemSymbol);
		if (iter != itemSymbols.end())
			iter->second->increaseValue(itemValue);
		else
			context.setRegister<int>(0, -1);		
	}
}


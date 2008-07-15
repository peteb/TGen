/*
 *  entity.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "entity.h"
#include "component.h"
#include "script/subsystem.h"
#include <tgen_core.h>

TGen::Engine::Entity::Entity(const std::string & name)
	: name(name)
	, getComponentSymbol(-1)
	, initializer(NULL)
	, dispatcher(NULL)
{
	context.setRegister(TGen::Engine::RegisterSelf, this);
	getComponentSymbol = TGen::Engine::Script::Subsystem::symbols["getComponent"];
}


TGen::Engine::Entity::~Entity() {
	for (int i = 0; i < components.size(); ++i)
		delete components[i];
}


void TGen::Engine::Entity::linkLocally() {
	for (ComponentMap::iterator iter = componentLookup.begin(); iter != componentLookup.end(); ++iter)
		iter->second->linkLocally(*this);
}


void TGen::Engine::Entity::linkGlobally(TGen::Engine::EntityList & entities) {
	for (ComponentMap::iterator iter = componentLookup.begin(); iter != componentLookup.end(); ++iter)
		iter->second->linkGlobally(entities, *this);
	
	if (initializer)
		initializer->trigger(context);
}


void TGen::Engine::Entity::trigger(TGen::Engine::TriggerContext & context) {
	int symbolId = context.getFunctionSymbol();
	
	if (symbolId == -1)
		return;
	
	if (symbolId == getComponentSymbol) {
		triggerGetComponent(context);
	}
	else {		
		EventMap::iterator iter = events.find(symbolId);
		if (iter == events.end()) {
			if (dispatcher) {
				dispatcher->trigger(context);
				
				if (context.getFunctionSymbol() != -1) {
					iter = events.find(symbolId);
					
					if (iter == events.end())
						throw TGen::RuntimeException("Entity::trigger", "Dispatch failure; unknown symbol.");	
					else	
						iter->second->trigger(context);
				}
			}
			else {
				throw TGen::RuntimeException("Entity::trigger", "Method not found.");	
			}
		}
		else {	
			iter->second->trigger(context);
		}
	}
}


void TGen::Engine::Entity::triggerGetComponent(TGen::Engine::TriggerContext & context) {
	int componentSymbol = *context.getRegister<int *>(2);
	
	ComponentSymbolMap::iterator iter = componentSymbols.find(componentSymbol);
	if (iter == componentSymbols.end())
		throw TGen::RuntimeException("Entity::triggerGetComponent", "component not found");
	
	int returnReg = context.getReturnRegister();
	context.setRegister<TGen::Engine::Triggerable *>(returnReg, dynamic_cast<TGen::Engine::Triggerable *>(iter->second));
}


void TGen::Engine::Entity::registerEvent(int symbolId, TGen::Engine::Triggerable * event) {
	if (symbolId == -2)
		initializer = event;
	else if (symbolId == -3)
		dispatcher = event;
	else
		events.insert(std::make_pair(symbolId, event));
}


void TGen::Engine::Entity::addComponent(TGen::Engine::Component * component, const std::string & name) {
	std::cout << "ADDCOMP " << name << " to " << this->name << std::endl;
	componentLookup.insert(ComponentMap::value_type(name, component));
	components.push_back(component);
	
	int symbolId = TGen::Engine::Script::Subsystem::symbols[name];
	componentSymbols.insert(std::make_pair(symbolId, component));
}
	

TGen::Engine::Component * TGen::Engine::Entity::getComponent(const std::string & name, std::nothrow_t noth) {
	ComponentMap::iterator iter = componentLookup.find(name);
	
	if (iter == componentLookup.end())
		throw TGen::RuntimeException("Entity::getComponent", "no component called '" + name + "' loaded!");

	return iter->second;
}


TGen::Engine::Component & TGen::Engine::Entity::getComponent(const std::string & name) {
	ComponentMap::iterator iter = componentLookup.find(name);
	if (iter == componentLookup.end())
		throw TGen::RuntimeException("Entity::getComponent", "no component called '" + name + "' loaded!");
	
	assert(iter->second);
	
	return *iter->second;
}


TGen::Engine::Component * TGen::Engine::Entity::getComponent(int index, std::nothrow_t noth) {
	if (index < 0 || index >= components.size())
		return NULL;
	
	return components[index];
}


TGen::Engine::Component & TGen::Engine::Entity::getComponent(int index) {
	TGen::Engine::Component * ret = components.at(index);
	if (!ret)
		throw TGen::RuntimeException("Entity::getComponent(index)", "NULL component");
	
	return *ret;
}


const std::string & TGen::Engine::Entity::getName() const {
	return name;
}


TGen::Engine::TriggerContext & TGen::Engine::Entity::getContext() {
	return context;
}



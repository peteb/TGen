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
#include "componentlinker.h"
#include <tgen_core.h>

TGen::Engine::Symbol TGen::Engine::Entity::symbolGetComponent = TGen::Engine::getUniqueSymbol("getComponent:");
TGen::Engine::Symbol TGen::Engine::Entity::symbolRespondsTo = TGen::Engine::getUniqueSymbol("respondsTo:");
TGen::Engine::Symbol TGen::Engine::Entity::symbolInit = TGen::Engine::getUniqueSymbol("init");
TGen::Engine::Symbol TGen::Engine::Entity::symbolGetWorldInterface = TGen::Engine::getUniqueSymbol("getWorldInterface");


TGen::Engine::Entity::Entity(const std::string & name)
	: name(name)
	, initializer(NULL)
	, dispatcher(NULL)
	, worldInterface(NULL)
{
	context.setRegister(TGen::Engine::RegisterSelf, this);
}


TGen::Engine::Entity::~Entity() {
	for (int i = 0; i < components.size(); ++i) {
		if (components[i]->isStatic() == false)
			delete components[i];
	}
}


void TGen::Engine::Entity::link(const TGen::Engine::ComponentLinker & linker) {
	TGen::Engine::ComponentLinker newLinker(linker);
	newLinker.setEntity(this);
	
	for (ComponentMap::iterator iter = componentLookup.begin(); iter != componentLookup.end(); ++iter) {
		try {
			iter->second->link(newLinker);
		} catch (const TGen::RuntimeException & err) {
			throw TGen::RuntimeException("Entity::link", "Failed to link component '" + getName() + ":" + iter->second->getName() + "':\n" + err.getWhere() + ": " + err.getDescription());
		}
	}
	
	initialize();
}

void TGen::Engine::Entity::initialize() {
	if (initializer)
		initializer->trigger(context, TGen::Engine::TriggerPrecise);	
	else
		std::cout << "WARNING: entity has no initializer" << std::endl;
}


void TGen::Engine::Entity::setWorldInterface(TGen::Engine::WorldObject * worldInterface) {
	this->worldInterface = worldInterface;
}


void TGen::Engine::Entity::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	TGen::Engine::Symbol symbolId = context.getFunctionSymbol();
	
	std::cout << "Entity " << getName() << " got message " << symbolId << std::endl;
	
	if (symbolId == -1)
		return;
	
	if (symbolId == symbolGetComponent) {
		triggerGetComponent(context);
	}
	else if (symbolId == symbolInit) {
		initialize();
	}
	else if (symbolId == symbolRespondsTo) {
		triggerRespondsTo(context);
	}
	else if (symbolId == symbolGetWorldInterface) {
		context.setReturn(worldInterface);
	}
	else {
		if (mode == TGen::Engine::TriggerPrecise) {
			EventMap::iterator iter = events.find(symbolId);
			if (iter == events.end()) {
				if (dispatcher) {
					dispatcher->trigger(context, mode);
					
					if (context.getFunctionSymbol() != -1) {
						iter = events.find(symbolId);
						
						if (iter == events.end())
							throw TGen::RuntimeException("Entity::trigger", "Dispatch failure; unknown symbol.");	
						else	
							iter->second->trigger(context, mode);
					}
				}
				else {
					std::cout << "NUM EVENTS: " << events.size() << std::endl;
					
					for (EventMap::iterator iter = events.begin(); iter != events.end(); ++iter) {
						std::cout << getSymbolName(iter->first) << std::endl;
					}
					
					throw TGen::RuntimeException("Entity::trigger", "Message symbol not defined: \"") << TGen::Engine::getSymbolName(symbolId) << "\"";	
				}
			}
			else {	
				iter->second->trigger(context, mode);
			}
		}
		else if (mode == TGen::Engine::TriggerFirst || mode == TGen::Engine::TriggerAll) {
			for (int i = 0; i < components.size(); ++i) {
				components[i]->trigger(context, mode);
				
				if (context.getFunctionSymbol() != -1) {
					if (mode == TGen::Engine::TriggerFirst)
						break;
				}
				else {
					context.setRegister<int>(0, symbolId);		// reset r0 to the correct symbol
				}
			}
		}
	}
}


void TGen::Engine::Entity::triggerGetComponent(TGen::Engine::TriggerContext & context) {
	int componentSymbol = context.getRegister<int>(2);
	int returnReg = context.getReturnRegister();
	
	ComponentSymbolMap::iterator iter = componentSymbols.find(componentSymbol);
	if (iter == componentSymbols.end()) {
		context.setRegister<TGen::Engine::Triggerable *>(returnReg, NULL);		
		return;
	}
	
	//	throw TGen::RuntimeException("Entity::triggerGetComponent", "component not found");
	
	context.setRegister<TGen::Engine::Triggerable *>(returnReg, dynamic_cast<TGen::Engine::Triggerable *>(iter->second));
}

void TGen::Engine::Entity::triggerRespondsTo(TGen::Engine::TriggerContext & context) {
	int selector = context.getParameter<int>(0);
	int returnReg = context.getReturnRegister();
	
	EventMap::iterator iter = events.find(selector);
	context.setRegister<int>(returnReg, (iter == events.end()));
}


void TGen::Engine::Entity::registerEvent(TGen::Engine::Symbol symbolId, TGen::Engine::Triggerable * event) {
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
	
	TGen::Engine::Symbol symbolId = TGen::Engine::getUniqueSymbol(name);
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



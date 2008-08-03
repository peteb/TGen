/*
 *  callop.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/13/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "script/callop.h"
#include "entitylist.h"
#include "triggerable.h"
#include "component.h"
#include "entity.h"
#include "componentlinker.h"

TGen::Engine::Script::CallOperation::CallOperation(TGen::Engine::Script::EventOperation * parent)
	: TGen::Engine::Script::EventOperation(parent)
	, event(NULL)
	, registerId(-1)
	, offset(-1)
	, triggerMode(TGen::Engine::TriggerPrecise)
	, numParameters(0)
	, componentId(1)
{
}

void TGen::Engine::Script::CallOperation::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	TGen::Engine::Triggerable * callee = event;
	
	if (!event && registerId != -1) {
		callee = *context.getRegister<TGen::Engine::Triggerable **>(registerId);
	}
	else if (componentId != -1 && !event) {
		TGen::Engine::Entity * self = *context.getRegister<TGen::Engine::Entity **>(TGen::Engine::RegisterSelf);
		std::cerr << "SELF IS " << self->getName() << std::endl;
		
		TGen::Engine::Component * component = comp.get(); //self->getComponent(componentId, std::nothrow);
		//std::cout << "COMPONENT " << componentId << " IS " << component->getName() << std::endl;
		
		callee = component;
		//exit(1);
	}
	
	if (callee) {
		//if (offset != -1)
			context.setRegister<int>(0, offset);
		
		context.numParameters = numParameters;
		
		if (shareContext || !callee->context) {
			callee->trigger(context, triggerMode);
		}
		else {
			callee->trigger(*callee->context, triggerMode);
		}
	}
	else {
		throw TGen::RuntimeException("Script::CallOperation::trigger", "failed to send message, receiver unknown. Event: ") << event << " register: " << registerId;
		
	}
}

void TGen::Engine::Script::CallOperation::link(const TGen::Engine::ComponentLinker & linker) {
	if (!entityName.empty()) {
		std::string fixedComponentName = entityName;
		
		if (eventName.empty() && linker.getEntityList())
			event = dynamic_cast<TGen::Engine::Triggerable *>(linker.getEntity(entityName));
		else
			event = dynamic_cast<TGen::Engine::Triggerable *>(linker.getComponent(entityName + ":" + eventName));
		
		if (linker.getEntity(entityName))
			std::cout << "NENT: " << linker.getEntity(entityName)->getName() << std::endl;
		
		if (!event) {
			//asm("int $3");
			//throw TGen::RuntimeException("CallOperation::link", "failed to get entity '" + entityName + "', event: '" + eventName + "', entity list: ") << linker.getEntityList();
		}
	}
	else if (!eventName.empty() && registerId == -1) {
		event = dynamic_cast<TGen::Engine::Triggerable *>(linker.getComponent(eventName));

		if (!event) {
			//comp.linkComponentIndex(linker);
			
			//std::cout << linker.getEntity() << " " << linker.getEntityList() << std::endl;
			
			//throw TGen::RuntimeException("CallOperation::link", "failed to get triggerable: " + eventName);
		}
	}	
		
	
	// UnaryDelegate för entity
	comp.link(linker);
}

void TGen::Engine::Script::CallOperation::linkRecipe(const TGen::Engine::EntityRecipe & recipe) {
	comp.linkComponentIndex(recipe);
}

void TGen::Engine::Script::CallOperation::setEvent(const std::string & eventName) {
	this->eventName = eventName;
	comp.set(eventName);
}

void TGen::Engine::Script::CallOperation::setNumParameters(int numParams) {
	numParameters = numParams;
}

void TGen::Engine::Script::CallOperation::setEvent(int regId) {
	this->registerId = regId;
}

void TGen::Engine::Script::CallOperation::setShareContext(bool shareContext) {
	this->shareContext = shareContext;
}

void TGen::Engine::Script::CallOperation::setOffset(int offset) {
	this->offset = offset;
}

void TGen::Engine::Script::CallOperation::setTriggerMode(TGen::Engine::TriggerMode triggerMode) {
	this->triggerMode = triggerMode;
}

void TGen::Engine::Script::CallOperation::setEntity(const std::string & entityName) {
	this->entityName = entityName;
}


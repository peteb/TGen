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
	: TGen::Engine::Script::EventOperation("CALL", parent)
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
	
	if (!event) {
		if (registerId != -1) {		// pointer to event is in register registerId
			callee = *context.getRegister<TGen::Engine::Triggerable **>(registerId);
		}
		else if (componentId != -1) {
			TGen::Engine::Entity * self = *context.getRegister<TGen::Engine::Entity **>(TGen::Engine::RegisterSelf);
			std::cerr << "SELF IS " << self->getName() << std::endl;
		
			TGen::Engine::Component * component = comp.get(); //self->getComponent(componentId, std::nothrow);
		//std::cout << "COMPONENT " << componentId << " IS " << component->getName() << std::endl;
		
			callee = component;
		//exit(1);
		}
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
	if (!event) {
		if (!entityName.empty()) {
			std::string fixedComponentName = entityName;
		
			if (eventName.empty() && linker.getEntityList())
				event = dynamic_cast<TGen::Engine::Triggerable *>(linker.getEntity(entityName));
			else
				event = dynamic_cast<TGen::Engine::Triggerable *>(linker.getComponent(entityName + ":" + eventName));
		}
		else if (!eventName.empty() && registerId == -1) {
			event = dynamic_cast<TGen::Engine::Triggerable *>(linker.getComponent(eventName));
		}	
	
		comp.link(linker);
	}
}

void TGen::Engine::Script::CallOperation::prelink(const TGen::Engine::ComponentLinker & linker) {
	std::cout << "PRELL: " << entityName << std::endl;
	event = linker.getEntity(entityName);
	
	if (!event)
		comp.prelink(linker);
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
	//comp.set(entityName);
}


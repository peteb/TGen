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

TGen::Engine::Script::CallOperation::CallOperation(TGen::Engine::Script::EventOperation * parent)
	: TGen::Engine::Script::EventOperation(parent)
	, event(NULL)
	, registerId(-1)
	, offset(-1)
	, triggerMode(TGen::Engine::TriggerPrecise)
{
}

void TGen::Engine::Script::CallOperation::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	TGen::Engine::Triggerable * callee = event;
	
	if (registerId != -1)
		callee = *context.getRegister<TGen::Engine::Triggerable **>(registerId);
	
	if (callee) {
		//if (offset != -1)
			context.setRegister<int>(0, offset);
		
		if (shareContext || !callee->context) {
			callee->trigger(context, triggerMode);
		}
		else {
			callee->trigger(*callee->context, triggerMode);
		}
	}
}

void TGen::Engine::Script::CallOperation::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	if (!eventName.empty() && registerId == -1) {
		event = dynamic_cast<TGen::Engine::Triggerable *>(entities.getComponent(eventName, entity, std::nothrow));
		
		if (!event)
			throw TGen::RuntimeException("CallOperation::linkGlobally", "failed to get triggerable " + eventName);
	}	
}

void TGen::Engine::Script::CallOperation::setEvent(const std::string & eventName) {
	this->eventName = eventName;
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


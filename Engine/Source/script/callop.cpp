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

TGen::Engine::Script::CallOperation::CallOperation()
	: event(NULL)
{
}

void TGen::Engine::Script::CallOperation::trigger(TGen::Engine::TriggerContext & context) {
	if (event) {
		if (shareContext) {
			event->trigger(context);
		}
		else {
			event->trigger(event->getDefaultContext());
		}
	}
}

void TGen::Engine::Script::CallOperation::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	if (!eventName.empty()) {
		event = dynamic_cast<TGen::Engine::Triggerable *>(entities.getComponent(eventName, entity, std::nothrow));
		
		if (!event)
			throw TGen::RuntimeException("CallOperation::linkGlobally", "failed to get triggerable " + eventName);
	}	
}

void TGen::Engine::Script::CallOperation::setEvent(const std::string & eventName) {
	this->eventName = eventName;
}

void TGen::Engine::Script::CallOperation::setShareContext(bool shareContext) {
	this->shareContext = shareContext;
}


/*
 *  event.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "script/event.h"
#include "entity.h"

TGen::Engine::Script::Event::Event(const std::string & name, int symbolId)
	: TGen::Engine::Component(name)
	, TGen::Engine::Script::EventOperation(NULL)
	, symbolId(symbolId)
{

}


void TGen::Engine::Script::Event::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	if (symbolId != -1)
		entity.registerEvent(symbolId, this);

	TGen::Engine::Script::EventOperation::linkGlobally(entities, entity);
}

void TGen::Engine::Script::Event::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	if (context.getFunctionSymbol() == symbolId || mode == TGen::Engine::TriggerPrecise)
		TGen::Engine::Script::EventOperation::trigger(context, mode);
	else
		TGen::Engine::Component::trigger(context, mode);
}

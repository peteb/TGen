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
	, symbolId(symbolId)
{

}


void TGen::Engine::Script::Event::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	if (symbolId != -1)
		entity.registerEvent(symbolId, this);

	TGen::Engine::Script::EventOperation::linkGlobally(entities, entity);
}


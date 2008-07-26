/*
 *  eventrecipe.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/25/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "script/eventrecipe.h"
#include "script/event.h"
#include "entity.h"

TGen::Engine::Script::EventRecipe::EventRecipe(const std::string & name, TGen::Engine::Script::Event * event)
	: TGen::Engine::ComponentRecipe(name)
	, event(event)
{
}

TGen::Engine::Script::EventRecipe::~EventRecipe() {
	delete event;
}

void TGen::Engine::Script::EventRecipe::fastLinkConstructed(TGen::Engine::Component & constructed, TGen::Engine::Entity & entity) {
	constructed.setOwner(reinterpret_cast<TGen::Engine::Entity *>(0xBEEF));	// TODO: might not be a good idea to use pointers

	entity.registerEvent(event->getSymbol(), event);
}

TGen::Engine::Component * TGen::Engine::Script::EventRecipe::createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing) {
	return event;
}

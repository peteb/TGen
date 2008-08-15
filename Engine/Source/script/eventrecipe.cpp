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
#include "componentlinker.h"

TGen::Engine::Script::EventRecipe::EventRecipe(const std::string & name, TGen::Engine::Script::Event * event)
	: TGen::Engine::ComponentRecipe(name)
	, event(event)
{
}

TGen::Engine::Script::EventRecipe::~EventRecipe() {
	delete event;
}

void TGen::Engine::Script::EventRecipe::prelink(const TGen::Engine::ComponentLinker & linker) {
	if (linker.getEntity())
		linker.getEntity()->registerEvent(event->getSymbol(), event);
	
	event->prelink(linker);
}

void TGen::Engine::Script::EventRecipe::link(const TGen::Engine::ComponentLinker & linker) {
	//if (linker.getComponent())
	//	linker.getComponent()->setOwner(reinterpret_cast<TGen::Engine::Entity *>(0xBEEF));	// TODO: might not be a good idea to use pointers

	event->link(linker);
//	event->prelink(prototype);
}

TGen::Engine::Component * TGen::Engine::Script::EventRecipe::createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing) {
	return event;
}

// TODO: gör en generisk klass för det här, dock kommer inte event kunna använda den generiska, eller jo det borde gå genom arv

// den hittar inte target entity! länkas kanske fel! warning: failed to find component  or index -1 (list: 0)



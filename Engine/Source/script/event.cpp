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
#include "componentlinker.h"

TGen::Engine::Script::Event::Event(const std::string & name, TGen::Engine::Symbol symbolId)
	: TGen::Engine::Component(name, true)	// static component
	, TGen::Engine::Script::EventOperation(NULL)
	, symbolId(symbolId)
	, minCallTime(-1.0)
	, lastCall(-1.0)
{

}


void TGen::Engine::Script::Event::link(const TGen::Engine::ComponentLinker & linker) {
	TGen::Engine::Component::link(linker);
	
	if (symbolId != -1 && linker.getEntity())
		linker.getEntity()->registerEvent(symbolId, this);

	TGen::Engine::Script::EventOperation::link(linker);
}

void TGen::Engine::Script::Event::linkRecipe(const TGen::Engine::EntityRecipe & recipe) {
	TGen::Engine::Script::EventOperation::linkRecipe(recipe);
}

void TGen::Engine::Script::Event::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	double now = TGen::Time::Now();

	//std::cout << std::fixed << "NOW: " << now << " LAST: " << lastCall << " MIN: " << minCallTime << std::endl;
	// scalar is not enough for the big stupid numbers generated, they are in unix time, epoch!!
	
	if (now - lastCall >= minCallTime) {		
		lastCall = now;
		
		if (context.getFunctionSymbol() == symbolId || mode == TGen::Engine::TriggerPrecise)
			TGen::Engine::Script::EventOperation::trigger(context, mode);
		else
			TGen::Engine::Component::trigger(context, mode);
	}
	else {
		TGen::Engine::Component::trigger(context, mode);		
	}
}

void TGen::Engine::Script::Event::setMinCallInterval(scalar interval) {
	this->minCallTime = interval;
}

TGen::Engine::Symbol TGen::Engine::Script::Event::getSymbol() const {
	return symbolId;
}

